#include "src/hw/hw.h"
#include "src/ap/ap.h"
// #include "src/common/Inc/rf_sample_queue.h"

// typedef enum
// {
//   REMOTE_IDLE,
//   REMOTE_LEARN,
//   REMOTE_CAPTURED,
//   REMOTE_DELETE,
//   REMOTE_MAX
// } remote_state_t;

// typedef enum
// {
//   LEARN_MODE_BEGIN_EVENT,
//   LEARN_MODE_END_EVENT,
  
//   MAIN_EVENT_MAX
// } main_event_t

// typedef struct 
// {
//   remote_state_t mode;
//   uint32_t       learn_mode_enter_time;

// } remote_mode_t;

// remote_mode_t remote_mode = {REMOTE_IDLE, 0};

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  hwInit();   
  apInit();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  apMain();
  // updateEvent();

/**
 *  =======normal, learn구현 후 마지막에 생각 할 부분 ==============
 * 
 *   enum
  {
    NORMAL_MODE,
    LEARN_MODE,
  };

  static uint8_t mode = NORMAL_MODE;
  static uint8_t mode_req = NORMAL_MODE;
  bool is_first = false;

  if (modeEventGet() == LEARN_MODE_BEGIN_EVENT) 
  {
    mode_req = LEARN_MODE;
  }
  if (modeEventGet() == LEARN_MODE_END_EVENT)
  {
    mode_req = NORMAL_MODE;
  }
  modeEventClear();

  if (mode_req != mode)
  {
    mode = mode_req;
    // 로그 출력 가능 
  }

  switch(mode)
  {
    case NORMAL_MODE:
      normalModeRun();
      break;
    
    case LEARN_MODE:
      learnModeRun();
      break;
  }
 * ===========================================
 */
  
}

// void updateState(void)
// {
//   modeUpdate();
// }

// void updateEvent(void)
// {
//   buttonEventHandler();
//   rfEventHandler(RF_CH1);
// }

// void getButtonState(void)
// {
//   uint32_t result = buttonGetData();

//   switch (result)
//   {
//     case 0b11:
//     uint32_t timer = buttonGetPressedTime();
//     Serial.printf("time:%d\n",timer);
//     break;
//   }
// }

// void modeUpdate(void)
// {
//   uint32_t timeout = millis() - remote_mode.learn_mode_enter_time;

//   switch(remote_mode.mode)
//   {
//     case REMOTE_IDLE:
//       // allLedOff();
//       break;

//     case REMOTE_LEARN:
//       // allLedTogglePin(500);
      
//       // if (sampleQueueGet(&remote_mode.rf_rx_code)) // 이 함수가 return으로 total값을 하는게 맞겠음. get() > 20 이런식으로
//       // {
//       //   Serial.printf("Learned: 0x%X\n", remote_mode.rf_rx_code);
//       // }

//       // if (timeout > 10000)
//       // {
//       //   changeRemoteState(REMOTE_IDLE);  
//       // }
//       break;

//     case REMOTE_DELETE:
//       // allLedTogglePin(100);
      
//       // if (timeout > 3000)
//       // {
//       //   changeRemoteState(REMOTE_IDLE);
//       // }
//       break;
//   }
// }

/**
 * @brief main state를 변경하는 함수, 진입 시 modeUpdate() 내에서 사용 할 타이머가 작동한다.
 * @@@ 이름생각!!!!!! 이건 main의 learn모드에서만 작동할예정. 근데 계획 변경 가능!!!
 */
// void changeRemoteState(remote_state_t new_state)
// {
//   if (remote_mode.mode == new_state)
//   {
//     return;
//   }

//   remote_mode.mode = new_state;
//   remote_mode.learn_mode_enter_time = millis();
// }



// /**
//  * @brief RF신호에 대한 이벤트 핸들러, sampleQueueWrite말고 다른거 생각;; sample이름은 너무 general함.
//  */
// void rfEventHandler(uint8_t ch)
// {
//   RF_Event_t rf_event = rfGetEvent(ch);

//   if (rf_event == RF_EVENT_RECIEVED)
//   {
//     // Serial.println("recieved");
//     // remote_mode.rf_event_occured = true;
//     remote_mode.rf_rx_code = rfGetData(ch);
//     rfSampleQueueWrite(remote_mode.rf_rx_code);
//   }
//   // sampling? rf 신호 stoped event를 받으면..
// }
