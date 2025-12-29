#ifndef AP_MODULE_MOTOR_H_
#define AP_MODULE_MOTOR_H_

#include "../../ap_def.h"

bool apMotorInit(void);
void motorRequestAction(uint8_t ch, ap_motor_action_t action);

#endif