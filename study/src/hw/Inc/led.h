#ifndef LED_H_
#define LED_H_

#include "../hw_def.h"

#define LED_MAX_CH       HW_LED_MAX_CH

bool ledInit(void);
void ledOn(uint8_t ch);
void ledOff(uint8_t ch);
void allLedOff(void);
void allLedTogglePin(uint32_t intervel_ms);
#endif