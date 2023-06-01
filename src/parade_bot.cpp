#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>
#include "parade_bot.h"
#include "subsystem.h"
#include "ball_shooter.h"
#include "drive_controller.h"
#include "candy_shooter.h"
#include "led_subsystem.h"
#include "sbus_processor.h"
#include "radiolink_t8s.h"
#include <FastLED.h>

// Timed loop variables
unsigned long startTime = millis();
int countIterations = 0;

// SbusController m_sbusController = SbusController(Serial1, Serial);
// BallShooter m_ballShooter = BallShooter(m_sbusController);
// DriveController m_driveController = DriveController(m_sbusController);
// CandyShooter m_candyShooter = CandyShooter(m_sbusController);


RobotMode ParadeBot::m_robotMode;
bool ParadeBot::m_childModeEnabled;

/**************************************************************
   childModeButtonPressed()
 **************************************************************/
void ParadeBot::childModeButtonPressed()
{
    Serial.println("Child mode pressed");
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    // If interrupts come faster than BUTTON_DEBOUNCE_TIME_MS, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > BUTTON_DEBOUNCE_TIME_MS)
    {

        // Toggle the childModeEnabled flag
        m_childModeEnabled = !m_childModeEnabled;
    }

    last_interrupt_time = interrupt_time;
}

void setupButtonInterrupts()
{
    // pinMode(ENABLE_BOT_BUTTON_PIN, INPUT);
    // attachInterrupt(digitalPinToInterrupt(ENABLE_BOT_BUTTON_PIN), enableBotButtonPressed, FALLING);

    pinMode(CHILD_MODE_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CHILD_MODE_BUTTON_PIN), ParadeBot::childModeButtonPressed, FALLING);
}

ParadeBot::ParadeBot()
{
    m_robotMode = RobotMode::UNKNOWN;
}

void ParadeBot::do_setup()
{
    wdt_disable();

    // Setup debug/monitor serial port
    Serial.begin(115200);

    Serial.println("ParadeBot::do_setup()");

    m_sbusController = new SbusProcessor(&Serial1, &Serial);
    m_controller = new RadiolinkT8S(m_sbusController);
    m_ballShooter = new BallShooter();
    m_driveController = new DriveController();
    m_candyShooter = new CandyShooter();
    m_ledSubsystem = new LedSubsystem(NUM_LEDS);
    m_ledSubsystem->showLED();

    m_driveController->stopDriveMotors();

    // sets up interrupts for the child mode and enable buttons
    setupButtonInterrupts();

    // pinMode(A12, INPUT);
    pinMode(CHILD_MODE_LED_PIN, OUTPUT);
    pinMode(ENABLE_BOT_LED_PIN, OUTPUT);
    pinMode(ENABLE_BOT_BUTTON_PIN, INPUT_PULLUP);

    digitalWrite(CHILD_MODE_LED_PIN, 0);
    digitalWrite(ENABLE_BOT_LED_PIN, 0);
    Serial.println("Press enable to start");
    int ledStatus = 0;
    int ledLastChanged = 0;
    while (digitalRead(ENABLE_BOT_BUTTON_PIN) == 1)
    {
        if (millis() - ledLastChanged > 250)
        {
            ledStatus = !ledStatus;
            digitalWrite(ENABLE_BOT_LED_PIN, ledStatus);
            ledLastChanged = millis();
        }
    }
    wdt_enable(WDTO_1S);
    digitalWrite(ENABLE_BOT_LED_PIN, 1);

    Serial.println("Set up done");
}

/**************************************************************
   timedLoop()
 **************************************************************/
void ParadeBot::timedLoop()
{
    unsigned long currTime = millis();
    if (currTime >= startTime + 10 * countIterations)
    {
        countIterations++;

        // this shouldn't be called but if it is it will reset the loop
        if (countIterations > 100)
        {
            countIterations = 0;
            startTime = millis();
        }

        // 1 second
        if (countIterations % 100 == 0)
        {
            countIterations = 0;
            startTime = millis();
        }

        // 500 milliseconds
        if (countIterations % 50 == 0)
        {
        }

        // 250 milliseconds
        if (countIterations % 25 == 0)
        {
            //      updateLEDs();
        }

        // 100 milliseconds
        if (countIterations % 10 == 0)
        {
        }
    }
}

/**************************************************************
   loop()
 **************************************************************/
int childModeRead = 1;
int childModeDebounceCount = 0;
void ParadeBot::do_loop()
{
    // Serial.println("ParadeBot::do_loop()");

    // showLED();
    

    int currentChildModeButton = digitalRead(CHILD_MODE_BUTTON_PIN);
    // childModeEnabled = currentChildModeButton;
    m_childModeEnabled = (currentChildModeButton == 0);
    digitalWrite(CHILD_MODE_LED_PIN, m_childModeEnabled);
    /*
    if(currentChildModeButton != childModeRead){
      if(currentChildModeButton == 1 && childModeDebounceCount >= 100){
        childModeEnabled = !childModeEnabled;
      }
      childModeRead = currentChildModeButton;
    }
    else{
      if(currentChildModeButton == 0){
        childModeDebounceCount++;
        //Serial.println(childModeDebounceCount);
      }
      else{
        if(childModeDebounceCount > 0){
          Serial.println("Clearing");
        }
        childModeDebounceCount = 0;
      }
    }
    */
    timedLoop();
    //Serial.println("kick");
    //wdt_reset();
    if (m_sbusController->processControllerData())
    {
        wdt_reset();
        switch (m_controller->getToggle(RCController::TOGGLE_RIGHT))
        {
        case RCController::TOGGLE_BOTTOM:
            if (m_robotMode != RobotMode::BALL) {
                Serial.println("Entering BALL Mode");
                m_robotMode = RobotMode::BALL;
            }
            break;
        case RCController::TOGGLE_MIDDLE:
            if (m_robotMode != RobotMode::UNKNOWN) {
                Serial.println("Entering UNKNOWN Mode");
                m_robotMode = RobotMode::UNKNOWN;
            }
            break;
        case RCController::TOGGLE_TOP:
            if (m_robotMode != RobotMode::CANDY) {
                Serial.println("Entering CANDY Mode");
                m_robotMode = RobotMode::CANDY;
            }
            break;
        default:
            m_robotMode = RobotMode::UNKNOWN;
            break;
        }
        m_driveController->processInput(m_controller);
        m_ballShooter->processInput(m_controller);
        m_candyShooter->processInput(m_controller);
    }
    digitalWrite(CHILD_MODE_LED_PIN, m_childModeEnabled);
}
