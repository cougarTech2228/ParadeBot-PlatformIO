#ifndef __PARADE_BOT_H__
#define __PARADE_BOT_H__

#include "constants.h"

class SbusProcessor;
class BallShooter;
class DriveController;
class CandyShooter;
class RCController;

class ParadeBot
{
public:
    ParadeBot();
    void do_setup();
    void do_loop();
    static RobotMode getRobotMode() { return m_robotMode; }
    static bool childModeEnabled() { return m_childModeEnabled; }
    static void childModeButtonPressed();

private:
    static RobotMode m_robotMode;
    static bool m_childModeEnabled;
    SbusProcessor *m_sbusController;
    BallShooter *m_ballShooter;
    DriveController *m_driveController;
    CandyShooter *m_candyShooter;
    RCController *m_controller;
};

#endif