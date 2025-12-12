#ifndef RF_H
#define RF_H

#include "../hw_def.h"

#define RF_MAX_CH       HW_RF_MAX_CH

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum 
{
  RF_EVENT_NONE,
  RF_EVENT_RECIEVED,
} RF_Event_t;

typedef enum
{
  RF_IDLE,
  RF_SIGNAL_DETECTED,
  RF_VALIDATED
} RF_mode_t;

bool rfInit(void);
void rfTask(void);
uint32_t rfGetData(uint8_t ch);
RF_Event_t rfGetEvent(uint8_t ch);

#ifdef __cplusplus
}
#endif

#endif