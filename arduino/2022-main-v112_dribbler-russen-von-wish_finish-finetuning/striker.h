
void neutralSpot_centered_offensive() {
  int d = 0, s = 0, r = 0;
  if(midpoint_valid) {
    d = midpointDir;
    s = (100*midpoint.rad)/MAX_REAL_MIDPOINT_RAD;
    r = (goal.phi*45)/PI;
  }
  else if(goal_seen) {
    if(goal.rad/MAX_REAL_MIDPOINT_RAD < GOALIE_DIST_RATIO - 0.05)  d = bound_direction(goalDir+8);
    if(goal.rad/MAX_REAL_MIDPOINT_RAD > GOALIE_DIST_RATIO + 0.05)  d = goalDir;
    double dist = goal.rad - (GOALIE_DIST_RATIO*MAX_REAL_MIDPOINT_RAD);
    s = 100*pow(dist/MAX_REAL_MIDPOINT_RAD,2);
  }

  fahre(d,s,r);
}

void neutralSpot_corner_offensive() {
  int d = 0, s = 0, r = 0;
  ////////////////////////////////////////////////
  //
  //
  //
  ////////////////////////////////////////////////
  fahre(d,s,r);
}

void moveToNeutralSpot_offensive() {
  if(neutralSpot_centered) neutralSpot_centered_offensive();
  else                     neutralSpot_corner_offensive(); 
}


void strike() {  
  int rot, spd;
  if(goal_seen) {
    rot = (goal.phi*60)/PI;
    spd = 100;
  }
  else {
    rot = goalDir*4;
    spd = 50;
  }

  bool fast_kick      = (abs((goal.phi*180)/PI) < 35 && (goal.rad/MAX_REAL_MIDPOINT_RAD) < 0.95);
  bool drive_by_snipe = (goal.rad/MAX_REAL_MIDPOINT_RAD < 1.3 && abs((goal.phi*180)/PI) < 7 && gotBall_timer > 50);
  
  if(fast_kick || drive_by_snipe) kick();
  else fahre_save(side(goalDir), spd, rot);
}

void striker() {
  if(!ball_seen && !gotBall) { moveToNeutralSpot_offensive(); return; }
  if(gotBall) { strike(); return; }
  kick_timer = 0;
  getBehindBall();
}
