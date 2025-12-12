#include "ap_input.h"

/**
 * @note ap단에서 buttonPressedTime함수를 hw보다 깔끔하게 구현하는법이 아직 떠오르지않음.. 

on BTN_EVENT_PRESSED:
  pressed_start = now;

on BTN_EVENT_RELEASED:
  duration = now - pressed_start;
 두상태에대항 hw로부터의 이벤트만 받고 이곳에서 pressedtime을
측정해야할지 고민중

 * @@ <<해결 해야할 문제 표시
*/

void inputProcess(void)
{
  // 이곳에서 pressedTime을 계산하고, 어떠한 이벤트를 발생시켜야함.
  // short event의 경우, 
  static bool pressed_event_occured  = false;

  buttonTask();

  if(buttonGetPressed(BTN_CH1) && buttonGetPressed(BTN_CH2) && !pressed_event_occured)
  {
    uint32_t pressed_time = millis();
  }
  else
}

void inputGetEvent(void)
{

}

