#ifndef AP_MODULE_MODE_H_
#define AP_MODULE_MODE_H_

typedef enum
{
  MODE_ACTION_NONE,
  MODE_ACTION_MOTOR_UP,
  MODE_ACTION_MOTOR_DOWN,
  MODE_ACTION_MOTOR_STOP,
  MODE_ACTION_MOTOR_LOCK,
} ap_mode_action_t;

typedef enum
{
  MODE_NORMAL = 0,
  MODE_REMOTE_LEARN,
  MODE_REMOTE_DELETE,
} ap_mode_state_t;

#include "../../ap_def.h"

bool modeInit(void);
void modeProcess(void);

#endif