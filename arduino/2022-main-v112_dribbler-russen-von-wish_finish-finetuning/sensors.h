////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   COLLECTING DATA    //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void getData_ai() {
  while(Serial2.available()) {

    int    ball_x1 = 0,    ball_x2 = 0,    ball_y1 = 0,    ball_y2 = 0;
    int    goal_x1 = 0,    goal_x2 = 0,    goal_y1 = 0,    goal_y2 = 0;
    int ownGoal_x1 = 0, ownGoal_x2 = 0, ownGoal_y1 = 0, ownGoal_y2 = 0;

    cam_timeout = 0;
  
    //delay(1);
    
    String aiString = Serial2.readStringUntil('\n');
    
    int label = aiString.substring(0,1).toInt();
    aiString = aiString.substring(3);

    //ball  
    if(label == 0) {
      ball_x1  = aiString.substring(0,aiString.indexOf(',')).toInt();
      aiString = aiString.substring(aiString.indexOf(' ')+1);
      ball_x2  = aiString.substring(0,aiString.indexOf(',')).toInt();
      aiString = aiString.substring(aiString.indexOf(' ')+1);
      ball_y1  = aiString.substring(0,aiString.indexOf(',')).toInt();
      aiString = aiString.substring(aiString.indexOf(' ')+1);
      ball_y2  = aiString.substring(0,aiString.indexOf(']')).toInt();

      double ball_X    =   ((ball_x1 + ball_x2)/2) - (AI_PIC_WIDTH/2);
      double ball_Y    = -(((ball_y1 + ball_y2)/2) - (AI_PIC_HEIGHT/2));
             ball_size = sqrt(  ( (ball_x2 - ball_x1)*(ball_x2 - ball_x1) ) + ( (ball_y2 - ball_y1)*(ball_y2 - ball_y1) )  );

      ball = Complex(ball_X, ball_Y) + mirrorOffset;
  
      ball_seen = true;
      
      ball_unseen_cnt = 0;
      ++goal_unseen_cnt;
      ++ownGoal_unseen_cnt;
    }
    
    //gegnerisches Tor
    if(label == strikingGoalLabel) {
      goal_x1  = aiString.substring(0,aiString.indexOf(',')).toInt();
      aiString = aiString.substring(aiString.indexOf(' ')+1);
      goal_x2  = aiString.substring(0,aiString.indexOf(',')).toInt();
      aiString = aiString.substring(aiString.indexOf(' ')+1);
      goal_y1  = aiString.substring(0,aiString.indexOf(',')).toInt();
      aiString = aiString.substring(aiString.indexOf(' ')+1);
      goal_y2  = aiString.substring(0,aiString.indexOf(']')).toInt();

      double goal_X    =   ((goal_x1 + goal_x2)/2) - (AI_PIC_WIDTH/2);
      double goal_Y    = -(((goal_y1 + goal_y2)/2) - (AI_PIC_HEIGHT/2));
             goal_size = sqrt(  ( (goal_x2 - goal_x1)*(goal_x2 - goal_x1) ) + ( (goal_y2 - goal_y1)*(goal_y2 - goal_y1) )  );
  
      goal = Complex(goal_X, goal_Y) + mirrorOffset;
      goal_seen = true;
      goal_unseen_cnt = 0;
      ++ball_unseen_cnt;
      ++ownGoal_unseen_cnt;
    }

    //eigenes tor
    if(label != strikingGoalLabel && label > 0) {
      ownGoal_x1  = aiString.substring(0,aiString.indexOf(',')).toInt();
      aiString = aiString.substring(aiString.indexOf(' ')+1);
      ownGoal_x2  = aiString.substring(0,aiString.indexOf(',')).toInt();
      aiString = aiString.substring(aiString.indexOf(' ')+1);
      ownGoal_y1  = aiString.substring(0,aiString.indexOf(',')).toInt();
      aiString = aiString.substring(aiString.indexOf(' ')+1);
      ownGoal_y2  = aiString.substring(0,aiString.indexOf(']')).toInt();

      double ownGoal_X    =   ((ownGoal_x1 + ownGoal_x2)/2) - (AI_PIC_WIDTH/2);
      double ownGoal_Y    = -(((ownGoal_y1 + ownGoal_y2)/2) - (AI_PIC_HEIGHT/2));
             ownGoal_size = sqrt(  ( (ownGoal_x2 - ownGoal_x1)*(ownGoal_x2 - ownGoal_x1) ) + ( (ownGoal_y2 - ownGoal_y1)*(ownGoal_y2 - ownGoal_y1) )  );
  
      ownGoal = Complex(ownGoal_X, ownGoal_Y) + mirrorOffset;
      ownGoal_seen = true;
      
      ownGoal_unseen_cnt = 0;
      ++ball_unseen_cnt;
      ++goal_unseen_cnt;
    }

    if(ball_unseen_cnt >= MAX_ITER_FOR_UNSEEN) {
      ball_seen = false;
      ballDir = -8;
      ball = Complex(0,0);
      ball_unseen_cnt = MAX_ITER_FOR_UNSEEN;
    }
    else ballDir = dirOfObj(ball);
    
    if(goal_unseen_cnt >= MAX_ITER_FOR_UNSEEN+1) {
      goal_seen = false;
      midpoint_valid = false;
      goalDir = -8;
      goal = Complex(0,0);
      goal_unseen_cnt = MAX_ITER_FOR_UNSEEN+1;
    }
    else goalDir = dirOfObj(goal);
    
    if(ownGoal_unseen_cnt >= MAX_ITER_FOR_UNSEEN+2) {
      ownGoal_seen = false;
      midpoint_valid = false;
      ownGoalDir = -8;
      ownGoal = Complex(0,0);
      goal_unseen_cnt = MAX_ITER_FOR_UNSEEN+2;
    }
    else ownGoalDir = dirOfObj(ownGoal);
  }
  
  midpoint = (goal+ownGoal)/2;
  midpointDir = dirOfObj(midpoint);

  
  if(goal_seen && ownGoal_seen) midpoint_valid = true;
  
  if(cam_timeout >= 500) {
      
      //hier rote LEDs an 
      
      ball_seen = false;
      ballDir = -8;
      ball = Complex(0,0);

      goal_seen = false;
      goalDir = -8;
      goal = Complex(0,0);

      ownGoal_seen = false;
      ownGoalDir = -8;
      ownGoal = Complex(0,0);

      midpoint_valid = false;
      midpointDir = -8;
      midpoint = Complex(0,0);
  }
}

