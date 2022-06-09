#ifndef __DRIVE_CONTROLLER_H__
#define __DRIVE_CONTROLLER_H__
#include <Servo.h>
#include "radiolink_t8s.h"
#include "subsystem.h"

class DriveController : public Subsystem
{
public:
    DriveController();
    void processInput(RCController *controller);
    void stopDriveMotors();

private:
    int applyDeadband(int value);
    Servo rightDriveMotor;
    Servo leftDriveMotor;
};
#endif