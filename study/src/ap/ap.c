#include "ap.h"



void apInit(void)
{
  inputInit();
  remoteInit();
  apMotorInit();
  modeInit();
}

void apMain(void)
{
  while(1)
  {
    inputProcess();
    remoteProcess();
    modeProcess();
    currentSensorProcess();
  }
}
