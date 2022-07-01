#include <EEPROM.h>
byte eepromByte[6] = {0,0,0,0,0,0};
#define EEPROM_SIZE 64

void readEEPROM() {
  
  //Serial.println("EEPROM read:");
  for (int i = 0; i < 6; i++) {
    eepromByte[i]=(byte(EEPROM.read(i)));
    //Serial.print(String(eepromByte[i])+", ");
  }
  //Serial.println();
}

void writeEEPROM() {
  //Serial.println("writeEEPROM");

  bool need2write = false;
  if(eepromByte[0] != play)                  { eepromByte[0] = play;                  need2write = true; }
  if(eepromByte[1] != gameMode)              { eepromByte[1] = gameMode;              need2write = true; }
  if(eepromByte[2] != dynamicGameModeSwitch) { eepromByte[2] = dynamicGameModeSwitch; need2write = true; }
  if(eepromByte[3] != strikingGoalLabel)     { eepromByte[3] = strikingGoalLabel;     need2write = true; }
  if(eepromByte[4] != cmps_head/256)         { eepromByte[4] = cmps_head/256;         need2write = true; }
  if(eepromByte[5] != cmps_head%256)         { eepromByte[5] = cmps_head%256;         need2write = true; }

  if(need2write) {
    for (int i = 0; i < 6; i++) 
      EEPROM.write(i, eepromByte[i]); 
    EEPROM.commit();
  }
}

void getStateEEPROM() {
  readEEPROM();
  play                  = eepromByte[0];
  gameMode              = GameMode(int(eepromByte[1]));
  dynamicGameModeSwitch = eepromByte[2];
  strikingGoalLabel     = eepromByte[3];
  cmps_head             = (eepromByte[4]*256) + eepromByte[5];
}
