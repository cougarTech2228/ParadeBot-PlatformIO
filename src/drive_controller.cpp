#include "drive_controller.h"
#include "constants.h"
#include "parade_bot.h"
#include <Arduino.h>

static int NON_CHILD_MAX_SPEED = 45;
static int CHILD_MAX_SPEED = 20;

DriveController::DriveController()
{
    leftDriveMotor.attach(LEFT_DRIVE_MOTOR_PIN,
                          MIN_PWM_SIGNAL_WIDTH, MAX_PWM_SIGNAL_WIDTH);
    rightDriveMotor.attach(RIGHT_DRIVE_MOTOR_PIN,
                           MIN_PWM_SIGNAL_WIDTH, MAX_PWM_SIGNAL_WIDTH);

    // Set the drive motors to their "stopped" position
    stopDriveMotors();
}
/**************************************************************
   handleDriveMotors()
 **************************************************************/
void DriveController::processInput(RCController *controller)
{
    int driveX = controller->getAxis(RCController::RIGHT_X);
    int driveY = controller->getAxis(RCController::RIGHT_Y);

    float throttle = -map(driveY, 200, 1800, -100, 100);
    float turn = -map(driveX, 200, 1800, -100, 100);

    int tempRight = ((100 - abs(turn)) * (throttle / (float)100)) + throttle;
    int tempLeft = ((100 - abs(throttle)) * (turn / (float)100)) + turn;

    int maxMotorSpeed;
    int minMotorSpeed;

    if (ParadeBot::childModeEnabled())
    {
        maxMotorSpeed = 90 + CHILD_MAX_SPEED;
        minMotorSpeed = 90 - CHILD_MAX_SPEED;
    }
    else
    {
        maxMotorSpeed = 90 + NON_CHILD_MAX_SPEED;
        minMotorSpeed = 90 - NON_CHILD_MAX_SPEED;
    }

    int right = (tempRight + tempLeft) / 2;
    int left = (tempRight - tempLeft) / 2;

    right = map(right, -100, 100, maxMotorSpeed, minMotorSpeed);
    left = map(left, -100, 100, minMotorSpeed, maxMotorSpeed);

    rightDriveMotor.write(right);
    leftDriveMotor.write(left);

    // Serial.print(right);
    // Serial.print(", ");
    // Serial.println(left);
}
/**************************************************************
   applyDeadband()
 **************************************************************/
int DriveController::applyDeadband(int value)
{
    if (abs(value) <= SCALED_DEADBAND)
    {
        return 0;
    }
    else
    {
        return value;
    }
}
/**************************************************************
   stopDriveMotors()
 **************************************************************/
void DriveController::stopDriveMotors()
{
    leftDriveMotor.write(SERVO_STOPPED);
    rightDriveMotor.write(SERVO_STOPPED);
}
