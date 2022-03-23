/*
 * Alle pixy-Funktionen
 * 
 * public :
 *
 *void pixy_read()
 *
 */

void pixy_auswerten()   // wird nur aufgerufen, wenn die Pixy überhaupt etwas sieht
{     
  int my_signature=1;  // wir spielen immer auf Signatur 1                
  int sieht_farbe=pixy.ccc.blocks[0].m_signature;
 
  if (sieht_farbe == my_signature)
  {
    tor_richtung   = -(pixy.ccc.blocks[0].m_x - 158)/2;
    tor_breite     =  pixy.ccc.blocks[0].m_width; 
    int tor_hoehe   = pixy.ccc.blocks[0].m_height; 
    //tor_entfernung_roh =  pixy.ccc.blocks[0].m_y-80;
    tor_entfernung_roh =  pixy.ccc.blocks[0].m_y;
    tor_entfernung = (tor_entfernung_roh-tor_hoehe)/4;//-abs(tor_richtung)/10;
    if (tor_entfernung < 0) tor_entfernung = 0;
    if (tor_entfernung > 63) tor_entfernung = 63;
   
  
    
    
    
    
    
    //tor_entfernung = (tor_entfernung*tor_entfernung*tor_entfernung)/10000;
    //tor_entfernung = (tor_entfernung*tor_entfernung)/200; // werte hoch 6 geteilt durch 1000000

   
  }
  else                  // soll hier überhaupt etwas geschehen?
  {                     
     tor_richtung  =  kompass ;
     Wire.beginTransmission(0x27); 
      Wire.write(255-2);                
      Wire.endTransmission();
    //sieht_tor=LOW;
    //topspeed=lowtop;
    //seekspeed=lowtop;
  }
  //pixyzeit=0;
 
}

void pixy_read()
  {
     int i; 
    // grab blocks!
    pixy.ccc.getBlocks();
    
    // If there are detect blocks, print them!
    if (pixy.ccc.numBlocks)
    {
     pixy_auswerten() ; 
     hat_tor =1;
    }
    else  
    {
      tor_richtung  =  kompass ;
      ledbyte += 2;       // ledbyte um 2 erhöhen, wenn keine Pixy-Wert;
      hat_tor=0;
    }
    
  } 
