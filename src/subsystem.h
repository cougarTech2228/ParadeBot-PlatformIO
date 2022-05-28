#ifndef __SUBSYSTEM_H__
#define __SUBSYSTEM_H__
#include "rc_controller.h"

class Subsystem
{
public:
    virtual void processInput(RCController *controller) = 0;
};

#endif
