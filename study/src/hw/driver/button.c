#include "../Inc/button.h"

#ifdef _USE_HW_BUTTON

#include "../Inc/gpio.h"

#define NAME_DEF(x)  x, #x

#define BTN1_PIN 0
#define BTN2_PIN 4

typedef struct
{
  uint8_t pin;
  uint8_t pin_mode;
  gpio_pin_state_t on_state;
  ButtonPinName_t pin_name;
  const char *p_name;
} button_hw_t;

typedef struct
{
  Button_debounce_t  state;
  bool               pressed;
  uint16_t           pressed_cnt;
  uint32_t           pre_time;
  uint32_t           pressed_start_time;
  uint32_t           pressed_end_time;
  ButtonEvent_t      pressed_event;
  const button_hw_t *p_hw;
} button_t;

static const button_hw_t button_hw[BUTTON_MAX_CH] =
  {
    {BTN1_PIN, INPUT_PULLUP, GPIO_STATE_LOW, NAME_DEF(BTN_CH1)},
    {BTN2_PIN, INPUT_PULLUP, GPIO_STATE_LOW, NAME_DEF(BTN_CH2)},
  };

static button_t button_tbl[BUTTON_MAX_CH];

bool buttonInit(void)
{
  bool ret = true;
  
  for(int i=0; i<BUTTON_MAX_CH; i++)
  {
    pinMode(button_hw[i].pin, button_hw[i].pin_mode);
  }

  for(int i=0; i<BUTTON_MAX_CH; i++)
  {
    button_tbl[i].p_hw         =  &button_hw[i];
    button_tbl[i].state        =  0;
    button_tbl[i].pressed      =  false;
    button_tbl[i].pressed_cnt  =  0;
    button_tbl[i].pre_time     =  0;
  }
  return ret;
}

void buttonTask(void)
{
  static bool is_3000ms_locked  = true;
  static bool is_10000ms_locked = true;

  for (int i=0; i<BUTTON_MAX_CH; i++)
  {
    button_t *p_btn = &button_tbl[i];

    switch (p_btn->state)
    {
      case BTN_IDLE: 
        if (buttonGetPin(i) == true)
        {
          p_btn->pre_time = millis();
          p_btn->state = BTN_DEBOUNCE_PRESSED;
        }
        else
        {
          p_btn->pre_time = millis();
          p_btn->state = BTN_DEBOUNCE_RELEASED;
        }
        break;
    
      case BTN_DEBOUNCE_PRESSED:
        if (millis() - p_btn->pre_time >= 20)
        {
          p_btn->pressed = true;
          p_btn->pressed_cnt++;
          p_btn->pressed_start_time = millis();
          p_btn->state = BTN_PRESSED;
        }
        else
        {
          if (buttonGetPin(i) == false)
          {
            p_btn->state = BTN_IDLE;
          }
        }
        break;

      case BTN_DEBOUNCE_RELEASED:
        if (millis() - p_btn->pre_time >= 20)
        {
          p_btn->pressed = false;
          p_btn->state = BTN_RELEASED;

          // if (buttonGetPressedTime(i) < 3000)
          // {
          //   p_btn->pressed_event = BUTTON_EVENT_SHORT;
          // }
          buttonResetTime(i);
        }
        else
        {
          if (buttonGetPin(i) == true)
          {
            p_btn->state = BTN_IDLE;
          } 
        }
        break;

      case BTN_PRESSED:
        p_btn->pressed_end_time = millis();

        // if (buttonGetPressedTime(i) >= 10000)
        // {
        //   p_btn->pressed_event = BUTTON_EVENT_LONG_10S;
        // }
        // else if (buttonGetPressedTime(i) >= 3000)
        // {
        //   p_btn->pressed_event = BUTTON_EVENT_LONG_3S;
        // }

        if (buttonGetPin(i) == false)
        {
          // buttonResetTime();
          p_btn->state = BTN_IDLE;
        }
        break;

      case BTN_RELEASED:

        if (buttonGetPin(i) == true)
        {
            p_btn->state = BTN_IDLE;
        }
        break;
    }
  }
}

bool buttonGetPin(uint8_t ch)
{
  bool ret = false;

  if (ch >= BUTTON_MAX_CH)
  {
    return false;
  }

  if (digitalRead(button_hw[ch].pin) == button_hw[ch].on_state)
  {
    ret = true;
  }

  return ret;
}

bool buttonGetPressed(uint8_t ch)
{
  if (ch >= BUTTON_MAX_CH)
  {
    return false;
  }

  return button_tbl[ch].pressed;
}


// 모든 버튼상태 비트값으로 저장
uint32_t buttonGetData(void)
{
  uint32_t ret = 0;

  for (int i=0; i<BUTTON_MAX_CH; i++)
  {
    ret |= (buttonGetPressed(i)<<i);
  }

  return ret;
}

uint32_t buttonGetPressedTime(uint8_t ch)
{
  volatile uint32_t ret;

  ret = button_tbl[ch].pressed_end_time - button_tbl[ch].pressed_start_time;
  return ret;
}

void buttonResetTime(uint8_t ch)
{
  button_tbl[ch].pressed_start_time    = 0;
  button_tbl[ch].pressed_end_time      = 0;
}

ButtonEvent_t buttonGetEvent(uint8_t ch)
{
  ButtonEvent_t button_event = button_tbl[ch].pressed_event;
  button_tbl[ch].pressed_event = BUTTON_EVENT_NONE; // 읽으면 초기화
  return button_event;
}

#endif

//**
// @question 여기서 테이블 정보를 다른 파일로 어떻게 전달할지?? 일단 -> getter식으로..
//*/