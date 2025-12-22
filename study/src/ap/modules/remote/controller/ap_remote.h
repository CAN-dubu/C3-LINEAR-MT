#ifndef AP_MODULE_REMOTE_H_
#define AP_MODULE_REMOTE_H_

#include "../../../ap_def.h"
#include "../common/rf_def.h"
#include "../storage/remote_storage.h"

typedef enum  // 연속적 데이터 (20개 이상을 검사할때) 와 3~4개 정도의 짧은 데이터를 구별
{
  REMOTE_POLICY_NORMAL,
  REMOTE_POLICY_LEARN,
} remote_policy_t;

typedef enum
{
  REMOTE_EVENT_NONE,
  REMOTE_EVENT_BUTTON_A,
  REMOTE_EVENT_BUTTON_B,
  REMOTE_EVENT_BUTTON_C,
  REMOTE_EVENT_BUTTON_D,
  REMOTE_EVENT_FLASH_ERROR,
  REMOTE_EVENT_NO_DATA_IN_FLASH,
  REMOTE_EVENT_SAMPLES_INVALIDATED,
  REMOTE_EVENT_SAMPLES_STORED_IN_FLASH,
  REMOTE_EVENT_BUTTON_UNKNOWN,
} remote_event_t;

bool remoteInit(void);
void remoteProcess(void);
uint8_t remoteGetCount(uint8_t ch);
uint32_t remoteGetData(uint8_t ch);
remote_event_t remoteGetEvent(uint8_t ch);
void remoteNotifyMode(uint8_t ch, remote_policy_t next_policy);

#endif