#include "ap_motor.h"

typedef struct
{
  bool is_locked;
  ap_motor_action_t pre_action;
} ap_motor_t;

static ap_motor_t ap_motor[MOTOR_MAX_CH];

bool apMotorInit(void)
{
  bool ret = true;
  
  for (int i=0; i<MOTOR_MAX_CH; i++)
  {
    ap_motor[i].is_locked  = false;
    ap_motor[i].pre_action = MODE_ACTION_NONE;
  }

  return ret;
}

void motorRequestAction(uint8_t ch, ap_motor_action_t action)
{
  if (ch >= MOTOR_MAX_CH)
  {
    return;
  }

  if (ap_motor[ch].is_locked == true && action != MODE_ACTION_MOTOR_LOCK) // lock상태이면 모두 out
  {
    return;
  }

  switch (action)
  {
    case MODE_ACTION_MOTOR_UP:

      if (ap_motor[ch].pre_action == MODE_ACTION_MOTOR_UP)
      {
        motorStop(ch);
        ap_motor[ch].pre_action = MODE_ACTION_NONE;
        break;
      }

      motorGoUp(ch);
      ap_motor[ch].pre_action = MODE_ACTION_MOTOR_UP;
      break;

    case MODE_ACTION_MOTOR_DOWN:

      if (ap_motor[ch].pre_action == MODE_ACTION_MOTOR_DOWN)
      {
        motorStop(ch);
        ap_motor[ch].pre_action = MODE_ACTION_NONE;
        break;
      }

      motorGoDown(ch);
      ap_motor[ch].pre_action = MODE_ACTION_MOTOR_DOWN;
      break;

    case MODE_ACTION_MOTOR_STOP:

      if (ap_motor[ch].pre_action == MODE_ACTION_MOTOR_STOP)
      {
        ap_motor[ch].pre_action = MODE_ACTION_NONE;
        break;
      }

      motorStop(ch);
      ap_motor[ch].pre_action = MODE_ACTION_MOTOR_STOP;
      break;

    case MODE_ACTION_MOTOR_LOCK:

      ap_motor[ch].is_locked = !ap_motor[ch].is_locked;
      motorStop(ch);
      ap_motor[ch].pre_action = MODE_ACTION_NONE;
      break;
  }
}