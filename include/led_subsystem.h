#ifndef __LED_H__
#define __LED_H__
#include "subsystem.h"
#include <FastLED.h>

class LedSubsystem : public Subsystem
{
public:
    LedSubsystem(uint32_t numLED);
    void processInput(RCController *controller);
    void loop();


    void showLED();
    void pride();
    void juggle();
    void confetti();
    void rainbow();
    void rainbowWithGlitter();
    void addGlitter( uint8_t chanceOfGlitter);

private:
    void setupLED();

    uint32_t m_numLED;
    CRGB *leds;
    uint8_t gHue = 0; // rotating "base color" used by many of the patterns
};
#endif