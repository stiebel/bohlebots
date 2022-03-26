
#include "BohleBlue.h"

BohleBlue quadroBT;

void setup() 
{
   Serial.begin(115200);
   Serial.println("Bluetooth test");
   quadroBT.init();
}

void loop() 
{
  byte b = quadroBT.checkBT();
  if (b>0) Serial.println("empfangen: "+String(b));
  byte m = quadroBT.modus();
  //if ((m%10)==1)  Serial.println("ich bin Master");
  //else            Serial.println("ich bin Slave");

  if ((m/10)==1)  Serial.println("connected");
  else            Serial.println("Problem");

  quadroBT.writeBT(205); // willkürlich
  delay(200);
}
