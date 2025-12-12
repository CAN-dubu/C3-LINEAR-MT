#ifndef MOTOR_H
#define MOTOR_H

#include "../hw_def.h"

#define MOTOR_MAX_CH       HW_MOTOR_MAX_CH

#define MOTOR_PWM_8BIT_RESOLUTION    8
#define MOTOR_PWM_16BIT_RESOLUTION  16

#define MOTOR_PWM_FREQ 5000
#define MOTOR_MAX_SPEED 255
#define MOTOR_STOP_SPEED 0 

#define MOTOR_IN1_PIN 18
#define MOTOR_IN2_PIN 19

typedef enum
{
  MOTOR_IDLE,
  MOTOR_UP,
  MOTOR_DOWN,
  MOTOR_STOP,
  MOTOR_LOCK
} motor_state_t;

bool motorInit(void);
void motorGoUp(uint8_t ch);
void motorGoDown(uint8_t ch);
void motorStop(uint8_t ch);
void motorLock(uint8_t ch);
bool motorGetLocked(uint8_t ch);

#endif