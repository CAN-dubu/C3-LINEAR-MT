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
