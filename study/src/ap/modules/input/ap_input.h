#ifndef AP_INPUT_H_
#define AP_INPUT_H_

#include "../../ap_def.h"

typedef enum
{
  PRESSED_SHORT,
  PRESSED_MIDDLE,
  PRESSED_LONG
} input_event_t;

void inputProcess(void);

void inputGetEvent(void);
#endif