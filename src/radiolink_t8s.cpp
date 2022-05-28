#include "radiolink_t8s.h"

void RadiolinkT8S::sbusCallback(const int channels[18], void *this_pointer)
{
    RadiolinkT8S *self = static_cast<RadiolinkT8S *>(this_pointer);
    self->onSbusPacket(channels);
}

RadiolinkT8S::RadiolinkT8S(SbusProcessor *sbusController) : m_sbusController(sbusController)
{
    sbusController->registerHandler(&RadiolinkT8S::sbusCallback, (void*)this);
}

void RadiolinkT8S::onSbusPacket(const int channels[18])
{
    radioLinkRightX = channels[RIGHT_STICK_X];
    radioLinkRightY = channels[RIGHT_STICK_Y];
    radioLinkLeftX = channels[LEFT_STICK_X];
    radioLinkLeftY = channels[LEFT_STICK_Y];
    radioLinkToggleRight = channels[RIGHT_TOGGLE];
    radioLinkToggleLeft = channels[LEFT_TOGGLE];
    radioLinkButton = channels[BUTTON];
}

int RadiolinkT8S::getAxis(Axis axis)
{
    int value = 0;
    switch (axis)
    {
    case LEFT_X:
        value = radioLinkLeftX;
        break;
    case LEFT_Y:
        value = radioLinkLeftY;
        break;
    case RIGHT_X:
        value = radioLinkRightX;
        break;
    case RIGHT_Y:
        value = radioLinkRightY;
        break;
    }
    return value;
}

RCController::TogglePosition RadiolinkT8S::convertTogglePosition(int value)
{
    TogglePosition pos = TOGGLE_UNKNOWN;
    switch (value)
    {
    case RL_TOGGLE_BOTTOM:
        pos = TOGGLE_BOTTOM;
        break;
    case RL_TOGGLE_MIDDLE:
        pos = TOGGLE_MIDDLE;
        break;
    case RL_TOGGLE_TOP:
        pos = TOGGLE_TOP;
        break;
    }
    return pos;
}

RCController::TogglePosition RadiolinkT8S::getToggle(Toggle side)
{
    TogglePosition pos = TOGGLE_UNKNOWN;
    switch (side)
    {
    case TOGGLE_LEFT:
        pos = convertTogglePosition(radioLinkToggleLeft);
        break;
    case TOGGLE_RIGHT:
        pos = convertTogglePosition(radioLinkToggleRight);
        break;
    };
    return pos;
}

bool RadiolinkT8S::isButtonPressed(Button button)
{
    bool pressed = false;
    switch (button)
    {
    case BUTTON_1:
        pressed = radioLinkButton == RL_BUTTON_PRESSED;
        break;
    }
    return pressed;
}