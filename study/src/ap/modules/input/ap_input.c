#include "ap_input.h"

/**
 * @note 이곳에서는 짧게, 중간, 길게 눌렀는지에 대한 이벤트만을 발생시켜 ap_mode에게 전달함
*/

static void inputUpdatePressedTime(uint8_t ch);

typedef struct
{
  uint32_t      pressed_start_time;
  input_event_t input_pressed_event;
} ap_input_t;

ap_input_t ap_input[BUTTON_PIN_MAX];

bool inputInit(void)
{
  bool ret = true;

  for (int i=0; i<BUTTON_PIN_MAX; i++)
  {
    ap_input[i].pressed_start_time   = 0;
    ap_input[i].input_pressed_event  = INPUT_EVENT_NONE;
  }

  return ret;
}

/**
 * @brief 실제 inputEvent를 발생시키는 프로세스, 
 *        hw_button에서 pressed 혹은 released 이벤트를 받아 pressedTime을 계산하고 
 *        그에 맞는 의미를 지닌 event를 발생시켜 상위 ap에 전달, 
 */
void inputProcess(void)
{
  buttonTask();

  for (int i=0; i<BUTTON_PIN_MAX; i++)
  {
    Button_hw_event_t btn_hw_event = buttonGetEvent(i);

    switch (btn_hw_event)
    {
      case BTN_HW_EVENT_PRESSED:
        ap_input[i].pressed_start_time = millis();
        break;

      case BTN_HW_EVENT_KEEP_PRESSING:
      {
        uint32_t pressed_time = millis() - ap_input[i].pressed_start_time; 

        if (pressed_time >= BTN_PRESSED_TIME_LONG) // PRESSED_LONG 제일 첫번째. 반대가 되면 PRESSED_MIDDLE에 영원히 걸림
        {
          ap_input[i].input_pressed_event = INPUT_EVENT_PRESSED_LONG;
        }
        else if (pressed_time >= BTN_PRESSED_TIME_MIDDLE)
        {
          ap_input[i].input_pressed_event = INPUT_EVENT_PRESSED_MIDDLE;
        }
        break;
      }

      case BTN_HW_EVENT_RELEASED:
      {
        uint32_t pressed_time = millis() - ap_input[i].pressed_start_time;
        
        if (pressed_time < BTN_PRESSED_TIME_MIDDLE)
        {
          ap_input[i].input_pressed_event = INPUT_EVENT_PRESSED_SHORT;
        }
        break;
      }
    }
  }
}


//  두채널 모두 확인해야함
input_event_t inputGetEvent(uint8_t ch)
{
  if (ch >= BUTTON_PIN_MAX)
  {
    return INPUT_EVENT_NONE;
  }

  input_event_t input_event = ap_input[ch].input_pressed_event;
  ap_input[ch].input_pressed_event = INPUT_EVENT_NONE; // 읽으면 초기화
  return input_event;
}