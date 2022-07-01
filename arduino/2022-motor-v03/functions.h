#include <CAN.h>
#include <elapsedMillis.h>

#define ENA   23

#define PWM1  14
#define DIR1  27

#define PWM2  33
#define DIR2  32

#define PWM3  22
#define DIR3  21

#define PWM4  19
#define DIR4  18




int timeout = 101;

int startzeit =0;

int LED = 12;

void fahre(int richtung, int geschwindigkeit, int drehung)    ;
void aus()                                                    ;
void roll()                                                   ;

double myAbs(double d)
{
  if (d<0) return -d;
  return d;
}

void checkCanBus()
{
   //Serial.println("checkcan");
  int packetSize = CAN.parsePacket();
  if (packetSize)
    {
       //Serial.println("was da");
      //Serial.println("es ist etwas da");
      int SensorId = CAN.packetId();
      if(SensorId == 0x10)
      {
        //Serial.println("für mich");
        int richtung = CAN.read() -7;           //Serial.println(richtung);
        int geschwindigkeit = CAN.read() ;      //Serial.println(geschwindigkeit);
        int drehung = CAN.read() -100;          //Serial.println(drehung);
        if(geschwindigkeit==101)                roll();
        else   if(startzeit>=20)                                 fahre(richtung, geschwindigkeit, drehung);
      }
      timeout = 0;   
      
      digitalWrite(LED,LOW);
    }
    
   else 
   { 
    timeout++; 
    
    Serial.println("Irgentwas geht schief"); 
   }
}



int spdToPwm(double spd) 
{
 //pwm 25..229 (<10%..<90%) für 0..100
 return (int) (25.5 + (203/100)*myAbs(spd)); 
}

void schreibeMotoren(boolean e, int p1, boolean d1, int p2, boolean d2, int p3, boolean d3, int p4, boolean d4) // Achtung neu !!!
{
  digitalWrite(ENA, e);

  ledcWrite(1, p1);
  ledcWrite(2, p2);
  ledcWrite(3, p3);
  ledcWrite(4, p4);
  
  digitalWrite(DIR1, d1);
  digitalWrite(DIR2, d2);
  digitalWrite(DIR3, d3);
  digitalWrite(DIR4, d4);
}

void fahre(int richtung, int geschwindigkeit, int drehung) 
{
  //Serial.println();
  //Serial.println(String(richtung) + " " + String(geschwindigkeit) + " " + String(drehung));
  //Serial.println();
  //richtung: -8..8 in 22.5 Grad Schritten
  //geschwindigkeit:   0..100
  //drehung:     -100..100
 
  int p1, p2, p3, p4;               //  PWM Werte
  boolean d1, d2, d3, d4, ena;          //  Richtungs Werte
  double spd_1, spd_2, spd_3, spd_4; //geschwindigkeits Werte
  double winkel = richtung * PI/8;             //Richtung in Radiant
  double radmax;                   //geschwindigkeit den schnellsten Rads
  
  //Parameter fixen
  //drehung = -drehung;                  //rechts ist plus...
 
  if(geschwindigkeit<0) geschwindigkeit=0;
  if(geschwindigkeit>100) geschwindigkeit=100;
  richtung=richtung % 8;
  
  if( drehung > ( 100-geschwindigkeit) ) drehung= 100-geschwindigkeit;  // geschwindigkeit und drehung können 100 nicht überschreiten
  if( drehung < (-100+geschwindigkeit) ) drehung=-100+geschwindigkeit;

  //Lineare Anteile berechnen         Siehe readme.h
  spd_1= sin(winkel) - cos(winkel);
  spd_2=-sin(winkel) - cos(winkel);
  spd_3=-sin(winkel) + cos(winkel);
  spd_4= sin(winkel) + cos(winkel);


    
  // auf maximal 100% ziehen
  radmax = myAbs(spd_1);
  if (myAbs(spd_2)>radmax) radmax= myAbs(spd_2);
  if (myAbs(spd_3)>radmax) radmax= myAbs(spd_3);
  if (myAbs(spd_4)>radmax) radmax= myAbs(spd_4);
  
  if (radmax > 1) { spd_1 = spd_1/radmax;    spd_2 = spd_2/radmax;    spd_3 = spd_3/radmax;    spd_4 = spd_4/radmax;  } 


  spd_1 = (spd_1*geschwindigkeit)+drehung;  
  spd_2 = (spd_2*geschwindigkeit)+drehung;  
  spd_3 = (spd_3*geschwindigkeit)+drehung;  
  spd_4 = (spd_4*geschwindigkeit)+drehung;



p1=spdToPwm(spd_1);  
  p2=spdToPwm(spd_2);  
  p3=spdToPwm(spd_3);  
  p4=spdToPwm(spd_4);
 
  if (spd_1 > 0 ) d1 = HIGH; else d1 = LOW;
  if (spd_2 > 0 ) d2 = HIGH; else d2 = LOW;  
  if (spd_3 > 0 ) d3 = HIGH; else d3 = LOW;
  if (spd_4 > 0 ) d4 = HIGH; else d4 = LOW;  
  ena=HIGH;

  schreibeMotoren(ena, p1, d1 ,p2, d2, p3, d3, p4, d4);

} // end of fahre

void aus() // Aktives AUS für Maxon (und Pololu?) 
{
  fahre(0,0,0);
}

void roll() // spricht für sich
{
  schreibeMotoren(LOW,0,HIGH,0,HIGH,0,HIGH,0,HIGH);
  digitalWrite(LED,HIGH);
}
