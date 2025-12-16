#include "ap_remote.h"

#define REMOTE_BTN_A      0x01
#define REMOTE_BTN_B      0x02
#define REMOTE_BTN_C      0x04
#define REMOTE_BTN_D      0x08

#define MIN_SAMPLE_NUM    20 // 샘플 최소수를 여기서 결정하는게 맞을지?? @@

#define NO_SIGNAL_TIMEOUT 80

static void remoteResetAll(uint8_t ch);
static void remoteHandleRfData(uint8_t ch);
static void remoteHandleTimeout(uint8_t ch);
static void remoteDecodeButton(uint8_t ch, uint32_t raw_data);

typedef enum
{
  REMOTE_STATE_IDLE,
  REMOTE_STATE_RECEIVING,
} remote_state_t;

typedef struct
{
  remote_state_t state;
  uint8_t        count;
  uint32_t       ref_data;
  uint32_t       received_time;
  remote_event_t remote_event;
} ap_remote_t;

static remote_policy_t policy;

ap_remote_t ap_remote[RF_MAX_CH];

bool remoteInit(void)
{
  bool ret = true;
  
  policy = REMOTE_POLICY_NORMAL;

  for (int i=0; i<RF_MAX_CH; i++)
  {
    ap_remote[i].count          = 0;
    ap_remote[i].ref_data       = 0;
    ap_remote[i].received_time  = 0;
    ap_remote[i].state          = REMOTE_STATE_IDLE;
    ap_remote[i].remote_event   = REMOTE_EVENT_NONE;
  }

  return ret;
}

/**
 * @note task를 두개로 분리한 이유 -> 논리적 원인이 있음 : 리모컨 버튼에서 손을 뗀 순간부터 검증검사를 하는 방식 -> normal mode
 *                                                      리모컨 버튼에서 손을 떼지 않고 연속적으로 검증 검사를 하는 방식 -> learn mode
 *                                                      실제 timeout을 체크하는 remoteHandleTimeout() 함수는 실질적으로 내부 상태머신에 의해 
 *                                                      리모컨 버튼을 눌렀다가 뗀 상태여야 발동하기 때문에,
 *                                                      실제 hw와의 동기화를 위해 두 task로 분리하였음.
 */
void remoteProcess(void)
{
  rfTask();
  
  for (int i=0; i<RF_MAX_CH; i++)
  {
    switch (rfGetEvent(i))
    {
      case RF_HW_EVENT_RECIEVED:
        remoteHandleRfData(i); 

        if (policy == REMOTE_POLICY_LEARN)
        {
          if (remoteGetCount(i) > MIN_SAMPLE_NUM)
          {
            ap_remote[i].remote_event = REMOTE_EVENT_SAMPLES_VALIDATED;
            policy = REMOTE_POLICY_NORMAL;
          }
        }
        break;

      case RF_HW_EVENT_NONE:
        if (policy == REMOTE_POLICY_NORMAL)
        {
          remoteHandleTimeout(i); 
        }
        break;
    }
  }
}

/**
 * @brief 실질적으로 데이터를 받아 비교하여 count를 제어하는 함수, 별도의 내부 상태머신으로 관리
 */
static void remoteHandleRfData(uint8_t ch)
{
  uint32_t rx_code = rfGetData(ch);

  ap_remote[ch].received_time = millis();

  switch (ap_remote[ch].state)
  {
    case REMOTE_STATE_IDLE: // 첫번째 데이터 수신
      ap_remote[ch].ref_data = rx_code;
      ap_remote[ch].count = 1;
      ap_remote[ch].state = REMOTE_STATE_RECEIVING;
      break;

    case REMOTE_STATE_RECEIVING:
      if (rx_code == ap_remote[ch].ref_data) // 이전 데이터와 비교
      {
        ap_remote[ch].count++;
      }
      else // 첫번째 데이터와는 다른 데이터 수신 -> 에러 발생, 별도의 핸들링은 어떻게? @@ 고민 POINT
      {
        ap_remote[ch].count        = 0;
        ap_remote[ch].ref_data     = 0;
        ap_remote[ch].state        = REMOTE_STATE_IDLE;
        ap_remote[ch].remote_event = REMOTE_EVENT_SAMPLES_INVALIDATED;

        if (policy == REMOTE_POLICY_LEARN)
        {
          policy = REMOTE_POLICY_NORMAL;
        }
      }
      break;
  }
}

static void remoteHandleTimeout(uint8_t ch)
{
  if (ap_remote[ch].state == REMOTE_STATE_RECEIVING) 
  {
    if (millis() - ap_remote[ch].received_time > NO_SIGNAL_TIMEOUT)
    {
      remoteDecodeButton(ch, remoteGetData(ch));

      ap_remote[ch].count        = 0;  
      ap_remote[ch].state        = REMOTE_STATE_IDLE;
    }
  }
}

static void remoteDecodeButton(uint8_t ch, uint32_t raw_data)
{
  uint8_t button = raw_data & 0x0F; // (리모컨의)16진수에서 첫번째 자리데이터 = 버튼데이터, 나머지는 주소데이터

  switch (button)
  {
    case REMOTE_BTN_A:
      ap_remote[ch].remote_event = REMOTE_EVENT_BUTTON_A;
      break;

    case REMOTE_BTN_B:
      ap_remote[ch].remote_event = REMOTE_EVENT_BUTTON_B;
      break;

    case REMOTE_BTN_C:
      ap_remote[ch].remote_event = REMOTE_EVENT_BUTTON_C;
      break;

    case REMOTE_BTN_D:
      ap_remote[ch].remote_event = REMOTE_EVENT_BUTTON_D;
      break;

    default:
      ap_remote[ch].remote_event = REMOTE_EVENT_BUTTON_UNKNOWN;
      break;
  }
}

uint8_t remoteGetCount(uint8_t ch)
{
  if (ch >= RF_MAX_CH)
  {
    return 0;
  }

  return ap_remote[ch].count;
}


uint32_t remoteGetData(uint8_t ch)
{
  if (ch >= RF_MAX_CH)
  {
    return 0;
  }

  return ap_remote[ch].ref_data;
}

remote_event_t remoteGetEvent(uint8_t ch)
{
  if (ch >= RF_MAX_CH)
  {
    return REMOTE_EVENT_NONE;
  }

  remote_event_t remote_event = ap_remote[ch].remote_event;
  ap_remote[ch].remote_event = REMOTE_EVENT_NONE;
  return remote_event;
}

/**
 * 
 * @brief ap_mode만 해당 함수를 호출해야함
 */
void remoteNotifyMode(uint8_t ch, remote_policy_t next_policy)
{
  ap_remote[ch].count = 0; 
  policy = next_policy;
}