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
static void modeChangeState(uint8_t new_state);
static void forceModeChange(uint8_t new_state);
static void modeHandleInput(void);
static void modeHandleRemote(void);

typedef enum
{
  MODE_NORMAL,
  MODE_LEARN,
  MODE_DELETE,
} ap_mode_state_t;

typedef enum
{
  MODE_ACTION_NONE,
  MODE_ACTION_MOTOR_UP,
  MODE_ACTION_MOTOR_DOWN,
  MODE_ACTION_MOTOR_STOP,
  MODE_ACTION_MOTOR_LOCK,
} ap_mode_action_t; //실제로 이 상태값을 모터ap단에서 받아 해석하게끔, motor up -> motor up 이면 stop은 ap motor단에서 구현해야함.

typedef struct
{
  ap_mode_state_t    state;
  ap_mode_state_t    next_state;
  ap_mode_action_t   motor_action;
  uint32_t           new_state_entered_time; 
  bool               is_learn_ended; 
} ap_mode_t;

typedef struct
{
  uint32_t           recieved_remote_code;
  uint32_t           confirmed_vaild_time;
  bool               is_valid_data;
} ap_mode_data_t;


static ap_mode_t ap_mode;
static ap_mode_data_t ap_mode_data = {0,};

bool modeInit(void)
{
  bool ret = true;
  ap_mode.new_state_entered_time = millis();
  ap_mode.is_learn_ended         = false;
  ap_mode.state        = MODE_NORMAL;
  ap_mode.next_state   = MODE_NORMAL;
  ap_mode.motor_action = MODE_ACTION_NONE;

  return ret;
}

void modeProcess(void)
{
  ap_mode.next_state = ap_mode.state; // 명시적으로 초기화 해주지 않으면 이벤트가 일어나지 않아도 계속 next_mode로 상태전이하게 된다.

  modeHandleInput();
  modeHandleRemote();

  modeChangeState(ap_mode.next_state); // 실제 핸들러에게 변화를 받아, state를 변화시키는 부분

  uint32_t timeout = millis() - ap_mode.new_state_entered_time;
  
  switch (ap_mode.state)
  {
    case MODE_NORMAL:
      normalModeRun();
      break;

    case MODE_LEARN:
      learnModeRun();
      if (timeout > 10000 || ap_mode.is_learn_ended)
      {
        ap_mode.is_learn_ended = false;
        forceModeChange(MODE_NORMAL); // 강제 state변화의 의미로 쓰임. (예외상황 혹은 강제성이 필요할때 사용)
      }
      break;

    case MODE_DELETE:
      deleteModeRun();
      if (timeout > 3000)
      {
        forceModeChange(MODE_NORMAL);
      }
      break;
  }
}

static void normalModeRun(void)
{
  allLedOff();

  switch (ap_mode.motor_action)
  {
    case MODE_ACTION_MOTOR_UP:
      printf("motor up\n");
      motorGoUp(MOTOR_CH1); // 임시test: hw_motor로 테스트중..
      // ap motor단에 전달할 함수 ,apMotorRequestAction()?? 이름고민..
      break;
    case MODE_ACTION_MOTOR_DOWN:
      printf("motor down\n");
      motorGoDown(MOTOR_CH1);
      // ap motor단에 전달할 함수
      break;
    case MODE_ACTION_MOTOR_STOP:
      // ap motor단에 전달할 함수
      break;
    case MODE_ACTION_MOTOR_LOCK:
      // ap motor단에 전달할 함수
      break;

    default:
      break;
  }

  ap_mode.motor_action = MODE_ACTION_NONE; // 이 줄을 추가 해주지 않으면 영원히 motor가 움직이게 된다.
}

