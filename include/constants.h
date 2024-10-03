#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

static const int ENABLE_BOT_BUTTON_PIN = 27;
static const int CHILD_MODE_BUTTON_PIN = 26;
static const int ENABLE_BOT_LED_PIN = 29;
static const int CHILD_MODE_LED_PIN = 28;

static const int RIGHT_DRIVE_MOTOR_PIN = 13;
static const int LEFT_DRIVE_MOTOR_PIN = 14;

static const int BALL_SHOOTER_LEFT_PIN = 7;
static const int BALL_SHOOTER_RIGHT_PIN = 8;
static const int TURRET_MOTOR_PIN = 6;
static const int TURRET_LIMIT_LEFT = 24;
static const int CANDY_LOADER_MOTOR_PIN = 5;

static const int BALL_CYLINDER_RELAY_PIN = 22;
static const int CANDY_SHOOTER_RELAY_PIN = 23;

static const int LED_DATA_PIN = 30;

static const int TURRET_LIMIT_LEFT_PIN = 31;
static const int TURRET_LIMIT_RIGHT_PIN = 32;

static const int NUM_LEDS = 150;

static const int TURRET_LIMIT_RIGHT = 25;
static const int TURRET_ENCODER_PIN = A0;

static const int MIN_PWM_SIGNAL_WIDTH = 1000;
static const int MAX_PWM_SIGNAL_WIDTH = 2000;

static const int SERVO_FULL_REVERSE = 160;
static const int SERVO_STOPPED = 90;
static const int SERVO_FULL_FORWARD = 20;
static const int SERVO_DEADBAND = 8;
static const int SERVO_SAFETY_MARGIN = 20;

static const float CHILD_MODE_THROTTLE_FACTOR = .2;
static const float CHILD_MODE_TURN_FACTOR = .2;
static const float CHILD_MODE_STRAFE_FACTOR = .3;

static const int TURN_CORRECTION = 10;
static const int TURN_COMPENSATION = 7;

static const int BUTTON_DEBOUNCE_TIME_MS = 300;

static const int SCALED_DEADBAND = 10; // 5% deadband because scaled values are -100 to 100

enum RobotMode
{
    BALL,
    CANDY,
    UNKNOWN
};

#endif