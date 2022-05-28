// These speeds are out of 128
#include <Arduino.h>
#include "ball_shooter.h"
#include "constants.h"
#include "parade_bot.h"

static const int BALL_LOAD_SPEED = 40;
static const int BALL_SHOOT_SPEED = 100;

BallShooter::BallShooter()
{
    pinMode(BALL_CYLINDER_RELAY_PIN, OUTPUT);
    ballShooterMotor.attach(BALL_SHOOTER_LEFT_PIN,
                            MIN_PWM_SIGNAL_WIDTH, MAX_PWM_SIGNAL_WIDTH);
    ballShooterMotor.write(SERVO_STOPPED);
}

void BallShooter::processInput(RCController *controller)
{
    if (ParadeBot::getRobotMode() != RobotMode::BALL)
    {
        return;
    }
    int controlX = controller->getAxis(RCController::LEFT_X);
    int controlY = controller->getAxis(RCController::LEFT_Y);

    Serial.println("In ball mode");
    if (controlY <= 500)
    {
        Serial.println("raising cyl");
        digitalWrite(BALL_CYLINDER_RELAY_PIN, 1);
    }
    else if (controlY >= 1500)
    {
        Serial.println("lowering cyl");
        digitalWrite(BALL_CYLINDER_RELAY_PIN, 0);
    }

    if (controlX <= 500)
    {
        Serial.println("loading");
        //        analogWrite(BALL_SHOOTER_LEFT_PIN, 128 - BALL_LOAD_SPEED);
        //        analogWrite(BALL_SHOOTER_RIGHT_PIN, 128 + BALL_LOAD_SPEED);
        ballShooterMotor.write(30);
    }
    else if (controller->isButtonPressed(RCController::BUTTON_1))
    {
        Serial.println("shooting");
        //        analogWrite(BALL_SHOOTER_LEFT_PIN, 128 - BALL_SHOOT_SPEED);
        //        analogWrite(BALL_SHOOTER_RIGHT_PIN, 128 + BALL_SHOOT_SPEED);
        ballShooterMotor.write(150);
    }
    else
    {
        Serial.println("none");
        // analogWrite(BALL_SHOOTER_LEFT_PIN, 128);
        // analogWrite(BALL_SHOOTER_RIGHT_PIN, 128);
        ballShooterMotor.write(SERVO_STOPPED);
    }
}
