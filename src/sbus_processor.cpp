#include "sbus_processor.h"
#include "constants.h"
#include <Arduino.h>

SbusProcessor::SbusProcessor(HardwareSerial *sbusPort, HardwareSerial *dbgPort) : m_dbgPort(dbgPort), m_sbusPort(sbusPort)
{
    m_callbackHander = NULL;
    sBusPacketsLost = 0;

    // The SBUS is a non standard baud rate of 100 kbs
    sbusPort->begin(100000, SERIAL_8E2);
    // put your setup code here, to run once:
    sbusPort->flush();
    dbgPort->println("sbusPort setup");
}

/**************************************************************
   processControllerData()
 **************************************************************/
bool SbusProcessor::processControllerData()
{
    static int sBusErrors = 0;
    static int sBusByteIndex = 0;
    byte nextSBusByte;
    bool packetReady = false;

    // Check the SBus serial port for incoming SBus data
    if (m_sbusPort->available())
    {
        nextSBusByte = m_sbusPort->read();
        // handle drive motors is called from inside processControllerData();
        // This is a new package and it's not the first byte then it's probably the start byte B11110000 (sent MSB)
        // so start reading the 25 byte packet
        if ((sBusByteIndex == 0) && (nextSBusByte != 0x0F))
        {
            // Serial.println("Enabled");
            // error - keep waiting for the start byte
        }
        else
        {
            // fill the buffer with the bytes until the end byte B0000000 is received
            sBusBuffer[sBusByteIndex++] = nextSBusByte;
        }

        // If we've got 25 bytes then this is a good packet so start to decode
        if (sBusByteIndex == 25)
        {
            sBusByteIndex = 0;

            if (sBusBuffer[24] == 0x00)
            {
                // m_dbgPort->println("Got packet");
                processSBusBuffer();
                packetReady = true;
            }
            else
            {
                //Serial.println("Error!");
                sBusErrors++; //?????
            }
        }
    }
    return packetReady;
}

void(* resetFunc) (void) = 0;

/**************************************************************
   processSBusBuffer()
 **************************************************************/
void SbusProcessor::processSBusBuffer()
{

    // 25 byte packet received is little endian. Details of how the package is explained on this website:
    // http://www.robotmaker.eu/ROBOTmaker/quadcopter-3d-proximity-sensing/sbus-graphical-representation

    channels[1] = ((sBusBuffer[1] | sBusBuffer[2] << 8) & 0x07FF);
    channels[2] = ((sBusBuffer[2] >> 3 | sBusBuffer[3] << 5) & 0x07FF);
    channels[3] = ((sBusBuffer[3] >> 6 | sBusBuffer[4] << 2 | sBusBuffer[5] << 10) & 0x07FF);
    channels[4] = ((sBusBuffer[5] >> 1 | sBusBuffer[6] << 7) & 0x07FF);
    channels[5] = ((sBusBuffer[6] >> 4 | sBusBuffer[7] << 4) & 0x07FF);
    channels[6] = ((sBusBuffer[7] >> 7 | sBusBuffer[8] << 1 | sBusBuffer[9] << 9) & 0x07FF);
    channels[7] = ((sBusBuffer[9] >> 2 | sBusBuffer[10] << 6) & 0x07FF);
    channels[8] = ((sBusBuffer[10] >> 5 | sBusBuffer[11] << 3) & 0x07FF);
    channels[9] = ((sBusBuffer[12] | sBusBuffer[13] << 8) & 0x07FF);
    channels[10] = ((sBusBuffer[13] >> 3 | sBusBuffer[14] << 5) & 0x07FF);
    channels[11] = ((sBusBuffer[14] >> 6 | sBusBuffer[15] << 2 | sBusBuffer[16] << 10) & 0x07FF);
    channels[12] = ((sBusBuffer[16] >> 1 | sBusBuffer[17] << 7) & 0x07FF);
    channels[13] = ((sBusBuffer[17] >> 4 | sBusBuffer[18] << 4) & 0x07FF);
    channels[14] = ((sBusBuffer[18] >> 7 | sBusBuffer[19] << 1 | sBusBuffer[20] << 9) & 0x07FF);
    channels[15] = ((sBusBuffer[20] >> 2 | sBusBuffer[21] << 6) & 0x07FF);
    channels[16] = ((sBusBuffer[21] >> 5 | sBusBuffer[22] << 3) & 0x07FF);
    channels[17] = ((sBusBuffer[23]) & 0x0001) ? 2047 : 0;
    channels[18] = ((sBusBuffer[23] >> 1) & 0x0001) ? 2047 : 0;

    /*
      Serial.print("CH1: ");
      Serial.println(channels[1]);

      Serial.print("CH2: ");
      Serial.println(channels[2]);

      Serial.print("CH3: ");
      Serial.println(channels[3]);

      Serial.print("CH4: ");
      Serial.println(channels[4]);
      Serial.print("CH5: ");
      Serial.println(channels[5]);
      Serial.print("CH6: ");
      Serial.println(channels[6]);
      Serial.print("CH7: ");
      Serial.println(channels[7]);
      Serial.print("CH8: ");
      Serial.println(channels[8]);
    */
    // Check for signal loss
    if ((sBusBuffer[23] >> 2) & 0x0001)
    {
        sBusPacketsLost++;
        Serial.print("Signal Lost: ");
        Serial.println(sBusPacketsLost);

        // Make sure the robot doesn't move when the signal is lost
        /*radioLinkTurnValue = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
          radioLinkThrottleValue = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
          radioLinkStrafeValue = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;*/
        if (sBusPacketsLost > 10) {
          resetFunc();
        }
    }
    else // Everything is okay so process the current values
    {
        receivedOneSBusPacketSinceReset = true;
        if (m_callbackHander != NULL) {
            // m_dbgPort->println("calling handler");
            m_callbackHander(channels, m_callbackCtx);
        }
    }

    /*
      Serial.print("Throttle: ");
      Serial.print(radioLinkThrottleValue);
      Serial.print(" Turn: ");
      Serial.println(radioLinkTurnValue);
    */
    /*
      if (channels[RADIOLINK_SHOOT_CANDY_CHANNEL] == RADIOLINK_CONTROLLER_MAXIMUM_VALUE)
      {
        Serial.println("Fail Safe Enabled");
        failSafeEnabled = true;
        radioLinkTurnValue = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
        radioLinkThrottleValue = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
        radioLinkStrafeValue = RADIOLINK_CONTROLLER_NEUTRAL_VALUE;
      }

      if (channels[RADIOLINK_AUTO_CHANNEL] == 200) {
        isAuto = true;
        //setGyroYawOffset();
      }
      else {
        isAuto = false;
      }
    */
}

void SbusProcessor::registerHandler(callback_t callback, void *ctx)
{
    m_callbackCtx = ctx;
    m_callbackHander = callback;
}

