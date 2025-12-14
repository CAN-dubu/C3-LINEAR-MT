#ifndef BUTTON_H_
#define BUTTON_H_

#include "../hw_def.h"

#define BUTTON_MAX_CH       HW_BUTTON_MAX_CH

/**
 * @brief event 종류
 */
typedef enum
{
  BTN_HW_EVENT_NONE,
  BTN_HW_EVENT_PRESSED,
  BTN_HW_EVENT_KEEP_PRESSING,
  BTN_HW_EVENT_RELEASED,
} Button_hw_event_t;

bool buttonInit(void);
void buttonTask(void);
bool buttonGetPin(uint8_t ch);
bool buttonGetPressed(uint8_t ch);
uint32_t buttonGetData(void);
void buttonResetTime(uint8_t ch);
Button_hw_event_t buttonGetEvent(uint8_t ch);

#endif