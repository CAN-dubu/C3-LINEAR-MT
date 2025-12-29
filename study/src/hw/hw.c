#include "hw.h"

bool hwInit(void)
{
  buttonInit();
  ledInit();
  rfInit();
  motorInit();
  sensorInit();

  return true;
}