/*
 * Alle header Informationen
 * z.B. alle Variablen
 * und
 * void setup()
 */
#include <CAN.h>
#include <elapsedMillis.h>
#include <Pixy2.h>
#include <Wire.h>
#include <math.h>
#define SDA 21
#define SCL 22
#define LICHTSCHRANKE 33
#define KICKER 32
#define LED 12

#define I2C_TASTER 0x20
#define I2C_LED 0x27

Pixy2 pixy;
#define EEPROM_SIZE 64
byte eprombyte[4] = {0,0,0,0};
byte tastbyte;    // speichert das Ergebnis der i2c Bus Tastenanfrage
byte ledbyte;     // speichert die per i2c Bus zu schaltenden LEDs
boolean st;       //stuermer
   
int kompass;      // speichert das Ergebnis der Kompass-Anfrage kompass_lesen()
int head;         // speichert die Tor-Richtung für den Kompass

int can_richtung;         // speichert die Werte aus dem fahre() - Befehl für den can-Bus
int can_geschwindigkeit;
int can_drehung;


int ball_richtung;        // speichert den aus dem Can-Bus gelesenen Wert der Ballrichtung
int ball_max;             // eigentlich Entfernung, wird nur als "0" = "kein Ball zu sehen" benutzt

int boden_richtung;       // speichert die Richtung, in der die weiße Linie gesehen wird
int boden_abweichung;     // speichert, ob es eine Abweichung > 90 zum ersten Wert gab
int boden_klar;           // speichert, ob die Bodenrichtuing klar definiert werden konnte
int boden_vorhanden;      // speichert, ob wir eine weiße Linie sehen
int boden_korektur;

boolean hat_ball;         // speichert die Auswertung der Lichtschranke

int tor_richtung;         // speichert die x-Position des Tors nach Auswertung von Kompass UND Pixy2
int tor_breite;           // speichert die Torbreite nach Auswertung durch Pixy2
int tor_entfernung;       // speichert die Torentfernung nach Auswertung durch Pixy2 
float tor_entfernung_roh; 
boolean hat_tor;

int r_oder_l;             // speichert, ob das Tor links oder rechts ist
boolean r_o_l_aenderbar;
int fahre_echt;           // speichert die zuletzt gefahrene Richtung bei "nicht-Linie"
int grund_sp = 65;

byte bt_available;
byte bt_angekommen;
byte bt_data_1;
byte bt_data_2;
int ball_richtung_b; 
int ball_max_b;
int irpaket_b;
int tor_entfernung_b;
boolean hat_tor_b;
int torpaket_b;
int bt_tempo;


elapsedMillis boden_zeit; // wie lange hat der Bot das letzte Mal gegengesteuert (oft bodenzeit = gruenzeit)
boolean boden_zeit_hilfe; // speichert, ob boden_zeit auf 0 gesetzt werden darf
int hektik;
elapsedMillis gruen_zeit; // wie lange hat der Bot die weiße Linie nicht gesehen?
int gruen_zeit_limit;     // wie lange von Linie wegfahren - variabel nach Ballrichtung

int           anti_nitro;
elapsedMillis anti_nitro_hilfe;

elapsedMillis loop_time;
elapsedMillis kick_totzeit; // die Zeit die mindestens zwischen zwei Kicks liegen muss

byte matrix[16][16]={
                                //0                                 //8
  { 0,  0,  0,  0,  0,  1,  1,    1,    1,  1,  1,  1,  1,  1,  1,    0},
  { 0,  0,  0,  0,  0,  0,  1,    1,    1,  1,  1,  1,  1,  1,  1,    1},
  { 1,  0,  0,  0,  0,  0,  0,    1,    1,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  0,  0,  0,  0,  0,    0,    1,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  0,  0,  0,  0,    0,    0,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  0,  0,  0,    0,    0,  0,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  0,  0,    0,    0,  0,  0,  1,  1,  1,  1,    1},
  
  { 1,  1,  1,  1,  0,  0,  0,    0,    0,  0,  0,  1,  1,  1,  1,    1},//0
  
  { 1,  1,  1,  1,  0,  0,  0,    0,    0,  0,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  0,  0,    0,    0,  0,  0,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  0,    0,    0,  0,  0,  0,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  1,    0,    0,  0,  0,  0,  0,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  1,    1,    0,  0,  0,  0,  0,  0,  1,    1},
  { 1,  1,  1,  1,  1,  1,  1,    1,    1,  0,  0,  0,  0,  0,  0,    1},
  { 1,  1,  1,  1,  1,  1,  1,    1,    1,  1,  0,  0,  0,  0,  0,    0},
  
  { 0,  0,  0,  0,  1,  1,  1,    1,    1,  1,  1,  0,  0,  0,  0,    0}//8

}; 
// von -7 bis 8 in Zeilen die Linie
// von -7 bis 8 in Spalten der Ball


#include "my_oled.h"
#include "my_kompass.h"
#include "my_pixy.h"
#include "my_can.h"
#include "my_functions.h"
#include "my_eprom.h"

void setup() 
  {
    st =false;                    // setzte Progarmm in standby - Modus
    Serial.begin(115200);
    Serial2.begin(38400);
    pinMode(LED,OUTPUT);    digitalWrite(LED, LOW); 
    pinMode(KICKER,OUTPUT); digitalWrite(KICKER, LOW); 
    pinMode(LICHTSCHRANKE, INPUT);
    while (!Serial);          // wartet, bis ESP32-Serial verfügbar ist.
    
    Serial.print("Warte auf Canbus");
    if (!CAN.begin(500E3)) {Serial.print("...failed!");while (1);  }// start the CAN bus at 500 kbps
    else Serial.println("...succeed");
    if (!EEPROM.begin(EEPROM_SIZE))
    {
      Serial.println("failed to initialise EEPROM"); delay(100000);
    }
    gruen_zeit   =1000;
    boden_zeit  =1000;
    boden_zeit_hilfe =false;
    
    Wire.begin(SDA,SCL);
    i2c_taster_init();
    ledbyte = 255;  i2c_led_setzen(ledbyte);  //delay(500); // alle an
    //ledbyte = 0;    i2c_led_setzen(ledbyte);              //  alle aus
      
    pixy.init();
    oled_init();
    
    eprom_init();
    ledbyte = 0;    i2c_led_setzen(ledbyte);              //  alle aus
    boden_korektur=0;
  }
  
  
/*
int nitro_matrix[16][4]={
  //torzone:
  //-1,  0,  1,  X    fahre_echt:
  {  0,  0,  0,  0},//-7
  {  5, 45, 10,  0},//-6
  { 20, 45, 10,  0},//-5
  { 40, 45, 15,  0},//-4
  { 40, 45, 10,  0},//-3
  {  0,  0,  0,  0},//-2
  {  0,  0,  0,  0},//-1
  { 30, 30, 30,  0},// 0
  {  0,  0,  0,  0},// 1
  {  0,  0,  0,  0},// 2
  { 10, 45, 40,  0},// 3
  { 15, 45, 40,  0},// 4
  { 10, 45, 20,  0},// 5
  { 10, 45,  5,  0},// 6
  {  0,  0,  0,  0},// 7
  { 15, 45, 15,  0} // 8
};
*/
