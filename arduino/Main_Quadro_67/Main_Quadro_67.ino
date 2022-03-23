#include "my_header.h"

boolean matrixcheck(int linie, int ball)
{
  if (matrix[linie+7][ball+7] == 1) return true;
  else return false;
}


int bt_speed(int ball_a, int ball_b, int nor_sp)
{
  if (bt_available == 1)
  {
    if(((hat_tor + hat_tor_b) == 2) && ((ball_max + ball_max_b) >=2))
    {
      if ((abs(ball_a)>=5)&&(abs(ball_b)>=5))
      {
        if (tor_entfernung<tor_entfernung_b)                                                 bt_tempo = nor_sp   ;
        if (tor_entfernung>=tor_entfernung_b)                                                bt_tempo = nor_sp+15;       
      }
      if ((ball_a == ball_b)&&((abs(ball_a)<=4)&&(abs(ball_b)<=4)))
      {
        if (tor_entfernung<tor_entfernung_b)                                                 bt_tempo = nor_sp+15;
        if (tor_entfernung>=tor_entfernung_b)                                                bt_tempo = nor_sp   ;    
      }
      if ((abs(ball_a)<abs(ball_b))&&(abs(ball_a)<((abs(ball_a-ball_b)))))                   bt_tempo = nor_sp+15;
      if (abs(ball_a)>abs(ball_b))                                                           bt_tempo = nor_sp   ;
    }
    else bt_tempo = nor_sp;
  }
  else 
  {
    if ((abs(ball_a)>4)&&tor_entfernung<35)       bt_tempo = nor_sp+15;
    else                                          bt_tempo = nor_sp;
  }
  
  return bt_tempo;
}   


void bt_read()
{
  if (Serial2.available()) 
  {
   bt_available = 1;
   while(Serial2.available()) 
    {
      bt_angekommen = Serial2.read();
    
      if (bt_angekommen%2==0)
      {
        Serial.println("ir_ist_da");
        irpaket_b = bt_angekommen;
        ball_richtung_b = (irpaket_b/16)-7;    
        ball_max_b = irpaket_b%16;  
        if ((irpaket_b & 8)==8) st=1;
      }
      if (bt_angekommen%2==1)
      {
        Serial.println("tor_ist_da");
        torpaket_b = bt_angekommen;
        tor_entfernung_b = torpaket_b/4;
        hat_tor_b = torpaket_b%4-1;
      }
    }
  }
  else
  {
    bt_available= 0;
    Serial.println("bt_failed");
  }
  
}
void bt_write()
{
  bt_data_1=(ball_richtung+7)*16+ball_max*2;              //durch x2   ist der letzte bit immer 0
  if (tastbyte == 130) bt_data_1=bt_data_1+8;             // 8, wenn startknopf
  bt_data_2=tor_entfernung*4+hat_tor*2+1;                 //durch x2+1 ist der letzte bit immer 1
  
 
  Serial2.write(bt_data_1);
  Serial2.write(bt_data_2);
}
void drehtest()
{
  if(ball_max <1) fahre(0,0,0);
  else            fahre(1,85,-50);
}

