#ifndef __RADIOLINK_T8S_H__
#define __RADIOLINK_T8S_H__

#include "sbus_processor.h"
#include "rc_controller.h"

class RadiolinkT8S : public RCController
{
public:
    RadiolinkT8S(SbusProcessor *SbusController);
    int getAxis(Axis axis);
    TogglePosition getToggle(Toggle toggle);
    bool isButtonPressed(Button button);

private:
    SbusProcessor *m_sbusController;

    /* Radio link channels */
    static const int RIGHT_STICK_X = 1;
    static const int LEFT_STICK_Y = 3;
    static const int RIGHT_STICK_Y = 2;
    static const int LEFT_STICK_X = 4;
    static const int RIGHT_TOGGLE = 5;
    static const int BUTTON = 6;
    static const int LEFT_TOGGLE = 7;
    static const int DIAL = 8;

    static const int RL_TOGGLE_BOTTOM = 200;
    static const int RL_TOGGLE_MIDDLE = 1000;
    static const int RL_TOGGLE_TOP = 1800;

    static const int RL_BUTTON_PRESSED = 1800;
    static const int RL_BUTTON_NOT_PRESSED = 200;

    static const int RADIOLINK_CONTROLLER_MINIMUM_VALUE = 200;
    static const int RADIOLINK_CONTROLLER_NEUTRAL_VALUE = 1000;
    static const int RADIOLINK_CONTROLLER_MAXIMUM_VALUE = 1800;

    int radioLinkLeftX = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
    int radioLinkLeftY = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
    int radioLinkRightX = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
    int radioLinkRightY = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
    int radioLinkToggleRight = -1;
    int radioLinkToggleLeft = -1;
    int radioLinkButton = -1;

    static TogglePosition convertTogglePosition(int value);
    static void sbusCallback(const int channels[18], void *this_pointer);
    void onSbusPacket(const int channels[18]);

};
#endif
