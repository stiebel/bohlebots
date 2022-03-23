/*
 * Alle Funktionen
 * 
 */
void fahre (int r, int g, int d)
  {
      can_richtung        = r;
      can_geschwindigkeit = g;
      can_drehung         = d;
      //leere Klammern weil es sonst nicht kompaliert
      //hier fahre-logik und -rechnung reinschreiben
  }

  void kick()
{
  if (kick_totzeit>1000)
  {
    digitalWrite(KICKER, HIGH); 
    delay(40);
    kick_totzeit=0;
  }
  digitalWrite(KICKER, LOW); 
}

void i2c_taster_init()
{
  Wire.beginTransmission(I2C_TASTER); 
  Wire.write(255);          // Taster auf Input schalten        
  Wire.endTransmission(); 
}

byte taster_lesen()
{
  Wire.requestFrom(I2C_TASTER, 1);
  if(Wire.available()) 
    {
       return 255-Wire.read();
    }
  else return 255;
}

void i2c_led_setzen(byte wert)
{
  Wire.beginTransmission(I2C_LED); // LEDs alle einmal an
  Wire.write(255-wert);                
  Wire.endTransmission();
}

boolean lichtschranke_lesen()
{
  Serial.println("licht: "+String(analogRead(LICHTSCHRANKE)));
  if (analogRead(LICHTSCHRANKE)  >500)  return false;
  else
  {
    ledbyte += 24;
    return true;
  }
  
}
