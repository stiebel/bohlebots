void schreibe()
{
  /*for (int lauf= 0; lauf < 14; lauf++)
  {
    //sprintf(s, "%4d", wert[lauf]);
    Serial.print (wert[lauf]); 
    Serial.print("\t");
  }*/                                           
  
  Serial.println();Serial.println();
  Serial.println(" Ballrichtung, Zone");
  Serial.print(ballrichtung/16-7);       // as expected by master
  Serial.print(" , ");
  //Serial.print(ballmax); 
  //Serial.print(" , ");
  Serial.println(zone); 
  Serial.flush();

}

void canbus()     
{
  if (true) //  if (canbus fragt bei mir(id=3) an ) 
  {
    ballrichtung = 0;
    ballmax       = 0;
    zone          = 0;
  
    if (ballmedian.getStatus()     == ballmedian.OK)     { ballmedian.getMedian(ballrichtung);   }
    if (balldistmedian.getStatus() == balldistmedian.OK) { balldistmedian.getMedian(ballmax);     }

    if       (ballmax > 450 ) { zone = 2;}  // ultrakrass nah                       Diese Grenzen müssen angepasst werden !!!
    else if  (ballmax > 150 ) { zone = 1;}  // normal
    else                      { zone = 0;}  // ich seh nichts...

    can_richtung = zone;
    if(zone != 0)
    {
      ballrichtung = ballrichtung+7;                              //damit es keine negativen werte gibt: 0..15
      ballrichtung = ballrichtung*16;       //shift um +4 bit
      can_richtung = ballrichtung+zone;     // Byte: r r r r 0 0 z z
    }
  
  }
}

void checkCanBus() 
{
  int packetSize = CAN.parsePacket();
  //Serial.println("start check"); 
  if (packetSize)
  {
    //Serial.println("es ist etwas da");
    int SensorId = CAN.packetId();
    if(SensorId == 0x03)
    {
     //Serial.println("es ist für mich");
      if (CAN.packetRtr()) 
      {
        //Serial.println("es ist eine Anfrage");
        canbus();
        //schreibe();
        CAN.beginPacket(0x03);  //Schicke an Hauptplatine
        CAN.write(can_richtung);
        CAN.endPacket();
      } 
    }
  }
}

void onReceive(int packetSize)
{
  checkCanBus();
}


void setup()

{                
 
  Serial.begin(115200);
  for (int lauf = 0; lauf<14; lauf++)
    pinMode(tssp_pin[lauf],INPUT);
  if (!CAN.begin(500E3)) {Serial.println("CAN failed!"); while (1);}
  Serial.println("Setup erfolgreich");
  CAN.filter(0x03);
  //CAN.onReceive(onReceive);
}  // end of setup()



void tssp_init()
{
  for (int lauf= 0; lauf < 14; lauf++)
  {
    wert[lauf] = 0; 
  }

}  


void tssp_read() // liest alle 32 Sensorwerte aus und speicher sie im Puffer ab.
{
  zeit = 0;
  while (zeit < 4)
  for (int lauf = 0 ; lauf < 14 ; lauf++)
  {
    wert[lauf] += 1-digitalRead(tssp_pin[lauf]);                         
  }


  
}

void finde_max()
{
  int richtungskorrektur;
  wertmax=-1;
  richtungmax=-1;
  for (int lauf = 0; lauf < 14; lauf++)
  {
    if (wert[lauf] > wertmax)
    {
      wertmax = wert[lauf];
      richtungmax=lauf;
    }
  }

  ballrichtung=0;
  
  if (richtungmax ==  0) ballrichtung = 0;
  if (richtungmax ==  1) ballrichtung = 0;
  if (richtungmax ==  2) ballrichtung = 1;
  if (richtungmax ==  3) ballrichtung = 2;
  if (richtungmax ==  4) ballrichtung = 3;
  if (richtungmax ==  5) ballrichtung = 4;
  if (richtungmax ==  6) 
    {
      if (wert[5] > wert[7])  richtungskorrektur=-1;
      else                  richtungskorrektur=1;
      if (wert[6+richtungskorrektur] > wert[6] *8/10)  ballrichtung = 6+ richtungskorrektur;
      else                                            ballrichtung = 6;
    }
  if (richtungmax ==  7) ballrichtung = 8;
  if (richtungmax ==  8) 
    {
      if (wert[9] > wert[7])  richtungskorrektur=1;
      else                  richtungskorrektur=-1;
      if (wert[8+richtungskorrektur] > wert[8] *8/10)  ballrichtung = -6+ richtungskorrektur;
      else                                            ballrichtung = -6;
    }
  if (richtungmax ==  9) ballrichtung = -4;
  if (richtungmax == 10) ballrichtung = -3;
  if (richtungmax == 11) ballrichtung = -2;
  if (richtungmax == 12) ballrichtung = -1;
  if (richtungmax == 13) ballrichtung = 0;
  
  ballmedian.add(ballrichtung);
  balldistmedian.add(wertmax);
}




void messen() 
{
    tssp_init();
    tssp_read();     
    finde_max(); 
}
