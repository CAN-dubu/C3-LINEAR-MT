#include "ap.h"



void apInit(void)
{
  inputInit();
}

void apMain(void)
{
  // sampleQueueUpdate();
  while(1)
  {
    inputProcess();
    // remoteProcess();
    // motorProcess();
    // modeProcess();
  }
}
