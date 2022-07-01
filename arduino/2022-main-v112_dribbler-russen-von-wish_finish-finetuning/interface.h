

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CONTROL DEVICES    //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool button(ControlDeviceNumber cdn, int front_or_back) {
  //Serial.println(String(cdn));
  if(cdn == left) {
    if(front_or_back == 1) return cD_left.button1;
    if(front_or_back == 2) return cD_left.button2;
  }
  if(cdn == middle) {
    if(front_or_back == 1) return cD_middle.button1;
    if(front_or_back == 2) return cD_middle.button2;
  }
  if(cdn == right) {
    if(front_or_back == 1) return cD_right.button1;
    if(front_or_back == 2) return cD_right.button2;
  }
  return false;
}



void led(ControlDeviceNumber cdn, int front_or_back, LED_Colour c) {
  int col = c;
  if(front_or_back == 1) col *= 2;
  if(front_or_back == 2) col *= 16;
  if(col > 63) col += 64;
  
  if(cdn == left) {
    if(front_or_back == 1) cD_left.led1_colour = col;
    if(front_or_back == 2) cD_left.led2_colour = col;
  }
  if(cdn == middle) {
    if(front_or_back == 1) cD_middle.led1_colour = col;
    if(front_or_back == 2) cD_middle.led2_colour = col;
  }
  if(cdn == right) {
    if(front_or_back == 1) cD_right.led1_colour = col;
    if(front_or_back == 2) cD_right.led2_colour = col;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONTROL INTERFACE   //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void shutOffLEDs() {
  led(left  ,1,off);
  led(left  ,2,off);
  led(middle,1,off);
  led(middle,2,off);
  led(right ,1,off);
  led(right ,2,off);
  digitalWrite(BUILTIN_LED, LOW);
}


void toggle_leds() {
  LED_Colour c[6] = {off,off,off,off,off,off};

  if(gotBall) c[0] = white;
  
  if(cam_timeout >= 500) c[1] = red;
  else c[1] = (strikingGoalLabel == 1 ? yellow : blue);

  switch (gameMode) {
    case strikerGameMode:  c[2] = red;     break;
    case goalieGameMode:   c[2] = green;   break;
    case dribblerGameMode: c[2] = magenta; break;
    case defenderGameMode: c[2] = yellow;  break;
    default: break;
  }

  if(goal_seen) {
    c[3] = white;
    if(goalDir == 0) c[3] = (strikingGoalLabel == 1 ? yellow : blue);
  }

  if(ball_seen) {
    c[4] = white;
    if(ballDir == 0) c[4] = red;
  }

  if(dynamicGameModeSwitch) c[5] = blue;
  else c[5] = c[2];
  
  if(cmpsDir == 0 && cD_right.button2 && !cD_left.button1) c[5] = white;
  
  led(left  ,1,c[0]);
  led(left  ,2,c[1]);
  led(middle,1,c[2]);
  led(middle,2,c[3]);
  led(right ,1,c[4]);
  led(right ,2,c[5]);
  digitalWrite(BUILTIN_LED, debug);
}


void controlInterface_settings() {
  if(cD_middle.button1) { gameMode = GameMode(gameMode-1 < 0 ? goalieGameMode : gameMode-1); button_timer = 0; }
  if(cD_middle.button2) { strikingGoalLabel = (strikingGoalLabel == 1 ? 2 : 1);              button_timer = 0; }
  if(cD_right.button1)  { debug = !debug;                                                    button_timer = 0; }
  if(cD_right.button2)  { dynamicGameModeSwitch = !dynamicGameModeSwitch;                    button_timer = 0; }
  toggle_leds();
}

void controlInterface_standby() {
  if(cD_middle.button1) { play = true;          button_timer = 0; playing_timer = 0; return; }
  if(cD_middle.button2) { ena_leds = !ena_leds; button_timer = 0; }
  if(cD_right.button1)  { kick();               button_timer = 0; }
  if(cD_right.button2)  { cmps_set_heading();   button_timer = 0; }
  if(ena_leds) toggle_leds();
  else shutOffLEDs();
}

void controlInterface() {
  if(button_timer < 500) return;
  if(play) {
    if(!cD_middle.button2) { shutOffLEDs(); return; }
    play = false;
    button_timer = 0;
  }
  else {
    if(cD_left.button1) controlInterface_settings();
    else controlInterface_standby();
  }
}
