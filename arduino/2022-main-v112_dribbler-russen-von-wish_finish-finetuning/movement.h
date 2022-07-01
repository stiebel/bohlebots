
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        HELPER        //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int side(const int& i) {
  if(i%8 == 0)  return  0;
  if(i > 0)     return  1;
  return -1;
}

int bound_direction(int d) {
  while(d >  8) d -= 16;
  while(d < -7) d += 16;
  return d;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   DIRECTION CALCS    //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int dirOfObj(const Complex& obj) {
  double d = ((obj.phi*180)/PI)/22.5;
  int di = 0;
  if(d>0) di = int(d+0.5);
  if(d<0) di = int(d-0.5);
  return bound_direction(di);
}

int dirOfDeg(const int& angle) {
  double d = angle/22.5;
  int di = 0;
  if(d>0) di = int(d+0.5);
  if(d<0) di = int(d-0.5);
  return bound_direction(di);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KICK AND FAHRE TYPES //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fahre_latest() {
  int d = lastDir+7;
  int s = lastSpd;
  int r = lastRot+100;
  
  CAN.beginPacket(0x10);
  CAN.write(d);
  CAN.write(s);
  CAN.write(r);
  CAN.endPacket(); 
}


void fahre_real(const int& dir, const int& spd, const int& rot) {
  int d = bound_direction(dir)+7;
  int s = min(spd, SPEED_TRAP);
  int r = rot+100;
   
  CAN.beginPacket(0x10);
  CAN.write(d);
  CAN.write(s);
  CAN.write(r);
  CAN.endPacket(); 

  lastDir = dir;
  lastSpd = spd;
  lastRot = rot;
}


void fahre_no_latest_overwrite(const int& dir, const int& spd, const int& rot) {
  int d = bound_direction(dir)+7;
  int s = min(spd, SPEED_TRAP);
  int r = rot+100; 

  CAN.beginPacket(0x10);
  CAN.write(d);
  CAN.write(s);
  CAN.write(r);
  CAN.endPacket();
}


void fahre(const int& dir, const int& spd, const int& rot) {
  int d = bound_direction(dir)+7;
  int s = min(spd, SPEED_TRAP);
  int r = rot+100; 
  
  CAN.beginPacket(0x10);
  CAN.write(d);
  CAN.write(s);
  CAN.write(r);
  CAN.endPacket(); 
  

  lastDir = dir;
  lastSpd = spd;
  lastRot = rot;
}

int maxSaveSpeed(const int& dir, const int& rot) {
  if(!midpoint_valid) return MAX_SPEED_AT_LINE;

  
  int diff_dir_midpoint = bound_direction(dir-midpointDir);
  if(abs(diff_dir_midpoint) > 4) return MAX_SPEED_AT_LINE;
  
  double dir_angle = (dir*22.5*PI)/180;
  
  Complex d = Complex(1,0);
  d.rotate(dir_angle);
  Complex m = midpoint / midpoint.rad;

  Complex v = d+m;
  double midpointDir_factor = v.rad/2;


  double ballRad_factor = 1;
  int diff_dir_ball = bound_direction(dir-ballDir);
  if(abs(diff_dir_ball) < 3) ballRad_factor = 1-(ball.rad/MAX_REAL_MIDPOINT_RAD);
  
  return MAX_SPEED_AT_LINE+(ballRad_factor*midpointDir_factor*(SPEED_TRAP-rot-MAX_SPEED_AT_LINE));
} 

double calc_slideFactor_prop() {
  const double& t = acceleration_time;
  const double  p = (t-MIN_ACCELERATION_TIME)/(MAX_ACCELERATION_TIME-MIN_ACCELERATION_TIME);
  
  const double  minF = MIN_SLIDE_PROP_FACTOR;
  const double  maxF = MAX_SLIDE_PROP_FACTOR;
  const double  dF   = MAX_SLIDE_PROP_FACTOR - MIN_SLIDE_PROP_FACTOR;
  
  return min(minF+(p*dF),maxF);
}

bool slideIn(const int& dir, const int&  spd, const int& rot) {
  if(dir == lastDir) { 
    acceleration_time = -1; 
    return false; 
  }
  if(acceleration_timer < MIN_ACCELERATION_TIME) { 
    acceleration_timer = 0; 
    acceleration_time = -1; 
    return false; 
  }
  
  if(acceleration_time < 0) {
      acceleration_time = acceleration_timer;
      break_timer = 0;
  }
  acceleration_time = 0;
  
  
  //creating complex number from old direction that we are sliding in now
  Complex dir_slide(1,0);
  dir_slide.stretch(double(lastSpd));
  dir_slide.rotate((lastDir*22.5*PI)/180);

  //creating complex number from old direction that we want to slide into now
  Complex dir_want(1,0);
  dir_want.stretch(double(spd));
  dir_want.rotate((dir*22.5*PI)/180);

  //calculating a factor in [0;1] that is anti-proportional to the time we already slided
  double slideFactor_time = 1.0;
  if(break_timer > 0) slideFactor_time = 1.0/double(break_timer);
  double slideFactor_dir = calc_slideFactor_prop() * slideFactor_time;

  //approaching dir we wanna drive into from direction we started sliding in
  Complex dir_compensation = dir_want - (dir_slide * slideFactor_dir);
  int d = dirOfObj(dir_compensation);
     
  //approaching spd we wanna drive in from spd we started sliding in
  double slideFactor_spd = min(1.0, slideFactor_dir);
  int s = double(spd) + (double(lastSpd-spd)*slideFactor_spd);

  //approaching rot we wanna drive in from rot we started sliding in
  int r = double(rot) + (double(lastRot-rot)*slideFactor_spd);
  //continue sliding into dir until d == dir
  
  if(d != dir) { fahre_no_latest_overwrite(d,s,r); return true; }
  return false;
}

void fahre_save(const int& dir, const int& spd, const int& rot) {
  int s = min(spd, maxSaveSpeed(dir,rot));
  if(slideIn(dir,s,rot)) return;
  fahre(dir,s,rot);
}

void fahre_save_rot(const int& dir, const int& spd, const int& rot) {
  int s = min(spd, maxSaveSpeed(dir,rot));
  int r = min(rot, rot*s/spd);
  if(slideIn(dir,s,r)) return;
  fahre(dir,s,r);
}

void kick() {
  if(kick_lockdown_timer > 1000) {
    if(!line_seen || (line_seen && abs(lineDir) > 2) )fahre_save(0,100,0);
    digitalWrite(KICK_PIN, HIGH);
    delay(KICK_DELAY);
    digitalWrite(KICK_PIN, LOW);
    kick_lockdown_timer = 0;
  }
}