void getData_can() {
 
  int bodenpaket = 0;
  //digitalWrite(LED,HIGH);//CAN.beginPacket(0x03,1,true); // sendet RTR und will 1 byte
  CAN.beginPacket(0x01,1,true); // sendet RTR und will 1 byte
  CAN.endPacket();
  while (!CAN.parsePacket()) delayMicroseconds(1);
  
  while (CAN.available()) bodenpaket = CAN.read();       
      
   //digitalWrite(LED,LOW);  
   line_unique = bodenpaket & 16;
   line_flag   = bodenpaket & 32;
   line_seen   = bodenpaket & 64;
   lineDir     = (bodenpaket%16)-7;
}


void btDoGameModeSwitch() {
  return;
  if(!play || btDoGameModeSwitch_timer < BLOCK_GAMEMODE_SWITCH_MILLIS) return;

  gameMode = btGameMode;
  dynamicGameModeSwitch = btDynamicGameModeSwitch;
  btDoGameModeSwitch_timer = 0;
  if(gameMode > lastGameMode)
    btJustBecameOffensive_timer = 0;
}

void getData_bt() {
  while(Serial.available()) {
      String btString = Serial.readStringUntil('\n');
      
      if(btString.substring(0,btString.indexOf(':')) != "BtData") continue;
        
      btString = btString.substring(btString.indexOf(' ')+1);
      
      btPlay = btString.substring(0,btString.indexOf(',')).toInt();
      btString = btString.substring(btString.indexOf(' ')+1);
      
      btDynamicGameModeSwitch = btString.substring(0,btString.indexOf(',')).toInt();
      btString = btString.substring(btString.indexOf(' ')+1);
      
      btGameMode = GameMode(btString.substring(0,btString.indexOf(',')).toInt());
      btString = btString.substring(btString.indexOf(' ')+1);
      
      btGotBall = btString.substring(0,btString.indexOf(',')).toInt();
      btString = btString.substring(btString.indexOf(' ')+1);
      
      btBall_seen = bool(btString.substring(0,btString.indexOf(',')).toInt());
      btString = btString.substring(btString.indexOf(' ')+1);
      
      btGoal_seen = bool(btString.substring(0,btString.indexOf(',')).toInt());
      btString = btString.substring(btString.indexOf(' ')+1);
      
      btOwnGoal_seen = bool(btString.substring(0,btString.indexOf(',')).toInt());
      btString = btString.substring(btString.indexOf(' ')+1);
      
      btBallDir = btString.substring(0,btString.indexOf(',')).toInt()-8;
      btString = btString.substring(btString.indexOf(' ')+1);
      
      btBallDist = btString.substring(0,btString.indexOf(',')).toInt()/100.0;
      btString = btString.substring(btString.indexOf(' ')+1);
      
      if(btGoal_seen && btOwnGoal_seen) {
        btMidpointDir = btString.substring(0,btString.indexOf(',')).toInt()-8;
        btString = btString.substring(btString.indexOf(' ')+1);
      
        btMidpointDist = btString.substring(0,btString.indexOf(',')).toInt()/100.0;
        btString = btString.substring(btString.indexOf(' ')+1);
      }
      
      if(btGoal_seen && !btOwnGoal_seen) {
        btGoalDir = btString.substring(0,btString.indexOf(',')).toInt()-8;
        btString = btString.substring(btString.indexOf(' ')+1);
      
        btGoalDist = btString.substring(0,btString.indexOf(',')).toInt()/100.0;
        btString = btString.substring(btString.indexOf(' ')+1);
      }

      if(!btGoal_seen && btOwnGoal_seen) {
        btOwnGoalDir = btString.substring(0,btString.indexOf(',')).toInt()-8;
        btString = btString.substring(btString.indexOf(' ')+1);
      
        btOwnGoalDist = btString.substring(0,btString.indexOf(',')).toInt()/100.0;
        btString = btString.substring(btString.indexOf(' ')+1);
      }

      btSwitch = bool(btString.substring(0,btString.indexOf(',')).toInt());
  }
  if(btSwitch) btDoGameModeSwitch();
}

