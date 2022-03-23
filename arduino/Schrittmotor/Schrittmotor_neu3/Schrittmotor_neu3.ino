const int motorPin1 = 15;  // Blue   - In 1
const int motorPin2 = 5;  // Pink   - In 2
const int motorPin3 = 18; // Yellow - In 3
const int motorPin4 = 19; // Orange - In 4
                          // Red    - pin 5 (VCC)

// Die Untersetzung ist 1 zu 63,6839
// 1019 Umdrehungen für Exakt 90 Grad. Das ist Kacke, das sind 127,375 Durchgänge

unsigned durch=1019;

unsigned int startposition=0;
unsigned int step_Position=0;

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
}


void loop()
{
   rechts90();
   rechts90();
   rechts90();
   links90();
   links90();
   delay(1000);
}

void rechts90()
{
  // müssen in Summe 1019 werden
  
  for (int lauf = 0; lauf<400 ; lauf++) rechtsrum(700-lauf);
  for (int lauf = 0; lauf<219 ; lauf++) rechtsrum(300);
  for (int lauf = 0; lauf<400 ; lauf++) rechtsrum(300+lauf);
  stop();
  delay(50);
  
}

void links90()
{
  // müssen in Summe 1019 werden
  
  for (int lauf = 0; lauf<400 ; lauf++) linksrum(700-lauf);
  for (int lauf = 0; lauf<219 ; lauf++) linksrum(300);
  for (int lauf = 0; lauf<400 ; lauf++) linksrum(300+lauf);
  stop();
  delay(50);
}

void rechtsrum(unsigned int motorSpeed)
{ 
  step_Position=(step_Position+1)%8;
  if (step_Position==0) { digitalWrite(motorPin4, HIGH);  digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, LOW); digitalWrite(motorPin1, LOW);   }
  if (step_Position==1) { digitalWrite(motorPin4, HIGH);  digitalWrite(motorPin3, HIGH);  digitalWrite(motorPin2, LOW); digitalWrite(motorPin1, LOW);   }
  if (step_Position==2) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, HIGH);  digitalWrite(motorPin2, LOW);   digitalWrite(motorPin1, LOW);   }
  if (step_Position==3) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, HIGH);  digitalWrite(motorPin2, HIGH);  digitalWrite(motorPin1, LOW);   }
  if (step_Position==4) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, HIGH);  digitalWrite(motorPin1, LOW);   }
  if (step_Position==5) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, HIGH);  digitalWrite(motorPin1, HIGH);  }
  if (step_Position==6) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, LOW);   digitalWrite(motorPin1, HIGH);  }
  if (step_Position==7) { digitalWrite(motorPin4, HIGH);digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, LOW);   digitalWrite(motorPin1, HIGH);  }
  delayMicroseconds(motorSpeed);
}

void linksrum(unsigned int motorSpeed)
{ 
  step_Position=(step_Position-1)%8;
  if (step_Position==0) { digitalWrite(motorPin4, HIGH);  digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, LOW); digitalWrite(motorPin1, LOW);   }
  if (step_Position==1) { digitalWrite(motorPin4, HIGH);  digitalWrite(motorPin3, HIGH);  digitalWrite(motorPin2, LOW); digitalWrite(motorPin1, LOW);   }
  if (step_Position==2) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, HIGH);  digitalWrite(motorPin2, LOW);   digitalWrite(motorPin1, LOW);   }
  if (step_Position==3) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, HIGH);  digitalWrite(motorPin2, HIGH);  digitalWrite(motorPin1, LOW);   }
  if (step_Position==4) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, HIGH);  digitalWrite(motorPin1, LOW);   }
  if (step_Position==5) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, HIGH);  digitalWrite(motorPin1, HIGH);  }
  if (step_Position==6) { digitalWrite(motorPin4, LOW); digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, LOW);   digitalWrite(motorPin1, HIGH);  }
  if (step_Position==7) { digitalWrite(motorPin4, HIGH);digitalWrite(motorPin3, LOW);   digitalWrite(motorPin2, LOW);   digitalWrite(motorPin1, HIGH);  }
  delayMicroseconds(motorSpeed);
}


void stop()
{ digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, LOW);
}
