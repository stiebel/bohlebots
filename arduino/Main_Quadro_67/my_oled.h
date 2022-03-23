/*
 * Alle Oled-Funktionen für SSD1306 OLED Display (128x64)
 * 
 * public:
 * 
 * void oled()
 * 
 * benötigt:
 * tor_richtung
 * tor_breite
 * linie
 * tor_entfernung
 * pos (Ballpos)
 * kompass
 */

#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

SSD1306  display(0x3c, 21, 22);

void drawArea()
{
  //display.drawLine(0, 15, DISPLAY_WIDTH-1, 15);
  display.drawRect(0, 0, 127, 15);
  display.drawLine(50, 16, 50, 63);
  //display.drawRect(0, 16, 48, 48);
}

void drawTor(int center, int width)
{
  display.fillRect(64+center*20-width/2, 2, width, 11);
}

void drawBot()
{
  display.drawCircle(24, 24+16, 17);
}

void drawBall(int pos)
{
  float w = 3.1415 * pos/8;
  int xpos =24+22*sin(w);
  int ypos =16+24-22*cos(w);
  display.fillCircle(xpos, ypos, 3);
}

void drawKompass(int pos)
{
  float w = 3.1415 * pos/180;
  int x1 =    24+8*sin(w)+0.5;
  int y1 =16+ 24-8*cos(w)+0.5;
  int x2 =    24-8*sin(w)+0.5;
  int y2 =16+ 24+8*cos(w)+0.5;
  display.drawLine(x1,y1,x2,y2);
  
  x2 =    24+4*cos(w)+0.5;
  y2 =16+ 24+4*sin(w)+0.5;
  display.drawLine(x1,y1,x2,y2);
  
  x2 =    24-4*cos(w)+0.5;
  y2 =16+ 24-4*sin(w)+0.5;
  
  display.drawLine(x1,y1,x2,y2);
  
}

void drawLinie(int pos)
{
  float w = 3.1415 * pos/8;
  int x1 =    24+12*sin(w)-20*cos(w)+0.5;
  int y1 =16+ 24-12*cos(w)-20*sin(w)+0.5;
  int x2 =    24+12*sin(w)+20*cos(w)+0.5;
  int y2 =16+ 24-12*cos(w)+20*sin(w)+0.5;
  
  display.drawLine(x1,y1,x2,y2);
}

void drawBatt(int b)
{
  display.drawProgressBar(55, 16, 70, 10, b);
}

void drawTorEntf(int entf)
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(80, 26, "Quadro");
  display.setFont(ArialMT_Plain_16);
  display.drawString(80, 48, String(entf));
}

void drawPower(int p)
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(90, 48, String(p) + "%");
}

void drawstat(int s)
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  String statstr="TW";
  if (s == 1) statstr="St";
  display.drawString(90, 26, statstr);
}

void oled_init()
{
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

void oled() 
{
  int linie=boden_richtung;
  int pos = ball_richtung;  
  display.clear();
  drawArea();
  drawTor(tor_richtung/8,tor_breite/4);
  drawBot();
  unsigned long oled_boden = boden_zeit/10;
  if (oled_boden > 100) oled_boden=100;
  drawBatt(oled_boden);
  //drawPower(80-2*linie);
  //drawstat((linie+10)%2);
  drawTorEntf(tor_entfernung);
  if(ball_max>0) drawBall(pos);
  drawKompass(kompass);
  if (boden_vorhanden) drawLinie(linie);
  display.display();
}
