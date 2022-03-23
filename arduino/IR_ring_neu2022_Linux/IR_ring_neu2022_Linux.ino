// quadro ir_ring 
// Version 2019 CAN Bus
// letzte Änderung : 02.03.2022
// Canbus return auf 0x03
// Diese Datei muss nicht geändert werden !!!

#include <elapsedMillis.h>
#include <CAN.h>
#include "RunningMedian.h"
#include "header.h"      // eigentlich keine echter header, aber hier steht alles drin, was für alle Ringe gilt
#include "functions.h"   // alle Funktionen, die unverändert bleiben sollten

void loop() 
{
  messen();
  checkCanBus();
  //canbus();schreibe();     //nur diagnose
      
} // von loop - rund 11ms -> 90 mal den Ball sehen in 1 Sekunde !!!
