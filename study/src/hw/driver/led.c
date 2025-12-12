#include "../Inc/led.h"

#ifdef _USE_HW_LED

#include "../Inc/gpio.h"

#define LED_PIN 2
#define LED2_PIN 1

typedef struct 
{
  uint8_t          pin;
  uint8_t          pin_mode;
  gpio_pin_state_t on_state;
  gpio_pin_state_t off_state;
  gpio_pin_state_t current_state;
} led_tbl_t;

static led_tbl_t led_tbl[LED_MAX_CH] = 
{
  {LED_PIN,  OUTPUT,  LOW,  HIGH},
  {LED2_PIN, OUTPUT,  LOW,  HIGH},
};

bool ledInit(void)
{
  bool ret = true;

  for(int i=0; i<LED_MAX_CH; i++)
  {
    pinMode(led_tbl[i].pin, led_tbl[i].pin_mode);
    
    ledOff(i);
  }

  return ret;
}

void ledOn(uint8_t ch)
{
  if (ch >= LED_MAX_CH) 
  {
    return;
  }

  digitalWrite(led_tbl[ch].pin, led_tbl[ch].on_state);
  led_tbl[ch].current_state = led_tbl[ch].on_state;
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_MAX_CH)
  {
    return;
  }

  digitalWrite(led_tbl[ch].pin, led_tbl[ch].off_state);
  led_tbl[ch].current_state = led_tbl[ch].off_state;
}

void allLedOff(void)
{
  for (int i=0; i<LED_MAX_CH; i++)
  {
    ledOff(i);
    led_tbl[i].current_state = led_tbl[i].off_state;
  }
}

void allLedTogglePin(uint32_t intervel_ms)
{
  static uint32_t pretime;

  if (millis() - pretime >= intervel_ms)
  {
    for (int i=0; i<LED_MAX_CH; i++)
    {
      led_tbl[i].current_state = !led_tbl[i].current_state;
      digitalWrite(led_tbl[i].pin, led_tbl[i].current_state);
    }

    pretime = millis();
  }
}

#endif