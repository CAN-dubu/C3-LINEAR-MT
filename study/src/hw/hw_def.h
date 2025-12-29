#ifndef HW_DEF_H_
#define HW_DEF_H_

#include "../common/def.h"

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         GPIO_PIN_MAX

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       BUTTON_PIN_MAX

#define _USE_HW_LED
#define      HW_LED_MAX_CH          2

#define _USE_HW_RF
#define      HW_RF_MAX_CH           RF_PIN_MAX

#define _USE_HW_MOTOR
#define      HW_MOTOR_MAX_CH        MOTOR_PIN_MAX

#define _USE_HW_SENSOR
#define      HW_SENSOR_MAX_CH       SENSOR_PIN_MAX

/**
 * @brief 사용하고자 하는 기능 채널의 수는 이곳에서 조정
 */

typedef enum
{
  PIN_NUM1,
  GPIO_PIN_MAX,
} GpioPinName_t;

typedef enum
{
  BTN_CH1,
  BTN_CH2,
  BUTTON_PIN_MAX,
} ButtonPinName_t;

typedef enum
{
  RF_CH1,
  RF_PIN_MAX
} RfPinName_t;

typedef enum
{
  MOTOR_CH1,
  MOTOR_PIN_MAX,
} MotorPinName_t;

typedef enum
{
  SENSOR_CH1,
  SENSOR_PIN_MAX,
} SensorPinName_t;

#endif