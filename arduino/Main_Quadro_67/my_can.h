/*
 * Alle canbus-Funktionen
 * 
 * public : Can()
 * 
 */

void Can()
  {
    elapsedMillis echtzeit=0;
   
    int zeit = 0;
    int bodenpaket;
    digitalWrite(LED,HIGH);CAN.beginPacket(0x03,1,true); // sendet RTR und will 1 byte
    CAN.beginPacket(0x01,1,true); // sendet RTR und will 1 byte
    CAN.endPacket();
      while (!CAN.parsePacket()) 
      {
        delayMicroseconds(1);
        zeit++;  
      }
    
      while (CAN.available()) 
        {
          bodenpaket = CAN.read();                
        }
      
    digitalWrite(LED,LOW);  
   
   
      boolean bit4 =  bodenpaket & 16;
      boolean bit5 =  bodenpaket & 32;
      boolean bit6 =  bodenpaket & 64;
    if(bit6)        { boden_vorhanden=1   ; }        
    else              boden_vorhanden=0   ;   
    
    if(bit5)        { boden_abweichung=1  ; }         
    else              boden_abweichung=0  ; 
    
    if(bit4)        { boden_klar=1        ; }       
    else              boden_klar=0        ; 
     
     
    
    boden_richtung = (bodenpaket%16)-7       ;
    
    zeit = 0;
    int irpaket;
    CAN.beginPacket(0x03,1,true); // sendet RTR und will 1 byte
    if (CAN.endPacket()==1)
    {
     
      while (!CAN.parsePacket()) 
      {
        delayMicroseconds(1);
        zeit++;
        
      }
      
      while (CAN.available()) 
        {
          irpaket=CAN.read();
          ball_richtung=(irpaket/16)-7;    
          int zone=irpaket%16;                       
          if (zone<1)
          {
            ball_max=0;
          }
          else ball_max=1 ;    
          if (ball_max<1)     ball_richtung=0;              
         
        }

     
      //Serial.println("done");
    }
    else Serial.println("IR antwortet nicht");
   
    
    
    CAN.beginPacket(0x10);
    CAN.write(can_richtung+7);     //Hier wird die Richtung gesendet          
    if ((fahre_echt==  can_richtung) && (abs(ball_richtung) >2))
    {
     fahre_echt=  can_richtung   ;        
    }
    else
    {
      anti_nitro_hilfe =0;
      fahre_echt=  can_richtung ;              
    }
    CAN.write(can_geschwindigkeit);        //Hier wird die Geschwindigkeit gesendet
    CAN.write(can_drehung+100);            //Hier wird die Drehung gesendet
    CAN.endPacket(); 
  }
