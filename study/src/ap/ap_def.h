#ifndef AP_DEF_H_
#define AP_DEF_H_


typedef enum
{
  MODE_ACTION_NONE,
  MODE_ACTION_MOTOR_UP,
  MODE_ACTION_MOTOR_DOWN,
  MODE_ACTION_MOTOR_STOP,
  MODE_ACTION_MOTOR_LOCK,
} ap_motor_action_t;


#include "../hw/hw.h"
#include "../common/def.h"
#include "modules/mode/ap_mode.h"
#include "modules/motor/ap_motor.h"
#include "modules/input/ap_input.h"
#include "modules/remote/controller/ap_remote.h"
#include "modules/sensor/current_sensor.h"

// #include "modules/mode/ap_sensor.h"

#define _USE_AP_PREFERENCE

/**
 * @brief 물리 버튼을 누르는 기준시간 상수
 */
#define BTN_PRESSED_TIME_MIDDLE    3000
#define BTN_PRESSED_TIME_LONG      10000

/**
 * @brief 모터 끝점 도달시, 과전류 방지를 위해 자동으로 모터를 turn off시켜줄 주기시간
 */
#define MOTOR_OVERCURRENT_DELAY_MS 3000

#endif