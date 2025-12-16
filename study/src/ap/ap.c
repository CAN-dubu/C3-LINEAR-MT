#include "ap.h"



void apInit(void)
{
  inputInit();
  remoteInit();
  modeInit();
}

void apMain(void)
{
  while(1)
  {
    inputProcess();
    remoteProcess();
    // motorProcess();
    modeProcess();
  }
}
