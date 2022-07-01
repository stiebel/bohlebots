
void debug_SerialOutput() {
    Serial.println("//////////////////////////////////////////////");
    Serial.println("Gammode:    "+String(gameMode));
    Serial.println();
    Serial.println("ball: "); ball.debug_println();
    Serial.println("  seen:    "+String(ball_seen));
    Serial.println("  dir:     "+String(ballDir));
    Serial.println("  dist:    "+String(ball.rad/MAX_REAL_MIDPOINT_RAD));
    Serial.println("  cnt:     "+String(ball_unseen_cnt));
    Serial.println();
    Serial.println("goal: "); goal.debug_println();
    Serial.println("  seen:    "+String(goal_seen));
    Serial.println("  dir:     "+String(goalDir));
    Serial.println("  dist:    "+String(goal.rad/MAX_REAL_MIDPOINT_RAD));
    Serial.println("  cnt:     "+String(goal_unseen_cnt));
    Serial.println();
    Serial.println("ownGoal: "); ownGoal.debug_println();
    Serial.println("  seen:    "+String(ownGoal_seen));
    Serial.println("  dir:     "+String(ownGoalDir));
    Serial.println("  dist:    "+String(ownGoal.rad/MAX_REAL_MIDPOINT_RAD));
    Serial.println("  cnt:     "+String(ownGoal_unseen_cnt));
    Serial.println();
    Serial.println("midpoint: "); midpoint.debug_println();
    Serial.println("  valid:   "+String(midpoint_valid));
    Serial.println("  dir:     "+String(midpointDir));
    Serial.println("  dist:    "+String(midpoint.rad/MAX_REAL_MIDPOINT_RAD));
    Serial.println();
    Serial.println("line: ");
    Serial.println("  seen:    "+String(line_seen));
    Serial.println("  dir:     "+String(lineDir));
    Serial.println("  unique:  "+String(line_unique));
    Serial.println("  timer:   "+String(line_timer));
    Serial.println();
    Serial.println("spdFactor:   "+String(last_save_speedFactor));
    Serial.println("dirOffset:   "+String(dirOffset()));
    Serial.println();
    Serial.println("light:       "+String(gotBall));
    Serial.println("cmpsDir:     "+String(cmpsDir));
    Serial.println("cmps_org:    "+String(cmps_read_org()));
    Serial.println("cmps_head:   "+String(cmps_head));
    Serial.println("//////////////////////////////////////////////");
    Serial.println();
}

int debug_cnt = 0;

void debugOutput(const int& n) {
  if (n <= 0) return;
  ++debug_cnt;
  if (debug_cnt >= n) {
    debug_SerialOutput();
    debug_cnt = 0;
  }
}

void debug_slideLogic() {
  elapsedMillis rect = 0;
  while(rect < 4500) {
    Serial.println();
    Serial.println("rect: "+String(rect));
    if(rect >= 2000 && rect < 2500) fahre_save( 0,100,0);
    if(rect >= 2500 && rect < 3000) fahre_save( 4,100,0); 
    if(rect >= 3000 && rect < 3500) fahre_save( 8,100,0);
    if(rect >= 3500 && rect < 4000) fahre_save(-4,100,0);
    if(rect >= 4000 && rect < 4500) fahre_save(0,0,0);
  }
}



void debug_slideIn() {
  lastDir = 0;
  lastSpd = 100;
  lastRot = 0;
  int spd = 90;

  Serial.println("sliding dir "+String(lastDir)+" with spd "+String(lastSpd));
  Serial.println();
  
  for(int dir = 1; dir<=8; ++dir) {
    if(dir == lastDir) continue;
    Serial.println("for-loop: wanna drive in dir "+String(dir)+" with spd "+String(50));
    for(int debug_break_timer = 0; debug_break_timer<500; ++debug_break_timer) {
      //creating complex number from old direction that we are sliding in now
      Complex dir_slide(1,0);
      dir_slide.stretch(double(lastSpd));
      dir_slide.rotate((lastDir*22.5*PI)/180);

      //creating complex number from old direction that we want to slide into now
      Complex dir_want(1,0);
      dir_want.stretch(double(spd));
      dir_want.rotate((dir*22.5*PI)/180);

      //calculating a factor in [0;1] that is anti-proportional to the time we already slided
      double slideFactor_time = 1;
      if(debug_break_timer > 0) slideFactor_time = 1.0/double(debug_break_timer);
      double slideFactor_dir = calc_slideFactor_prop() * slideFactor_time;
    
      //approaching dir we wanna drive into from direction we started sliding in
      Complex dir_compensation = dir_want - (dir_slide * slideFactor_dir);
      int d = dirOfObj(dir_compensation);
      
    
      //approaching spd we wanna drive in from speed we started slidig in
      //double slideFactor_spd = min(1.0, slideFactor_dir);
      //int s = double(spd) + (double(lastSpd-spd)*slideFactor_spd);

      
      //Serial.println("  timer:  "+String(debug_break_timer));
      //Serial.println("  factor: "+String(slideFactor_dir));
      //Serial.println("  dir_slide: "+dir_slide.debug_string());
      //Serial.println("  dir_want:  "+dir_want.debug_string());
      //Serial.println();
      //Serial.println("  dir_compesation:  "+dir_compensation.debug_string());
      //Serial.println();
      //Serial.println("  d: "+String(d));
      //Serial.println("  s: "+String(s));
      //Serial.println();
      //Serial.println();
      //Serial.println();
      
      if(d == dir) {
        Serial.println("  timer:  "+String(debug_break_timer));
        break;
      }
    }
    Serial.println();
  }
}

