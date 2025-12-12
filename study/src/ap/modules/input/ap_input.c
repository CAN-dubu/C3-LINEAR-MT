#include "ap_input.h"

/**
 * @note ap단에서 buttonPressedTime함수를 hw보다 깔끔하게 구현하는법이 아직 떠오르지않음.. 
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

