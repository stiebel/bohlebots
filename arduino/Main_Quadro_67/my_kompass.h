/*
 * Alle Kompass-Funktionen für den CMPS11
 * 
 * public :
 * int kompass_lesen()
 * void set_headinh()
 * 
 */

#define CMPS11_ADDRESS 0x60       // cmps11 
#define ANGLE_8  1

int sign(int x)
{
  if (x>0) return 1;
  if (x<0) return -1;
  return 0;
}

int kompass_org()
{
  unsigned char high_byte, low_byte, angle8;
  unsigned int angle16;
  Wire.beginTransmission(CMPS11_ADDRESS);
  Wire.write(ANGLE_8);
  Wire.endTransmission();
  Wire.requestFrom(CMPS11_ADDRESS, 3);
  while(Wire.available() < 3);
  angle8 = Wire.read();               // Read back the 5 bytes
  high_byte = Wire.read();
  low_byte = Wire.read();
  angle16 = high_byte;                 // Calculate 16 bit angle
  angle16 <<= 8;
  angle16 += low_byte;
  return angle16 / 10;
}

int kompass_lesen()
{
  return((((kompass_org()-head)+180+360)%360)-180);
}

void set_heading()
{
  head = kompass_org();
}