int cmps_read_org() {
  unsigned char high_byte, low_byte;//, angle8;
  unsigned int angle16;
  Wire.beginTransmission(CMPS_ADD);
  Wire.write(1);
  Wire.endTransmission();
  Wire.requestFrom(CMPS_ADD, 3);
  while(Wire.available() < 3);
  Wire.read();//angle8 = Wire.read();      
  high_byte = Wire.read();
  low_byte = Wire.read();
  angle16 = high_byte;                 
  angle16 <<= 8;
  angle16 += low_byte;
  return angle16 / 10;
}

void getData_controling() {
  
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
    packet = packet % 128;
    cD_right.button2 = (packet > 63);
    packet = packet % 2;
    cD_right.button1 = (packet > 0);
  }  
}

void getData_i2c() {
  cmpsDir = -dirOfDeg((((cmps_read_org()-cmps_head)+180+360)%360)-180);
  getData_controling();
}

void getData_pins() {
  gotBall = (analogRead(LIGHT_PIN) >= 3500);
  if(!gotBall) gotBall_timer = 0;
}

void getData() {
  getData_ai();
  getData_can();
  getData_bt();
  getData_i2c();  
  getData_pins();

  if(!goal_seen) goalDir = cmpsDir;
}


bool btSendGameModeSwitch() {
  if(!play || !btPlay || lastGameMode <= btGameMode || playing_timer < BLOCK_GAMEMODE_SWITCH_MILLIS) return false; //other bot is more defensive and therefore the deciding one

  bool defensive_blocks_offensive = false;
  if(btGoal_seen && btOwnGoal_seen) defensive_blocks_offensive = (midpointDir == btMidpointDir);
  else if(btGoal_seen)              defensive_blocks_offensive = (goalDir == btGoalDir);
  else if(btOwnGoal_seen)           defensive_blocks_offensive = (ownGoalDir == btOwnGoalDir);
  
  bool defensive_in_front_of_offensive = (ball_seen && btBall_seen && 
                                          abs(ballDir) < 4 && ballDir == btBallDir && defensive_blocks_offensive && 
                                          ball.rad/MAX_REAL_MIDPOINT_RAD < 0.95*btBallDist);
                                          
  bool defensive_can_strike            = (ball_seen && ballDir == 0 && ball.rad/MAX_REAL_MIDPOINT_RAD < GOALIE_DIST_RATIO*0.8 && 
                                          ownGoal.rad/MAX_REAL_MIDPOINT_RAD < GOALIE_DIST_RATIO+0.05 && abs(btBallDir) > 4 && !btGotBall);
                                          
  bool offensive_ball_not_seen         = (ball_seen && ballDir == 0 && !btBall_seen && !btGotBall && ball.rad/MAX_REAL_MIDPOINT_RAD < GOALIE_DIST_RATIO*0.5);
  
  bool switch_not_performed            = (gameMode != lastGameMode && btGameMode != lastGameMode);
  
  return (gotBall || defensive_in_front_of_offensive || defensive_can_strike || offensive_ball_not_seen || switch_not_performed);  
}

