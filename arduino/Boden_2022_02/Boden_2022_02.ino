/*
   Bodensensor Canbus Platine 2022_01_10
		Serielle ausgabe in Line 215
*/

#include <elapsedMillis.h>
#include <CAN.h>
#define TOT 2 // hier wird die Totzeit gesetzt. Je ein TOT entspricht 20ms. eine TOT von 5 ist 100ms

#include "Zuordnung.h"
elapsedMillis zeit;

int schwellwert = 250;  // setzen und immer wieder prüfen !!!


int sensor[] = { 26, 33, 32, 25}; // 4 Ananlogeingänge für 4 Multiplexer
int zonen[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte zuordnungzone = 0b00000000;

int bodenled = 2;

int add0 = 13; // 8 port Multiplexer Adressleitungen
int add1 = 27;
int add2 = 14;

boolean sieht_weiss = false;
boolean weiss_abweichung = false;
boolean weiss_klar = false;
int weiss_winkel;
int weiss_winkel_save;
byte weiss_byte;
byte weiss_paket;
int weiss_totzeit;
char ausgabe[8];


void checkCanBus() 
{
  
  int packetSize = CAN.parsePacket();
  if (packetSize)
  {
    int SensorId = CAN.packetId();
    if(SensorId == 0x01)
    {
      if (CAN.packetRtr()) 
      {
        //Serial.println("Boden hat etwas"); 
        CAN.beginPacket(0x01);  //Schicke an Hauptplatine
        CAN.write(weiss_paket);
        CAN.endPacket();
      } 
    }
  }
}



// nur für debug-zwecke......
void printBits(byte myByte)
{
  for (byte mask = 0x80; mask; mask >>= 1)
  {
    if (mask  & myByte) Serial.print('1');
    else               Serial.print('0');
  }
}
// Ende nur für debug-zwecke......



void setup()
{
  pinMode(bodenled, OUTPUT);
  pinMode(add0, OUTPUT);
  pinMode(add1, OUTPUT);
  pinMode(add2, OUTPUT);

  Serial.begin(115200);
  digitalWrite(bodenled, LOW);

  setze_zuordnung();
  if (!CAN.begin(500E3)) {Serial.println("CAN failed!"); while (1);}
   Serial.println("Setup erfolgreich");
}
void setzeMultiplexer(int adresse)
{
  if ((adresse & 1) == 1) {
    digitalWrite(add0, HIGH); // die Adressports werden digital hochgezählt
  } else {
    digitalWrite(add0, LOW);
  }
  if ((adresse & 2) == 2) {
    digitalWrite(add1, HIGH);
  } else {
    digitalWrite(add1, LOW);
  }
  if ((adresse & 4) == 4) {
    digitalWrite(add2, HIGH);
  } else {
    digitalWrite(add2, LOW);
  }
  delayMicroseconds(60); // damit der multiplexer umschalten kann
}

void sensorread() // liest alle 64 Sensorwerte aus und speicher sie im Puffer ab.
{
  for (int z = 0; z < 8; z++)  zonen[z] = 0; // Alle Zonen auf "keine Linie" setzen

  for (int lauf=0; lauf<8; lauf++)
  {
    setzeMultiplexer(lauf);
    /*for (int innen=0; innen<4; innen++)
    {
      Serial.print(analogRead(sensor[innen]));Serial.print(", ");
    }
    Serial.println();*/
  }
  /*Serial.println();
  setzeMultiplexer(6);
  if (analogRead(sensor[3]) > schwellwert) Serial.println("6");
  //Serial.println(analogRead(sensor[0]));
  */
  setzeMultiplexer(2);  // r3
    if (analogRead(sensor[0]) > schwellwert) zonen[1] = zonen[1] + 1;
    if (analogRead(sensor[1]) > schwellwert) zonen[3] = zonen[3] + 1;
    if (analogRead(sensor[2]) > schwellwert) zonen[5] = zonen[5] + 1;
    if (analogRead(sensor[3]) > schwellwert) zonen[7] = zonen[7] + 1;
    
  setzeMultiplexer(4);  //r2
    if (analogRead(sensor[0]) > schwellwert) {zonen[0] = zonen[0] + 1;zonen[1] = zonen[1] + 1;}
    if (analogRead(sensor[1]) > schwellwert) {zonen[2] = zonen[2] + 1;zonen[3] = zonen[3] + 1;}
    if (analogRead(sensor[2]) > schwellwert) {zonen[4] = zonen[4] + 1;zonen[5] = zonen[5] + 1;}
    if (analogRead(sensor[3]) > schwellwert) {zonen[6] = zonen[6] + 1;zonen[7] = zonen[7] + 1;}
    
  setzeMultiplexer(0);  //r1
    if (analogRead(sensor[0]) > schwellwert) zonen[0] = zonen[0] + 1;
    if (analogRead(sensor[1]) > schwellwert) zonen[2] = zonen[2] + 1;
    if (analogRead(sensor[2]) > schwellwert) zonen[4] = zonen[4] + 1;
    if (analogRead(sensor[3]) > schwellwert) zonen[6] = zonen[6] + 1;
  
  setzeMultiplexer(6);  //mitte
    if (analogRead(sensor[0]) > schwellwert) zonen[0] = zonen[0] + 1;
    if (analogRead(sensor[1]) > schwellwert) zonen[2] = zonen[2] + 1;
    if (analogRead(sensor[2]) > schwellwert) zonen[4] = zonen[4] + 1;
    if (analogRead(sensor[3]) > schwellwert) zonen[6] = zonen[6] + 1;
    
  setzeMultiplexer(5);  //l1
    if (analogRead(sensor[0]) > schwellwert) zonen[0] = zonen[0] + 1;
    if (analogRead(sensor[1]) > schwellwert) zonen[2] = zonen[2] + 1;
    if (analogRead(sensor[2]) > schwellwert) zonen[4] = zonen[4] + 1;
    if (analogRead(sensor[3]) > schwellwert) zonen[6] = zonen[6] + 1;
  
  setzeMultiplexer(7);  //l2
    if (analogRead(sensor[0]) > schwellwert) {zonen[0] = zonen[0] + 1;zonen[7] = zonen[7] + 1;}
    if (analogRead(sensor[1]) > schwellwert) {zonen[2] = zonen[2] + 1;zonen[1] = zonen[1] + 1;}
    if (analogRead(sensor[2]) > schwellwert) {zonen[4] = zonen[4] + 1;zonen[3] = zonen[3] + 1;}
    if (analogRead(sensor[3]) > schwellwert) {zonen[6] = zonen[6] + 1;zonen[5] = zonen[5] + 1;}
    
  setzeMultiplexer(3);  //l3
    if (analogRead(sensor[0]) > schwellwert) zonen[7] = zonen[7] + 1;
    if (analogRead(sensor[1]) > schwellwert) zonen[1] = zonen[1] + 1;
    if (analogRead(sensor[2]) > schwellwert) zonen[3] = zonen[3] + 1;
    if (analogRead(sensor[3]) > schwellwert) zonen[5] = zonen[5] + 1;
  
  setzeMultiplexer(1);  //l4
    if (analogRead(sensor[0]) > schwellwert) zonen[7] = zonen[7] + 1;
    if (analogRead(sensor[1]) > schwellwert) zonen[1] = zonen[1] + 1;
    if (analogRead(sensor[2]) > schwellwert) zonen[3] = zonen[3] + 1;
    if (analogRead(sensor[3]) > schwellwert) zonen[5] = zonen[5] + 1;
 
  
} // end of sensorread()

void zone()
{
  zuordnungzone = 0b00000000;
  if (zonen[0] > 0) {
    zuordnungzone = zuordnungzone + 0b00000001;
  }
  if (zonen[1] > 0) {
    zuordnungzone = zuordnungzone + 0b00000010;
  }
  if (zonen[2] > 0) {
    zuordnungzone = zuordnungzone + 0b00000100;
  }
  if (zonen[3] > 0) {
    zuordnungzone = zuordnungzone + 0b00001000;
  }
  if (zonen[4] > 0) {
    zuordnungzone = zuordnungzone + 0b00010000;
  }
  if (zonen[5] > 0) {
    zuordnungzone = zuordnungzone + 0b00100000;
  }
  if (zonen[6] > 0) {
    zuordnungzone = zuordnungzone + 0b01000000;
  }
  if (zonen[7] > 0) {
    zuordnungzone = zuordnungzone + 0b10000000;
  }
}

void linie_erkennen()
{
  if (zuordnungzone > 0)   // weiße Linie wird irgendwo erkannt
  {
    weiss_totzeit = TOT;
    weiss_winkel   = zuordnung[zuordnungzone];
   
    // zuerst prüfen, ob die Linie ganz neu ist...
    if (sieht_weiss == false) weiss_winkel_save = weiss_winkel;              // save Wert ist neuwert !!!

    sieht_weiss = true;
    //Serial.println(weiss_winkel);
    // prüfen, ob die Werte etwas taugen
    if (weiss_winkel_save > 8)    // Sie taugen nichts. Mist!
    {
      weiss_winkel_save = 0;
      weiss_klar = false;
    }

    else      // die Linie wurde erkannt, die Savelinie ist klar definiert...
    {
      weiss_klar = true;
      // Abweichung 90 Grad oder weniger, inkl. Sprung von -7 bis 7   UND weiss_winkel klar definiert (also nicht 100 oder 200)
      if (((abs(weiss_winkel_save - weiss_winkel) < 5) | (abs(weiss_winkel_save - weiss_winkel) > 11)) & (weiss_winkel < 9)) // Abweichung 90 Grad oder weniger    // muss noch oder Sprung -7->8 und umgekehrt
      {
        weiss_winkel_save = weiss_winkel;
        weiss_abweichung = false;
      }
      else weiss_abweichung = true;
    }
  }

  else
  {
    if (weiss_totzeit > 0) weiss_totzeit--;
    else sieht_weiss = false;
  }


}

void pakete_erstellen()
{
  /*
     Bit 7    = 0
     Bit 6    Linie erkannt? -> Das Bit, welches zur Handlung zwingt... 1 für erkannt, 0 für nix
     Bit 5    Abweichung (aktuelle Linie == erste Linie ?) 0 für gleich, 1 für anders
     Bit 4    Klares Ergebnis (sollte immer so sein, aber wenn nicht -> Problem. 1 für gut, 0 für Problem
     Bit 0-3  16 unterschiedliche Richtungen -> Umwandeln auf -7 bis 8
  */
  weiss_byte = 0;
  if (sieht_weiss == true)      weiss_byte = weiss_byte + 64;
  if (weiss_abweichung == true) weiss_byte = weiss_byte + 32;
  if (weiss_klar == true)       weiss_byte = weiss_byte + 16;
  weiss_byte  = weiss_byte + (weiss_winkel_save + 7);
  // für CanBus
  weiss_paket = weiss_byte;
}


void loop()
{
  zeit = 0;

  digitalWrite(bodenled, HIGH);
  delayMicroseconds(100);             // eigentlich nur 100
  sensorread();                       //dauert weniger als 1ms - testen
  digitalWrite(bodenled, LOW);
  zone();
  linie_erkennen();
  pakete_erstellen();
  checkCanBus();
  //schreibe();
  //Serial.println(" Linie :" + String(weiss_abweichung));
  while (zeit < 2) checkCanBus();    // fülle auf 1 Sekunde auf - nur Diagnose !!!(momentan 2 milisekunden)
  
}


void schreibe()
{
  
  Serial.print(String(zonen[0])+",  ");
  Serial.print(String(zonen[1])+",  ");
  Serial.print(String(zonen[2])+",  ");
  Serial.print(String(zonen[3])+",  ");
  Serial.print(String(zonen[4])+",  ");
  Serial.print(String(zonen[5])+",  ");
  Serial.print(String(zonen[6])+",  ");
  Serial.print(String(zonen[7])      );
  Serial.println();
  
  
  
  /*
  Serial.print("Weiß    : ");   Serial.print("Zone : " );
  printBits(zuordnungzone);     Serial.println(" Linie :" + String(weiss_winkel));

  //Serial.println("Zeit   : "+String(zeit));
  Serial.println();
  Serial.println("weißpaket : " + String(weiss_paket));
  Serial.println();



  Serial.println(); Serial.println("Zeit   : " + String(zeit));
  Serial.println();
  */

}
