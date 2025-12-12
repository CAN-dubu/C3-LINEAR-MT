#ifndef BUTTON_H_
#define BUTTON_H_

#include "../hw_def.h"

#define BUTTON_MAX_CH       HW_BUTTON_MAX_CH

/**
 * @brief 버튼의 채터링을 잡기위한 state, event 종류를 이곳에 정의함.
 */
typedef enum
{
  BTN_IDLE,
  BTN_DEBOUNCE_PRESSED,
  BTN_DEBOUNCE_RELEASED,
  BTN_PRESSED,    
  BTN_RELEASED,    
} Button_debounce_t;

typedef enum 
{
  BUTTON_EVENT_NONE,
  BUTTON_EVENT_SHORT,
  BUTTON_EVENT_LONG_3S,
  BUTTON_EVENT_LONG_10S,
} ButtonEvent_t;

bool buttonInit(void);
void buttonTask(void);
bool buttonGetPin(uint8_t ch);
bool buttonGetPressed(uint8_t ch);
uint32_t buttonGetData(void);
uint32_t buttonGetPressedTime(uint8_t ch);
void buttonResetTime(uint8_t ch);
ButtonEvent_t buttonGetEvent(uint8_t ch);

#endif