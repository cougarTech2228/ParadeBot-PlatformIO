#include <Arduino.h>
#include "parade_bot.h"

static ParadeBot m_paradeBot;

void setup()
{
    m_paradeBot.do_setup();
}

void loop()
{
    m_paradeBot.do_loop();
}