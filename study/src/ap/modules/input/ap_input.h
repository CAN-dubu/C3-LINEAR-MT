#ifndef AP_MODULE_INPUT_H_
#define AP_MODULE_INPUT_H_

#include "../../ap_def.h"

#define BTN_PRESSED_TIME_MIDDLE 3000
#define BTN_PRESSED_TIME_LONG   10000

typedef enum
{
  INPUT_EVENT_NONE,
  INPUT_EVENT_PRESSED_SHORT,
  INPUT_EVENT_PRESSED_MIDDLE,
  INPUT_EVENT_PRESSED_LONG
} input_event_t;

bool inputInit(void);
void inputProcess(void);
input_event_t inputGetEvent(uint8_t ch);

#endif