void weisse_linie()
{
  boden_zeit_hilfe =1;  // ich sehe weiß
  gruen_zeit=0;
  r_o_l_aenderbar =1;
  if (boden_zeit< 1000)     hektik =30;
  else                      hektik = 0;
  
    
  if( boden_klar ==1)
  {
    if (boden_abweichung==0)      
    {
      if(matrixcheck(boden_richtung,ball_richtung) == 1)
      {
        if(boden_richtung== 0)     fahre( 8,80,kompass/4); 
        if(boden_richtung==-7)     fahre( 0,80,kompass/4);
        if(boden_richtung==-6)     fahre( 2,80,kompass/4);
        if(boden_richtung==-5)     fahre( 4-boden_korektur,80,kompass/4);
        if(boden_richtung==-4)     fahre( 4-boden_korektur,80,kompass/4);
        if(boden_richtung==-3)     fahre( 4-boden_korektur,80,kompass/4);
        if(boden_richtung==-2)     fahre( 6,80,kompass/4);
        if(boden_richtung==-1)     fahre( 8,80,kompass/4);
        if(boden_richtung== 1)     fahre( 8,80,kompass/4);
        if(boden_richtung== 2)     fahre(-6,80,kompass/4);
        if(boden_richtung== 3)     fahre(-4+boden_korektur,80,kompass/4);
        if(boden_richtung== 4)     fahre(-4+boden_korektur,80,kompass/4);
        if(boden_richtung== 5)     fahre(-4+boden_korektur,80,kompass/4);
        if(boden_richtung== 6)     fahre(-2,80,kompass/4);
        if(boden_richtung== 7)     fahre( 0,80,kompass/4);
        if(boden_richtung== 8)     fahre( 0,80,kompass/4);
      }
      else 
      {
        if (boden_zeit< 1000)    // muss noch angepasst werden. 1000 erscheint zu hoch
        {
          fahre( 0,0,kompass/5);     
          boden_zeit =0;     
        }
        else
        {
          if(boden_richtung== 0)     fahre( 8,80,tor_richtung/4); 
          if(boden_richtung==-7)     fahre( 0,80,tor_richtung/4);
          if(boden_richtung==-6)     fahre( 2,80,tor_richtung/4);
          if(boden_richtung==-5)     fahre( 4-boden_korektur,80,tor_richtung/4);
          if(boden_richtung==-4)     fahre( 4-boden_korektur,80,tor_richtung/4);
          if(boden_richtung==-3)     fahre( 4-boden_korektur,80,tor_richtung/4);
          if(boden_richtung==-2)     fahre( 6,80,tor_richtung/4);
          if(boden_richtung==-1)     fahre( 8,80,tor_richtung/4);
          if(boden_richtung== 1)     fahre( 8,80,tor_richtung/4);
          if(boden_richtung== 2)     fahre(-6,80,tor_richtung/4);
          if(boden_richtung== 3)     fahre(-4+boden_korektur,80,tor_richtung/4);
          if(boden_richtung== 4)     fahre(-4+boden_korektur,80,tor_richtung/4);
          if(boden_richtung== 5)     fahre(-4+boden_korektur,80,tor_richtung/4);
          if(boden_richtung== 6)     fahre(-2,80,tor_richtung/4);
          if(boden_richtung== 7)     fahre( 0,80,tor_richtung/4);
          if(boden_richtung== 8)     fahre( 0,80,tor_richtung/4);
        }
      }
    }
     else
     {
      if(boden_richtung== 0)     fahre( 8,90-hektik, kompass/3);
      if(boden_richtung==-7)     fahre( 0,90-hektik, kompass/3);
      if(boden_richtung==-6)     fahre( 2,90-hektik, kompass/3);
      if(boden_richtung==-5)     fahre( 4-boden_korektur,90-hektik, kompass/3);
      if(boden_richtung==-4)     fahre( 4-boden_korektur,90-hektik, kompass/3);
      if(boden_richtung==-3)     fahre( 4-boden_korektur,90-hektik, kompass/3);
      if(boden_richtung==-2)     fahre( 6,90-hektik, kompass/3);
      if(boden_richtung==-1)     fahre( 8,90-hektik, kompass/3);
      if(boden_richtung== 1)     fahre( 8,90-hektik, kompass/3);
      if(boden_richtung== 2)     fahre(-6,90-hektik, kompass/3);
      if(boden_richtung== 3)     fahre(-4+boden_korektur,90-hektik, kompass/3);
      if(boden_richtung== 4)     fahre(-4+boden_korektur,90-hektik, kompass/3);
      if(boden_richtung== 5)     fahre(-4+boden_korektur,90-hektik, kompass/3);
      if(boden_richtung== 6)     fahre(-2,90-hektik, kompass/3);
      if(boden_richtung== 7)     fahre( 0,90-hektik, kompass/3);
      if(boden_richtung== 8)     fahre( 0,90-hektik, kompass/3);
     }
    }
    else
    {
      fahre(0,0,kompass/4);  
    }
 }

