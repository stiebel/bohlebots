 

// 20.03.2022
// Bohlebots
// header für BohleBots Bluetooth

/*
 * Diese Header Datei nutzt den "BluetoothSerial.h"
 * aus den offiziellen Bibliotheken und wrapped ein paar Bohlebots Flags dazu.
 * 
 * Achtung: in der "BluetoothSerial.cpp"
 * müssen folgende Zeilen geändert werden:
 * 
 * 65 und 66:
 * #define READY_TIMEOUT (2 * 1000)     // orig : (10 * 1000)
 * #define SCAN_TIMEOUT (INQ_LEN * 200)  // orig : (INQ_LEN * 2 * 1000)
 *   
 * Dies ermöglicht ein schnelles "new-connect".
 */
#include <elapsedMillis.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

class BohleBlue
{
  public:
    
    boolean isMaster = false;
    boolean connected;
    //Chip IDs 
    uint32_t chipId = 0;
    
    //Hart-kodierte Master/Slave Paare
    // Master IDs 
    uint32_t m1=3115908;
    uint32_t m2=16367580;
    uint32_t m3=0;
    // Slave Adressen:
    uint8_t s1address[6]  = {0x3c, 0x71, 0xbf, 0x53, 0xb6, 0x7a};//3c:71:bf:53:b6:7a
    uint8_t s2address[6]  = {0x80, 0x7d, 0x3a, 0xcf, 0x58, 0xaa};//80:7d:3a:cf:58:aa  
    uint8_t s3address[6]  = {0x80, 0x7d, 0x3a, 0xcf, 0x58, 0xaa};//80:7d:3a:cf:58:aa
    elapsedMillis timeoutBT=0;  
             
    BohleBlue()
    {
      for(int i=0; i<17; i=i+8) chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;         
    }
    
    void init()
    {
      Serial.print("Init : ");
      if ((chipId == m1)|(chipId == m2)|(chipId == m3)) setBTmaster();
      else SerialBT.begin("BohleSlave");
      byte m = modus();
      
      if ((m%10)==1)  Serial.println("ich bin Master");
      else            Serial.println("ich bin Slave");
    }

    byte modus()
    {
      byte m=0;
      if (isMaster) m=1;
      if (SerialBT.connected()) m=m+10;
      return m;
    }

    byte checkBT()
    {
      if (isMaster) if (!SerialBT.connected()) SerialBT.connect();
      else          if (!SerialBT.connected()) SerialBT.begin("BohleSlave");
      
      if (SerialBT.available())
      {
        timeoutBT=0;
        return SerialBT.read();
      }
      if (timeoutBT > 2000) // seit 2 Sekunden kein BT mehr bekommen
      {
        
        SerialBT.disconnect();
        timeoutBT=0;
      }
      return(0);
     }

     void writeBT(byte b)
     {
      if (SerialBT.connected()) SerialBT.write(b);
     }
     
    
    private:
    void setBTmaster()
    {
      isMaster=true;
      Serial.print(" setze Master ... ");
      SerialBT.begin("BohleMaster", true);
      Serial.print(" suche Slave ... ");
      if (chipId == m1) connected = SerialBT.connect(s1address);
      if (chipId == m2) connected = SerialBT.connect(s2address);
      if (chipId == m3) connected = SerialBT.connect(s3address);
      Serial.print(" gefunden ... ");
    }
   

};
