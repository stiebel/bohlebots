//LINE 160 in sensors.h !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ////////////////////////////////////////////////
 /*
  CHECK ON EVERY NEW FIELD:
  - GOAL_DIST_RATIO
  - MAX_SPEED_AT_LINE
  - GOAL_ANGLE_GAMEMODESWITCH_INNER_BOARDER
  - GOAL_ANGLE_GAMEMODESWITCH_OUTER_BOARDER
  */
 /////////////////////////////////////////////////

//////////////////////////////////////////////////
/*
  AENDERUNGEN: 
  functions.h -> Zeile 230 - 233
    --> rechnung geaendert um zu verhindern dass bot schraeg, nah hinter ball zu langsam wird
  declerations.h -> SPEED_TRAP von 70 zu 65
  striker.h -> Zeile 50
    --> striker faehrt jetzt nicht 0 mit drehung sondern 1*side(goalDir) mit drehung
 */
//////////////////////////////////////////////////

 
#include <Wire.h>
#include <CAN.h>
#include <elapsedMillis.h>
#include "complex.h"
#include "declerations.h"
#include "eeprom.h"
#include "movement.h"
#include "sensors.h"
#include "interface.h"
#include "functions.h"
#include "striker.h"
#include "dribbler.h"
#include "goalie.h"
#include "defender.h"
#include "debug.h"

void setup() {
  //Serial.begin(115200);
  Serial.begin(38400);
  Serial.println("succesfully set up serial1");
  
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("succesfully set up serial connection to pi");
  
  pinMode(LIGHT_PIN, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.println("succesfully set up lighbarrier");
  
  Wire.begin(SDA,SCL);
  cmps_set_heading();
  Serial.println("succesfully set up cmps");
  
  if(!CAN.begin(500E3)) while(1); 
  Serial.println("succesfully set up CAN");

  gameMode = strikerGameMode;
  Serial.println("set gamemode to striker");

  Wire.beginTransmission(CONTROL_ADD_LEFT);
  if (Wire.endTransmission()!= 0) { 
    Serial.println("LEFT CONTROL DEVICE FAILED");
    while(1);
  }

  Wire.beginTransmission(CONTROL_ADD_MIDDLE);
  if (Wire.endTransmission()!= 0) {
    Serial.println("MIDDLE CONTROL DEVICE FAILED");
    while(1);
  }
  
  Wire.beginTransmission(CONTROL_ADD_RIGHT);
  if (Wire.endTransmission()!= 0) {
    Serial.println("RIGHT CONTROL DEVICE FAILED");
    while(1);
  }
  Serial.println("succesfully set up control devices");

  pinMode(KICK_PIN, OUTPUT);
  digitalWrite(KICK_PIN, LOW);
  Serial.println("succesfully set up kicker");
  

  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM FAILED");
    while(1);
  }
 
  getStateEEPROM();
  Serial.println("succesfully set up eeprom");
} 


void offensiveGameMode() {
  if(offensiveGameModeSwitch() ==  strikerGameMode)  striker();
  if(offensiveGameModeSwitch() == dribblerGameMode)  dribbler(); 
}

void defensiveGameMode() {
  if(defensiveGameModeSwitch() == defenderGameMode) { led(middle, 1, white); defender(); }
  if(defensiveGameModeSwitch() == goalieGameMode)   { led(middle, 1, off);   goalie(); }
}

void act() {
  if((gameMode == dribblerGameMode || gameMode == strikerGameMode) && gotBall && line_seen && goal_seen && abs(goalDir) < 3) kick();
  stayInBounce();
  
  if(!firstGreen && !line_seen) {
    line_timer = 0;
    firstGreen = true;
  }
  
  if(line_timer > 50 && firstGreen) gotJustBack = false;
  if(gotJustBack) { fahre(lastDir,lastSpd,3*goalDir); return; }
  
  switch (gameMode) {
    case strikerGameMode:  offensiveGameMode(); break;
    case dribblerGameMode: offensiveGameMode(); break;
    case defenderGameMode: defensiveGameMode(); break;
    case goalieGameMode:   defensiveGameMode(); break;
    default: break;
  }
}


bool debug_doOnlyOnce = true;

void loop() {
  if(debug_doOnlyOnce) {
    //[ debug tests that should only be done once here  ]
    debug_doOnlyOnce = false;
  }
  
  getData();
  controlInterface();
  sendDataBt(20);
  writeEEPROM();
  if(play)  act();
  if(debug) debugOutput(100);
  
  
  delay(2);
}
