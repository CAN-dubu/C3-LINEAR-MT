#include "hw.h"

bool hwInit(void)
{
  buttonInit();
  ledInit();
  rfInit();
  // motorInit();

  return true;
}