#ifndef _HW_SENSOR_H_
#define _HW_SENSOR_H_

#include "../hw_def.h"

#define SENSOR_MAX_CH       HW_SENSOR_MAX_CH

#define SENSOR_PIN 3

bool sensorInit(void);
float getSensedValue(uint8_t ch);

#endif