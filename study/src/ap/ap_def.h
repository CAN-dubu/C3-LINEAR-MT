#ifndef AP_DEF_H_
#define AP_DEF_H_

#include "../hw/hw.h"
#include "../common/def.h"
#include "modules/mode/ap_mode.h"
#include "modules/motor/ap_motor.h"
#include "modules/input/ap_input.h"
#include "modules/remote/controller/ap_remote.h"

// #include "modules/mode/ap_sensor.h"

#define _USE_AP_PREFERENCE

/**
 * @brief 물리 버튼을 누르는 기준시간 상수
 */
#define BTN_PRESSED_TIME_MIDDLE 3000
#define BTN_PRESSED_TIME_LONG   10000

#endif