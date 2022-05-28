#ifndef __SBUS_PROCESSOR_H__
#define __SBUS_PROCESSOR_H__
#include <Arduino.h>
// More information on the SBus: https://github.com/uzh-rpg/rpg_quadrotor_control/wiki/SBUS-Protocol

class SbusProcessor
{
public:
	typedef void (*callback_t)(const int channels[18], void *ctx);

    SbusProcessor(HardwareSerial sbusPort, HardwareSerial dbgPort);
    bool processControllerData();
    void registerHandler(callback_t callback, void *ctx);

private:
    callback_t m_callbackHander;
    void *m_callbackCtx;

    int channels[18];
    byte sBusBuffer[25];
    int sBusPacketsLost;
    void processSBusBuffer();
    bool receivedOneSBusPacketSinceReset;

    HardwareSerial m_dbgPort;
    HardwareSerial m_sbusPort;
};
#endif