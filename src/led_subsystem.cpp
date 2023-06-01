#include <led_subsystem.h>
#include "FastLED.h"
#include "constants.h"
#include <protothreads.h>

// Pride2015
// Animated, ever-changing rainbows.
// by Mark Kriegsman

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

//#define DATA_PIN    40
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
//#define NUM_LEDS    150
#define BRIGHTNESS  255

// pt ptThread;

LedSubsystem::LedSubsystem(uint32_t numLED) {
    m_numLED = numLED;
    leds = (CRGB*)malloc(sizeof(CRGB) * numLED);
    setupLED();
}


// int ledThread(struct pt* pt, LedSubsystem *subsystem) {
//   PT_BEGIN(pt);

//   // Loop forever
//   for(;;) {
//     subsystem->showLED();
//     PT_SLEEP(pt, 20);
// 	// if (buttonState == HIGH) {
// 	// 	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
// 	// 	PT_SLEEP(pt, 200);
// 	// 	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
// 	// 	PT_SLEEP(pt, 100);
// 	// } else {
// 	// 	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
// 	// 	PT_YIELD(pt);
// 	// }
//   }

//   PT_END(pt);
// }

void LedSubsystem::processInput(RCController *controller) {
    // no-op
}

void LedSubsystem::loop() {
   // PT_SCHEDULE(ledThread(&ptThread, this));
   showLED();
}

void LedSubsystem::setupLED() {
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_DATA_PIN,COLOR_ORDER>(leds, m_numLED)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

//   PT_INIT(&ptThread);
  Serial.println("LedSubsystem::setupLED()");

//   PT_SCHEDULE(ledThread(&ptThread, this));
}


void LedSubsystem::showLED()
{
    //pride();
    //juggle();
    //confetti();
    rainbow();
    //rainbowWithGlitter();
    FastLED.show();
}


// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void LedSubsystem::pride() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < m_numLED; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (m_numLED - 1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}

void LedSubsystem::juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, m_numLED, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, m_numLED - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void LedSubsystem::confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, m_numLED, 10);
  int pos = random16(m_numLED);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void LedSubsystem::rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, m_numLED, gHue, 7);
}

void LedSubsystem::rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void LedSubsystem::addGlitter( uint8_t chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(m_numLED) ] += CRGB::White;
  }
}