void normalfahrt()
{
  if (ball_richtung == 0)
  {
    if  ((hat_tor==1)&&(abs(tor_richtung<10)))       fahre( 0,   bt_speed(ball_richtung,ball_richtung_b,grund_sp  ), tor_richtung/3);
    else
    {                              
      if ((hat_tor==1)&&(abs(tor_richtung>9)))       fahre( 0,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ), tor_richtung/2);
                                    
      else                                           fahre( 0,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ), tor_richtung/4);    //pixy sieht nichts
    }
                                                                  
  }   
  else
  { 
      if ((bt_available>0)&&(abs(ball_richtung_b <2))&&(abs(ball_richtung <7))&&abs(ball_richtung >2))
      {
        if(tor_richtung<12)       fahre(8,40, tor_richtung/3);
        else                      fahre(0, 0, tor_richtung/3);                                                
      }
    else
    {
      if (ball_richtung== 1)    fahre( 2,   bt_speed(ball_richtung,ball_richtung_b,grund_sp-10),  tor_richtung/5);
      if (ball_richtung==-1)    fahre(-2,   bt_speed(ball_richtung,ball_richtung_b,grund_sp-10),  tor_richtung/5);
      if (ball_richtung== 2)    fahre( 3,   bt_speed(ball_richtung,ball_richtung_b,grund_sp-10),  tor_richtung/5);
      if (ball_richtung==-2)    fahre(-3,   bt_speed(ball_richtung,ball_richtung_b,grund_sp-10),  tor_richtung/5);
      if (ball_richtung== 3)    fahre( 5,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/4);   
      if (ball_richtung==-3)    fahre(-5,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/4);   
      if (ball_richtung== 4)    fahre( 6,   bt_speed(ball_richtung,ball_richtung_b,grund_sp-10),  tor_richtung/4);   
      if (ball_richtung==-4)    fahre(-6,   bt_speed(ball_richtung,ball_richtung_b,grund_sp-10),  tor_richtung/4);   
      if (ball_richtung== 5)    fahre( 7,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/3);   
      if (ball_richtung==-5)    fahre(-7,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/3);   
      if (ball_richtung== 6)    fahre( 8,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/3);   
      if (ball_richtung==-6)    fahre( 8,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/3);   
      if (ball_richtung== 7)    fahre(-7,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/3);     
      if (ball_richtung==-7)    fahre( 7,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/3);   
      if (ball_richtung== 8)      
      {
        r_o_l_aenderbar =0;
        if (r_oder_l >=0)       fahre( 5,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/5);
        else                    fahre(-5,   bt_speed(ball_richtung,ball_richtung_b,grund_sp   ),  tor_richtung/5);
      }
      else                          r_o_l_aenderbar =1;
    }
  }
}

void schussfahrt()
{
  if (hat_tor==1)
  {
   if((abs(tor_richtung<10))&&(tor_entfernung<25))
   {
    kick();
    fahre(0,0,tor_richtung/4) ;
           
   }
   else       fahre(0,65,tor_richtung/3);
   
  }
  else fahre(0,60,tor_richtung/3);         
            
}

void stuermer()
{  
  if(hat_tor>0)
  {
    if (tor_entfernung>=35)         boden_korektur=2;
    else                          boden_korektur=-2;
  }
  
  if (r_o_l_aenderbar ==1)  r_oder_l =tor_richtung -kompass;
  if (ball_richtung <-4 || ball_richtung >4) gruen_zeit_limit = 300;   // müssen angepasst werden
  else                                    gruen_zeit_limit = 5;
  if (anti_nitro_hilfe >100)              anti_nitro = 100;
  else                                    anti_nitro = 66;
      
  if (boden_vorhanden ==1)  weisse_linie();     
  
  else  // keine weiße Linie
  { 
   if (boden_zeit_hilfe ==1)  //  war bei letzte Messung noch weiß
    {
      boden_zeit =0;
      boden_zeit_hilfe =0;
    }
    
     
       if (gruen_zeit < gruen_zeit_limit) fahre(fahre_echt,50,kompass/4);   // war gerade noch an weißer Linie...
       else
       {
        
          if (ball_max==0)  fahre(0,0,tor_richtung/5);    // Ich sehe den Ball nicht
          else  if (hat_ball ==1) schussfahrt();  
                else normalfahrt();
        } 
    
  }
}     // Ende von stuermer

void loop() 
{
  loop_time=0;
  ledbyte=0;
  tastbyte=taster_lesen();            
  Can();                              
    
  kompass = -kompass_lesen();         
  pixy_read();                        
  hat_ball = lichtschranke_lesen();   
    
  if (abs(kompass) < 10) ledbyte += 128;
  
  if (tastbyte == 6)    {set_heading(); writeEEPROM();}
  if (tastbyte == 1)    {st = 0;        writeEEPROM();}
  if (tastbyte == 130)  {st = 1;        writeEEPROM();} 
  if (tastbyte == 160)  {kick();        writeEEPROM();}
  
  if (st ==1)       stuermer();              
  else            fahre(0,0,0);          
                                      
                                      
  //oled();                            
  i2c_led_setzen(ledbyte);            
  bt_read();
  bt_write();
  //Serial.println(tor_entfernung_roh);
   Serial.println(tor_entfernung);
   //Serial.println(tastbyte);
    
  delay(5);                           
 
 
  
  //Serial.println();
  //Serial.println();
}
