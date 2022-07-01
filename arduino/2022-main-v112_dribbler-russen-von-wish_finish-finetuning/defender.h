
void defend() {
  int    dir;
  double spd;
  int    rot;

  Complex spot(0,0);
  if(abs(ballDir) > 3) spot = ownGoal + ((ball - ownGoal)*sqrt(GOALIE_DIST_RATIO));
  else                 spot = ownGoal + ((ball - ownGoal) - ((goal - ownGoal)*0.15));
  dir = dirOfObj(spot);

  double spotRad_factor = spot.rad/MAX_REAL_MIDPOINT_RAD;
  double spotDir_factor = double(4-abs(abs(dir)-4) ) / 4.0; 
  spd = (ballDir != 0 ? 100 * spotRad_factor * spotDir_factor : 75);
  
  int fake_ballDir = ballDir - side(ballDir);
  rot = 3*fake_ballDir;

  if(gotBall) {
    int s = side(midpointDir);
    if(!midpoint_valid) s = side(goalDir);
  
    int fake_goalDir = goalDir + side(ownGoalDir);

    fahre_save_rot(-2*s,90,6*fake_goalDir*abs(fake_goalDir));
    return;
  }

  fahre_save(dir,spd,rot);
}
  

void defender() { 
  if(!ownGoal_seen || ownGoal.rad/MAX_REAL_MIDPOINT_RAD > 1.0 || (!ball_seen && !gotBall)) {
    goalie();
    return;
  }
  defend();
}
