#include "src/hw/hw.h"
#include "src/ap/ap.h"

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  hwInit();   
  apInit();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  apMain();
}