static void learnModeRun(void)
{
  if(ap_mode_data.is_valid_data)
  {
    allLedTogglePin(100);
    
    uint32_t timeout = millis() - ap_mode_data.confirmed_vaild_time;

    // flash 저장 (가지고 있는 데이터로)
  
    if (timeout > 3000)
    {
      ap_mode_data.recieved_remote_code = 0;
      ap_mode_data.is_valid_data = false;
      ap_mode.is_learn_ended = true;
      ap_mode.next_state = MODE_NORMAL;
    }
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

static void modeChangeState(uint8_t new_state)
{
  if (ap_mode.state == new_state)
  {
    return;
  }

  ap_mode.state = new_state;
  ap_mode.new_state_entered_time = millis();

  switch (ap_mode.state) // 명시적으로 remote에게 현재 모드상태를 알린다. -> 리모트는 두가지 동작을 상태에따라 변화하면서 실행한다.
  {
    case MODE_LEARN:
      remoteNotifyMode(RF_CH1, REMOTE_POLICY_LEARN);
      break;

    case MODE_NORMAL:
      remoteNotifyMode(RF_CH1, REMOTE_POLICY_NORMAL);
      break;
  }
}

static void forceModeChange(uint8_t new_state)
{
  /**
   * @note:
   * 해당 함수는 오직 예외처리용으로 만들어진 함수이다.
   * 예외에서 모드변경이 직접 필요한 경우를 제외하고는 절대 쓰지 말아야 한다.
   */
  modeChangeState(new_state);
}

/**
 * @brief inputEvent를 핸들링하는 함수
 */
static void modeHandleInput(void)
{
  input_event_t first_input_event  = inputGetEvent(BTN_CH1);
  input_event_t second_input_event = inputGetEvent(BTN_CH2);
  
  switch (ap_mode.state)
  {
    case MODE_NORMAL:
      if (first_input_event == INPUT_EVENT_PRESSED_LONG && second_input_event == INPUT_EVENT_PRESSED_LONG)
      {
        ap_mode.next_state = MODE_DELETE;
      }
      else if (first_input_event == INPUT_EVENT_PRESSED_MIDDLE && second_input_event == INPUT_EVENT_PRESSED_MIDDLE)
      {
        ap_mode.next_state = MODE_LEARN;
      }
      else if (first_input_event == INPUT_EVENT_PRESSED_SHORT && second_input_event == INPUT_EVENT_NONE)
      {
        ap_mode.motor_action = MODE_ACTION_MOTOR_UP;
      }
      else if (first_input_event == INPUT_EVENT_NONE && second_input_event == INPUT_EVENT_PRESSED_SHORT)
      {
        ap_mode.motor_action = MODE_ACTION_MOTOR_DOWN;
      }
      break; 
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
        
        switch (remote_event) // @@ 이제 여기서 저장됀 플래쉬속 리모컨 주소와 매칭시켜야함.. 맞으면 아래 내용 실행하는걸로
        {
          case REMOTE_EVENT_BUTTON_A:
            ap_mode.motor_action = MODE_ACTION_MOTOR_UP;
            break;

          case REMOTE_EVENT_BUTTON_B:
            ap_mode.motor_action = MODE_ACTION_MOTOR_DOWN;
            break;

          case REMOTE_EVENT_BUTTON_C:
            // printf("motor stop\n");
            break;

          case REMOTE_EVENT_BUTTON_D:
            // printf("motor lock\n");
            break;

          case REMOTE_EVENT_BUTTON_UNKNOWN: 
            printf("unKown remote Controller\n");
            break;
        }
      break;
    
    case MODE_LEARN:  
        switch (remote_event)
        {
          case REMOTE_EVENT_SAMPLES_INVALIDATED:
            printf("Sampling Failed\n");
            ap_mode.next_state = MODE_NORMAL;
            break;

          case REMOTE_EVENT_SAMPLES_VALIDATED:
            ap_mode_data.recieved_remote_code = remoteGetData(RF_CH1);
            ap_mode_data.is_valid_data = true;
            ap_mode_data.confirmed_vaild_time = millis();
            printf("valid data inputs\n");
        }
      break;
  }
}