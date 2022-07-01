
void neutralSpot_centered_defensive() {
  int d = 0, s = 0, r = 0;
  if(midpoint_valid) {
    Complex spot = ownGoal + ((midpoint - ownGoal)*GOALIE_DIST_RATIO);
    d = dirOfObj(spot);
    s = (100*spot.rad)/MAX_REAL_MIDPOINT_RAD;
    r = (goal.phi*45)/PI;
  }
  else if(ownGoal_seen) {
    if(ownGoal.rad/MAX_REAL_MIDPOINT_RAD < GOALIE_DIST_RATIO - 0.05)  d = bound_direction(ownGoalDir+8);
    if(ownGoal.rad/MAX_REAL_MIDPOINT_RAD > GOALIE_DIST_RATIO + 0.05)  d = ownGoalDir;
    double dist = ownGoal.rad - (GOALIE_DIST_RATIO*MAX_REAL_MIDPOINT_RAD);
    s = 100*pow(dist/MAX_REAL_MIDPOINT_RAD,2);
  }
  
  fahre(d,s,r);
}

void neutralSpot_corner_defensive() {
  int d = 0, s = 0, r = 0;
  ////////////////////////////////////////////////
  //
  //
  //
  ////////////////////////////////////////////////
  fahre(d,s,r);
}

void moveToNeutralSpot_defensive() {
  if(neutralSpot_centered) neutralSpot_centered_defensive();
  else                     neutralSpot_corner_defensive(); 
}

void blockGoal() {

  int    dir;
  double spd;
  int    rot;

  int ball_angle = (ball.phi*180)/PI;
  dir = (abs(ball_angle) > 5 ? 4*ball_angle/abs(ball_angle) : 0);

  bool outOfDistZone = false;
  if(ownGoal_seen && ownGoal.rad/MAX_REAL_MIDPOINT_RAD < GOALIE_DIST_RATIO - 0.05) { 
    dir = bound_direction(ownGoalDir+8);
    outOfDistZone = true;
  }
  if(ownGoal_seen && ownGoal.rad/MAX_REAL_MIDPOINT_RAD > GOALIE_DIST_RATIO + 0.05) {
    dir = ownGoalDir;
    outOfDistZone = true;
  }

  double ballRad_factor = sqrt(ball.rad/MAX_REAL_MIDPOINT_RAD);
  double ballDir_factor = double(4-abs(abs(ballDir)-4) ) / 4.0;

  double d_dist = ownGoal.rad - (GOALIE_DIST_RATIO*MAX_REAL_MIDPOINT_RAD);
  double outOfDistZone_factor = pow(d_dist/MAX_REAL_MIDPOINT_RAD,3);

  if(outOfDistZone) spd = 100 *outOfDistZone_factor;
  spd = 100 * ballDir_factor * ballRad_factor;
  
  if(!ball_seen) spd = (goal.phi*45)/PI;
  if(dir == 0 && ball_seen && side(ballDir) == 0) spd = 0;
  
  rot = 3*bound_direction(ownGoalDir+8);
  if(!ownGoal_seen) {
    if(goal_seen) rot = 3*bound_direction(goalDir+8);
    else          rot = 3*cmpsDir;
  }
  
  fahre_save(dir,spd,rot);
}

void goalie() {
  if(!ball_seen && !gotBall) { moveToNeutralSpot_defensive(); return; }
  blockGoal();
}
