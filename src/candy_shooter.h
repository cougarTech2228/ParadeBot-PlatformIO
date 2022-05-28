#ifndef __CANDY_SHOOTER_H__
#define __CANDY_SHOOTER_H__

#include <Servo.h>
#include "radiolink_t8s.h"
#include "subsystem.h"

class CandyShooter : public Subsystem
{
public:
    CandyShooter();
    void processInput(RCController *controller);

private:
    Servo turretMotor;
    Servo candyLoaderMotor;
};

#endif