#include "current_sensor.h"

void currentSensorProcess(void)
{
  static uint32_t low_cur_detected_time = 0;
  static bool is_low_cur_detected = false;
  static const float sensor_threshold = 0.04;

  if (getSensedValue(SENSOR_CH1) < sensor_threshold)
  {
    if (!is_low_cur_detected)
    {
      low_cur_detected_time = millis();
      is_low_cur_detected = true;
    }
    else
    {
      if (millis() - low_cur_detected_time >= MOTOR_OVERCURRENT_DELAY_MS)
      {
        motorRequestAction(MOTOR_CH1, MODE_ACTION_MOTOR_STOP);
        is_low_cur_detected = false;
      }
    }
  }
  else
  {
    if (is_low_cur_detected)
    {
      is_low_cur_detected = false;
    }
  }
  
}