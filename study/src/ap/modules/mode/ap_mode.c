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
static void modeHandleInput(void);

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
} ap_mode_t;

static ap_mode_t ap_mode;

bool modeInit(void)
{
  bool ret = true;

  ap_mode.state        = MODE_NORMAL;
  ap_mode.next_state   = MODE_NORMAL;
  ap_mode.motor_action = MODE_ACTION_NONE;

  return ret;
}

void modeProcess(void)
{
  modeHandleInput();
  // rf

  modeChangeState(ap_mode.next_state);

  uint32_t timeout = millis() - ap_mode.new_state_entered_time;
  
  switch (ap_mode.state)
  {
    case MODE_NORMAL:
      normalModeRun();
      break;

    case MODE_LEARN:
      learnModeRun();
      if (timeout > 10000)
      {
        modeChangeState(MODE_NORMAL);
      }
      break;

    case MODE_DELETE:
      deleteModeRun();
      if (timeout > 3000)
      {
        modeChangeState(MODE_NORMAL);
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
      // ap motor단에 전달할 함수 ,apMotorRequestAction()?? 이름고민..
      break;
    case MODE_ACTION_MOTOR_DOWN:
      printf("motor down\n");
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
  allLedTogglePin(500);
}

static void deleteModeRun(void)
{
  allLedTogglePin(100);
}

static void modeChangeState(uint8_t new_state)
{
  if (ap_mode.state != new_state)
  {
    ap_mode.state = new_state;
    ap_mode.new_state_entered_time = millis();
  }
}

/**
 * @brief inputEvent를 핸들링하는 함수
 */
static void modeHandleInput(void)
{
  ap_mode.next_state   = ap_mode.state; // 명시적으로 초기화 해주지 않으면 이벤트가 일어나지 않아도 계속 next_mode로 상태전이하게 된다.

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