#include "ap.h"



void apInit(void)
{
  inputInit();
  modeInit();
}

void apMain(void)
{
  while(1)
  {
    inputProcess();
    // remoteProcess();
    // motorProcess();
    modeProcess();
  }
}
