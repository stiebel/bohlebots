#include "functions.h"

void setup() 
{
  Serial.begin(115200);
  Serial.print("Setup ... ");
  Serial.println("abs="+String(myAbs(-0.5)));
  
  // Motor Pins einrichten
  ledcAttachPin(PWM1, 1); ledcSetup(1, 1000, 8);  ledcWrite(1, 0);
  ledcAttachPin(PWM2, 2); ledcSetup(2, 1000, 8);  ledcWrite(2, 0);
  ledcAttachPin(PWM3, 3); ledcSetup(3, 1000, 8);  ledcWrite(3, 0);
  ledcAttachPin(PWM4, 4); ledcSetup(4, 1000, 8);  ledcWrite(4, 0);
   
  pinMode(ENA, OUTPUT);  digitalWrite(ENA, LOW);
  pinMode(DIR1, OUTPUT); 
  pinMode(DIR2, OUTPUT);
  pinMode(DIR3, OUTPUT);
  pinMode(DIR4, OUTPUT);

  pinMode(LED, OUTPUT);
  
 
  while (!Serial);

  Serial.println("CAN Setup");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) 
  {
    Serial.println("Starting CAN failed!");
    while (1);
  }
   CAN.filter(0x10);
   Serial.print("Setup beendet");

   fahre(0,0,0);
 
  delay(500);
  
}
void loop() 
{  

  //fahre(0,0,20);
  
  //roll();
   
   
   if (startzeit<20)   startzeit++;
   checkCanBus(); 
   if (timeout>100) roll();
  delay(1);
  Serial.println(startzeit);
}
