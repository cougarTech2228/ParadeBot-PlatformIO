#ifndef __BALLSHOOTER_H__
#define __BALLSHOOTER_H__
#include <Servo.h>
#include "radiolink_t8s.h"
#include "subsystem.h"

class BallShooter : public Subsystem
{
public:
    BallShooter();
    void processInput(RCController *controller);

private:
    Servo ballShooterMotor;
};
#endif