#ifndef __RC_CONTROLLER_H__
#define __RC_CONTROLLER_H__

class RCController
{
public:
    enum Axis
    {
        LEFT_X,
        LEFT_Y,
        RIGHT_X,
        RIGHT_Y
    };
    enum Toggle
    {
        TOGGLE_LEFT,
        TOGGLE_RIGHT
    };

    enum TogglePosition
    {
        TOGGLE_BOTTOM,
        TOGGLE_MIDDLE,
        TOGGLE_TOP,
        TOGGLE_UNKNOWN
    };

    enum Button
    {
        BUTTON_1
    };

    virtual int getAxis(Axis axis) = 0;
    virtual TogglePosition getToggle(Toggle toggle) = 0;
    virtual bool isButtonPressed(Button button) = 0;
};
#endif