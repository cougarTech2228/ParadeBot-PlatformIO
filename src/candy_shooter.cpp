#include "candy_shooter.h"
#include "constants.h"
#include "parade_bot.h"
#include <Arduino.h>

static const int TURRET_SPEED = 35;
static const int ELEVATOR_SPEED = 30;
static const int ENCODER_MIN = 90;
static const int ENCODER_MAX = 1023;

CandyShooter::CandyShooter()
{
    pinMode(CANDY_SHOOTER_RELAY_PIN, OUTPUT);
    pinMode(TURRET_LIMIT_LEFT_PIN, INPUT_PULLUP);
    pinMode(TURRET_LIMIT_RIGHT_PIN, INPUT_PULLUP);


    turretMotor.attach(TURRET_MOTOR_PIN,
                       MIN_PWM_SIGNAL_WIDTH, MAX_PWM_SIGNAL_WIDTH);
    candyLoaderMotor.attach(CANDY_LOADER_MOTOR_PIN,
                            MIN_PWM_SIGNAL_WIDTH, MAX_PWM_SIGNAL_WIDTH);
}

void CandyShooter::processInput(RCController *controller)
{
    if (ParadeBot::getRobotMode() != RobotMode::CANDY)
    {
        return;
    }

    //Serial.println("In candy mode");

    int controlX = controller->getAxis(RadiolinkT8S::LEFT_X);
    int controlY = controller->getAxis(RadiolinkT8S::LEFT_Y);

    // if (controlY <= 500)
    // {
    //     Serial.println("Raising elevator");
    //     candyLoaderMotor.write(90 + ELEVATOR_SPEED);
    // }
    if (controlY >= 1500)
    {
        Serial.println("Raising elevator");
        candyLoaderMotor.write(90 + ELEVATOR_SPEED);
    }
    else
    {
        //Serial.println("Elevator not moving");
        candyLoaderMotor.write(90);
    }

    int leftLimit = !digitalRead(TURRET_LIMIT_LEFT_PIN);
    int rightLimit = !digitalRead(TURRET_LIMIT_RIGHT_PIN);
    // Serial.print("Left limit is: ");
    // Serial.print(leftLimit);
    // Serial.print(" Right limit is: ");
    // Serial.println(rightLimit);

    // TODO: add limit switches

    if (controlX <= 500 && !leftLimit)
    {
        Serial.println("turret turn left");
        turretMotor.write(90 + TURRET_SPEED);
    }
    else if (controlX >= 1500 && !rightLimit)
    {
        Serial.println("turret turn right");
        turretMotor.write(90 - TURRET_SPEED);
    }
    else
    {
        //Serial.println("turret not moving");
        turretMotor.write(90);
    }

    if (controller->isButtonPressed(RCController::BUTTON_1))
    {
        Serial.println("shooting");
        digitalWrite(CANDY_SHOOTER_RELAY_PIN, 1);
    }
    else
    {
        digitalWrite(CANDY_SHOOTER_RELAY_PIN, 0);
    }
}
