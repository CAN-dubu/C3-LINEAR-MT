#include "../Inc/motor.h"

#ifdef _USE_HW_MOTOR

#include "../Inc/gpio.h"

typedef struct
{
  uint8_t  dir_a_pin;
  uint8_t  dir_b_pin;
  uint8_t  pin_mode;
  uint8_t  pwm_bit;
  uint16_t pwm_frequency;
} motor_hw_t;

typedef struct 
{
  uint16_t            motor_max_speed;
  uint8_t             motor_min_speed;
  const motor_hw_t   *p_hw;
} motor_t;

static const motor_hw_t motor_hw[MOTOR_MAX_CH] =
{
  { MOTOR_IN1_PIN, MOTOR_IN2_PIN, OUTPUT, MOTOR_PWM_8BIT_RESOLUTION, MOTOR_PWM_FREQ},
};

static motor_t motor_tbl[MOTOR_MAX_CH];

bool motorInit(void)
{
  bool ret = true;

  for(int i=0; i<MOTOR_MAX_CH; i++)
  {
    pinMode(motor_hw[i].dir_a_pin, motor_hw[i].pin_mode);
    pinMode(motor_hw[i].dir_b_pin, motor_hw[i].pin_mode);
    ledcAttach(motor_hw[i].dir_a_pin, motor_hw[i].pwm_frequency, motor_hw[i].pwm_bit);
    ledcAttach(motor_hw[i].dir_b_pin, motor_hw[i].pwm_frequency, motor_hw[i].pwm_bit);
  }

  for(int i=0; i<MOTOR_MAX_CH; i++)
  {
    motor_tbl[i].p_hw = &motor_hw[i];
    motor_tbl[i].motor_min_speed = 0;
    motor_tbl[i].motor_max_speed = (1UL << motor_tbl[i].p_hw->pwm_bit)  - 1;
  }

  return ret;
}

void motorGoUp(uint8_t ch)
{
  ledcWrite(motor_tbl[ch].p_hw->dir_a_pin, motor_tbl[ch].motor_max_speed);
  ledcWrite(motor_tbl[ch].p_hw->dir_b_pin, motor_tbl[ch].motor_min_speed);
}

void motorGoDown(uint8_t ch)
{
  ledcWrite(motor_tbl[ch].p_hw->dir_a_pin, motor_tbl[ch].motor_min_speed);
  ledcWrite(motor_tbl[ch].p_hw->dir_b_pin, motor_tbl[ch].motor_max_speed);
}

void motorStop(uint8_t ch)
{
  ledcWrite(motor_tbl[ch].p_hw->dir_a_pin, motor_tbl[ch].motor_min_speed);
  ledcWrite(motor_tbl[ch].p_hw->dir_b_pin, motor_tbl[ch].motor_min_speed);
}
#endif