int bluetooth_cnt = 0; 

void sendDataBt(const int& n) {
     
  bool btSwitch = btSendGameModeSwitch();
  if(!btSwitch) {
    if(gameMode < btGameMode) btJustBecameOffensive = false;
    lastGameMode = gameMode;
    lastDynamicGameModeSwitch = dynamicGameModeSwitch;
  }
  else {
    if(btJustBecameOffensive_timer > BLOCK_GAMEMODE_SWITCH_MILLIS+10) {
      btJustBecameOffensive = true;
      btJustBecameOffensive_timer = 0;
    }
    btDoGameModeSwitch(); 
  }
  
  if (playing_timer < BLOCK_GAMEMODE_SWITCH_MILLIS) btJustBecameOffensive = false;
  
  if(!btPlay && gameMode > btGameMode && playing_timer > BLOCK_GAMEMODE_SWITCH_MILLIS) 
    btDoGameModeSwitch();
  
  String btString = "BtData: ";
  btString += String(play)+", ";
  btString += String(lastDynamicGameModeSwitch)+", ";
  btString += String(lastGameMode)+", ";
  btString += String(gotBall)+", ";
  btString += String(ball_seen)+", ";
  btString += String(goal_seen)+", ";
  btString += String(ownGoal_seen)+", ";
  btString += String(ballDir+8)+", ";
  btString += String((ball.rad*100)/MAX_REAL_MIDPOINT_RAD)+", ";
  
  if(midpoint_valid) {
    btString += String(midpointDir+8)+", ";
    btString += String((midpoint.rad*100)/MAX_REAL_MIDPOINT_RAD)+", "; 
  } else if(goal_seen) {
    btString += String(goalDir+8)+", ";
    btString += String((goal.rad*100)/MAX_REAL_MIDPOINT_RAD)+", "; 
  } else if(ownGoal_seen) {
    btString += String(ownGoalDir+8)+", ";
    btString += String((ownGoal.rad*100)/MAX_REAL_MIDPOINT_RAD)+", "; 
  }

  btString += String((btSwitch || btJustBecameOffensive))+", ";

  bluetooth_cnt = (bluetooth_cnt+1)%n;
  if(bluetooth_cnt == 0) Serial.println(btString);
}

void cmps_set_heading() {
  cmps_head = cmps_read_org();
}
