#include "ap.h"



void apInit(void)
{
  sampleQueueInit();
}

void apMain(void)
{
  // sampleQueueUpdate();
  while(1)
  {
    inputProcess();
    remoteProcess();
    motorProcess();
    modeProcess();
  }
}

// void apMain(void)
// {

// }
