#include "../Inc/rf.h"

#ifdef _USE_HW_RF

#include "../Inc/gpio.h"
#include <RCSwitch.h>

#define RF_DAT_PIN 10
#define RF_MIN_LENGTH 24

static bool rfGetDetected(uint8_t ch);
static uint16_t rfGetLength(uint8_t ch);

typedef struct
{
  uint8_t    pin;
  uint8_t    irq_pin;
} rf_hw_t;

typedef struct
{
  uint8_t         state;
  uint32_t        pre_time;
  uint32_t        received_data;
  bool            received_data_is_valid;
  RF_Event_t      received_event;
  RCSwitch       *p_rcswitch;
  const rf_hw_t  *p_hw;
} rf_t;

static rf_hw_t rf_hw[RF_MAX_CH] = 
{
  {RF_DAT_PIN, 0},
};

static rf_t rf_tbl[RF_MAX_CH];

RCSwitch my_switch[RF_MAX_CH];

bool rfInit(void)
{  
  bool ret = true;
  
  for (int i=0; i<RF_MAX_CH; i++)
  {
    rf_hw[i].irq_pin = digitalPinToInterrupt(rf_hw[i].pin);
  }  
  
  for(int i=0; i<RF_MAX_CH; i++)
  {
    rf_tbl[i].p_hw = &rf_hw[i];
    rf_tbl[i].p_rcswitch = &my_switch[i];
    rf_tbl[i].p_rcswitch -> enableReceive(rf_tbl[i].p_hw -> irq_pin); 
    rf_tbl[i].received_data_is_valid = false;
    rf_tbl[i].state = RF_IDLE;
  }

  return ret;
}

void rfTask(void)
{
  for(int i=0; i<RF_MAX_CH; i++)
  {
    rf_t *p_rf = &rf_tbl[i];

    switch (p_rf->state)
    {
      case RF_IDLE:
        if (rfGetDetected(i))
        {
          p_rf->pre_time = millis();
          p_rf->state = RF_SIGNAL_DETECTED;
        }
        break;

      case RF_SIGNAL_DETECTED:
    if (millis() - p_rf->pre_time >= 50)
    {
        p_rf->received_data = p_rf->p_rcswitch->getReceivedValue();
        p_rf->received_data_is_valid = rfGetLength(i) >= RF_MIN_LENGTH;

        p_rf->p_rcswitch->resetAvailable();
        p_rf->state = RF_VALIDATED;
    }
    break;

      case RF_VALIDATED:
        if (p_rf->received_data_is_valid)
        {        
          rf_tbl[i].received_event = RF_HW_EVENT_RECIEVED;
        }
        p_rf->state = RF_IDLE;
        break;
        
    }
  }
}

static bool rfGetDetected(uint8_t ch)
{
  bool ret = false;

  if (ch >= RF_MAX_CH)
  {
    return 0;
  }

  if (rf_tbl[ch].p_rcswitch -> available())
  {
    ret = true;
  }

  return ret;
}

uint32_t rfGetData(uint8_t ch)
{
  if (ch >= RF_MAX_CH)
  {
    return 0;
  }

  return rf_tbl[ch].received_data;
}

static uint16_t rfGetLength(uint8_t ch)
{
  if (ch >= RF_MAX_CH)
  {
    return 0;
  }

  return rf_tbl[ch].p_rcswitch -> getReceivedBitlength();
}

RF_Event_t rfGetEvent(uint8_t ch)
{
  RF_Event_t rf_event = rf_tbl[ch].received_event;
  rf_tbl[ch].received_event = RF_HW_EVENT_NONE; // 읽으면 초기화
  return rf_event;
}

#endif