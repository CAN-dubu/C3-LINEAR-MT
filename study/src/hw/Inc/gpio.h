#ifndef GPIO_H_
#define GPIO_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "../hw_def.h"

typedef enum
{
  GPIO_STATE_LOW,
  GPIO_STATE_HIGH
} gpio_pin_state_t;

typedef struct
{
  uint8_t pin_num;
  gpio_mode_t pin_mode;
  gpio_pin_state_t on_state;
  gpio_pin_state_t off_state;
  const char   *p_name;
} gpio_pin_t;

#ifdef _USE_HW_GPIO

#define GPIO_MAX_CH     HW_GPIO_MAX_CH

#endif

#ifdef __cplusplus
}
#endif

#endif