void debug_i2csync() {
  int led_left = 255-cD_left.led1_colour-cD_left.led2_colour;
  Wire.beginTransmission(CONTROL_ADD_LEFT);
  Wire.write(led_left);
  Wire.endTransmission(); 

  Wire.requestFrom(CONTROL_ADD_LEFT,1);
  if(Wire.available()) {
    int packet = 255 - Wire.read();
    //Serial.println("i2c read: "+String(packet));
    packet = packet % 128;
    cD_left.button2 = (packet > 63);
    packet = packet % 2;
    cD_left.button1 = (packet > 0);
  }   


  int led_middle = 255-cD_middle.led1_colour-cD_middle.led2_colour;
  Wire.beginTransmission(CONTROL_ADD_MIDDLE);
  Wire.write(led_middle);
  Wire.endTransmission(); 

  Wire.requestFrom(CONTROL_ADD_MIDDLE,1);
  if(Wire.available()) {
    int packet = 255 - Wire.read();
    //Serial.println("i2c read: "+String(packet));
    packet = packet % 128;
    cD_middle.button2 = (packet > 63);
    packet = packet % 2;
    cD_middle.button1 = (packet > 0);
  }  


  int led_right = 255-cD_right.led1_colour-cD_right.led2_colour;
  Wire.beginTransmission(CONTROL_ADD_RIGHT);
  Wire.write(led_right);
  Wire.endTransmission(); 

  Wire.requestFrom(CONTROL_ADD_RIGHT,1);
  if(Wire.available()) {
    int packet = 255 - Wire.read();
    //Serial.println("i2c read: "+String(packet));
    packet = packet % 128;
    cD_right.button2 = (packet > 63);
    packet = packet % 2;
    cD_right.button1 = (packet > 0);
  }  
}

int debug_cl1 = 1, debug_cl2 = 2;
int debug_cm1 = 3, debug_cm2 = 4;
int debug_cr1 = 5, debug_cr2 = 6;
elapsedMillis debug_rainbow = 0;
void led_rainbow(const int& t) {
  led(left  ,1,LED_Colour(debug_cl1));
  led(left  ,2,LED_Colour(debug_cl2));
  led(middle,1,LED_Colour(debug_cm1));
  led(middle,2,LED_Colour(debug_cm2));
  led(right ,1,LED_Colour(debug_cr1));
  led(right ,2,LED_Colour(debug_cr2));
  if(debug_rainbow > t) {
    debug_cl1 = (debug_cl1+1 > magenta ? debug_cl1-magenta+1 : debug_cl1+1);
    debug_cl2 = (debug_cl2+1 > magenta ? debug_cl2-magenta+1 : debug_cl2+1);
    debug_cm1 = (debug_cm1+1 > magenta ? debug_cm1-magenta+1 : debug_cm1+1);
    debug_cm2 = (debug_cm2+1 > magenta ? debug_cm2-magenta+1 : debug_cm2+1);
    debug_cr1 = (debug_cr1+1 > magenta ? debug_cr1-magenta+1 : debug_cr1+1);
    debug_cr2 = (debug_cr2+1 > magenta ? debug_cr2-magenta+1 : debug_cr2+1);
    debug_rainbow = 0;
  }
}

void debug_ball_inside() {
  for(int i=-7; i<=8; ++i) {
    Complex debug_ball(1,0);
    debug_ball.rotate((i*22.5*PI)/180);
    //int dir_b = dirOfObj(debug_ball);
    for(int j=-7; j<=8; ++j) {
      ballDir = i;
      lineDir = j;
      int dir_diff = 0;
      if(ballDir+ 8 >= lineDir) dir_diff = ballDir - lineDir;
      else dir_diff = lineDir - ballDir;
      dir_diff = abs(dir_diff);
      //Serial.println("ballDir: "+String(dir_b));
      //Serial.println("lineDir: "+String(dir_l));
      //Serial.println("dir_diff: "+String(dir_diff)+" --> in: "+String((dir_diff > 4 && dir_diff < 12)));
      //Serial.println();
      Serial.print("  "+String((dir_diff > 4 && dir_diff < 12)));
    }
    Serial.println();
    //Serial.println("---------------------------);
  }
}

void debug_goalieInCorner() {
  for(int i=-7; i<=8; ++i) {
   ballDir = i;
    for(int j=-7; j<=8; ++j) {
      ownGoalDir = j;
      int dir_diff = 0;
      //if(ballDir+4 >= goalDir) dir_diff = ballDir - goalDir;
      //else dir_diff = goalDir - ballDir;
      //dir_diff = abs(dir_diff);
      dir_diff = -(ballDir - ownGoalDir);
      Serial.print("  "+String((dir_diff >= 0 && dir_diff < 5))); // rechts
      //Serial.print("  "+String(ballDir - goalDir));
    }
    Serial.println();
  }
}
