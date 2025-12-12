#ifndef HW_H_
#define HW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#include "Inc/button.h"
#include "Inc/led.h"
#include "Inc/rf.h"
#include "Inc/motor.h"

bool hwInit(void);
void hwUpdate(void);

#ifdef __cplusplus
}
#endif

#endif