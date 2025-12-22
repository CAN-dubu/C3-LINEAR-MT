#include "ap_mode.h"

/**
 * 
 * @brief 일반모드, learn모드 구분은 어떤기준? -> 버튼기준이다. 
 *        3초, 10초 기준으로 모드가 바뀐다.
 * 
 */

static void normalModeRun(void);
static void learnModeRun(void);
static void deleteModeRun(void);
static void modeChangeState(ap_mode_state_t new_state);
static void modeHandleTimeIssue(void);
static void modeHandleInput(void);
static void modeHandleRemote(void);

typedef struct
{
  ap_mode_state_t    state;
  ap_mode_state_t    next_state;
  ap_mode_action_t   motor_action;
  uint32_t           new_state_entered_time;
  bool               is_learn_ended; 
} ap_mode_t;

static ap_mode_t ap_mode;

bool modeInit(void)
{
  bool ret = true;
  ap_mode.new_state_entered_time = millis();
  ap_mode.is_learn_ended         = false;
  ap_mode.state                  = MODE_NORMAL;
  ap_mode.next_state             = MODE_NORMAL;
  ap_mode.motor_action           = MODE_ACTION_NONE;

  return ret;
}

void modeProcess(void)
{
  // 1) 이번 tick의 기본 next_state는 현재 state
  ap_mode.next_state = ap_mode.state;

  // 2) 이벤트 처리: 입력 / 리모컨 / 딜레이와 타임아웃
  modeHandleInput();
  modeHandleRemote();
  modeHandleTimeIssue();

  // 3) 상태 전이 확정
  modeChangeState(ap_mode.next_state);

  // 4) 현재 상태에 따른 동작 수행
  switch (ap_mode.state)
  {
    case MODE_NORMAL:
      normalModeRun();
      break;

    case MODE_REMOTE_LEARN:
      learnModeRun();
      break;

    case MODE_REMOTE_DELETE:
      deleteModeRun();
      break;
  }
}

static void normalModeRun(void)
{
  allLedOff();

  if (ap_mode.motor_action != MODE_ACTION_NONE)
  {
    motorRequestAction(MOTOR_CH1, ap_mode.motor_action);
  }
  ap_mode.motor_action = MODE_ACTION_NONE; // 이 줄을 추가 해주지 않으면 영원히 motor가 움직이게 된다.
}

static void learnModeRun(void)
{
  if(ap_mode.is_learn_ended)
  {
    allLedTogglePin(100);
  }
  else
  {
    allLedTogglePin(500);
  }
}

static void deleteModeRun(void)
{
  allLedTogglePin(100);
}

/**
 * @brief next_state기반으로 현재 state를 변경하는 함수
 */
static void modeChangeState(ap_mode_state_t new_state)
{
  if (ap_mode.state == new_state)
  {
    return;
  }

  ap_mode.state = new_state;
  ap_mode.new_state_entered_time = millis();

  switch (ap_mode.state) // 명시적으로 remote에게 현재 모드상태를 알린다. -> 리모트는 두가지 동작을 상태에따라 변화하면서 실행한다. (짧은 데이터를 검증할지, 긴데이터를 연속으로 검증할지)
  {
    case MODE_REMOTE_LEARN:
      remoteNotifyMode(RF_CH1, REMOTE_POLICY_LEARN);
      break;

    case MODE_NORMAL:
      remoteNotifyMode(RF_CH1, REMOTE_POLICY_NORMAL);
      break;
  }
}

static void modeHandleTimeIssue(void)
{
  uint32_t delay = millis() - ap_mode.new_state_entered_time;

  switch (ap_mode.state)
  {
    case MODE_REMOTE_LEARN:
      if (ap_mode.is_learn_ended)
      {
        if (delay > 3000)
        {
          printf("succeed to learn\n");
          ap_mode.is_learn_ended = false;
          ap_mode.next_state = MODE_NORMAL;
        }
      }
      else if (delay > 5000) // 10초 -> 5초 딜레이
      {
        printf("learning failed by timeout issue \n");
        ap_mode.next_state = MODE_NORMAL;
      }
      break;
      
    case MODE_REMOTE_DELETE:
      if (delay > 3000)
      {
        printf("all deleted\n");
        remoteStorageDeleteAll();
        ap_mode.next_state = MODE_NORMAL;
      }
      break;
  }
}

/**
 * @brief inputEvent를 핸들링하는 함수
 */
static void modeHandleInput(void)
{
  input_event_t first_input_event  = inputGetEvent(BTN_CH1);
  input_event_t second_input_event = inputGetEvent(BTN_CH2);
  
  if (ap_mode.state != MODE_NORMAL)
  {
    return;
  }
    
  if (first_input_event == INPUT_EVENT_PRESSED_LONG && second_input_event == INPUT_EVENT_PRESSED_LONG)
  {
    ap_mode.next_state = MODE_REMOTE_DELETE;
  }
  else if (first_input_event == INPUT_EVENT_PRESSED_MIDDLE && second_input_event == INPUT_EVENT_PRESSED_MIDDLE)
  {
    ap_mode.next_state = MODE_REMOTE_LEARN;
  }
  else if (first_input_event == INPUT_EVENT_PRESSED_SHORT && second_input_event == INPUT_EVENT_NONE)
  {
    ap_mode.motor_action = MODE_ACTION_MOTOR_UP;
  }
  else if (first_input_event == INPUT_EVENT_NONE && second_input_event == INPUT_EVENT_PRESSED_SHORT)
  {
    ap_mode.motor_action = MODE_ACTION_MOTOR_DOWN;
  }
}

static void modeHandleRemote(void)
{
  // 모드 state에 따라 제어가 달라져야함. learn모드일시 별도의 제어로직 필요
  // normal이면 모터제어 여기서

  remote_event_t remote_event = remoteGetEvent(RF_CH1);

  switch (ap_mode.state)
  {
    case MODE_NORMAL:
        
        switch (remote_event) 
        {
          case REMOTE_EVENT_BUTTON_A:
            ap_mode.motor_action = MODE_ACTION_MOTOR_UP;
            break;

          case REMOTE_EVENT_BUTTON_B:
            ap_mode.motor_action = MODE_ACTION_MOTOR_DOWN;
            break;

          case REMOTE_EVENT_BUTTON_C:
            ap_mode.motor_action = MODE_ACTION_MOTOR_STOP;
            break;

          case REMOTE_EVENT_BUTTON_D:
            ap_mode.motor_action = MODE_ACTION_MOTOR_LOCK;
            break;

          case REMOTE_EVENT_NO_DATA_IN_FLASH:
            printf("need to learn this remote_controller\n");
            break;

          case REMOTE_EVENT_BUTTON_UNKNOWN: 
            printf("unKown remote Controller\n");
            break;
        }
      break;
    
    case MODE_REMOTE_LEARN:  // ap_remote단에서 flash저장까지 마치고 온다. 그리고 flash 저장에 실패했을때를 대비하여 여러가지 에러 처리를 여기에 구현하는것도 나쁘지 않아보임.
        switch (remote_event)
        {
          case REMOTE_EVENT_SAMPLES_INVALIDATED:
            printf("Sampling Failed\n");
            ap_mode.next_state = MODE_NORMAL;
            break;

          case REMOTE_EVENT_SAMPLES_STORED_IN_FLASH:
            ap_mode.is_learn_ended = true;
            break;

          case REMOTE_EVENT_FLASH_ERROR:
            printf("flash error\n");
            break;
        }
      break;
  }
}