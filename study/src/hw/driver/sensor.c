#include "../Inc/sensor.h"

#ifdef _USE_HW_SENSOR

#define INPUT_VOLTAGE 3.3f
#define SENSOR_12BITS_RESOLUTION_VALUE 4095.0f

#define TARGET_MOTOR_GAIN    0.554f

typedef struct
{
  float    sensed_current;
} sensor_t;

static sensor_t sensor[SENSOR_MAX_CH];

bool sensorInit(void)
{
  bool ret = true;

  analogReadResolution(12);

  for (int i=0; i<SENSOR_MAX_CH; i++)
  {
    sensor[i].sensed_current = 0;
  }
  return ret;
}

float getSensedValue(uint8_t ch)
{
    // 1. ADC 읽기
    uint32_t raw_adc_val = analogRead(SENSOR_PIN);
    
    // 2. 전압 변환
    float sensed_voltage = (raw_adc_val / (float)SENSOR_12BITS_RESOLUTION_VALUE) * INPUT_VOLTAGE;
    
    // 3. 전류 계산
    float raw_current = sensed_voltage / TARGET_MOTOR_GAIN;
    
    // 4. 로우패스 필터 적용 (90% 이전값 + 10% 새값)
    static float filtered_current[SENSOR_MAX_CH] = {0};  // 채널별 필터링 값 저장
    filtered_current[ch] = filtered_current[ch] * 0.9f + raw_current * 0.1f;
    
    // 5. 센서 구조체에 저장
    sensor[ch].sensed_current = filtered_current[ch];
    
    return sensor[ch].sensed_current;
}


#endif