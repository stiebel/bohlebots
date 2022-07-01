
void turnAroundBall() {  
  if(goalDir == 0) { kick(); return; }
  
  int sign = side(midpointDir);
  if(!midpoint_valid) sign = side(goalDir);
  
  int fake_goalDir = goalDir + side(ownGoalDir);

  //fahre_save(-2*sign,90,6*fake_goalDir*abs(fake_goalDir));
  fahre_save_rot(-2*sign,90,6*fake_goalDir*abs(goalDir));
}

void dribble() {
  if(goal.rad < 1.0*MAX_REAL_MIDPOINT_RAD) {
    turnAroundBall();
    return;
  }
  
  Complex heading = Complex::rotate(goal,-(side(goalDir)*PI*40)/180);
  
  int dir = side(goalDir);
  int spd = (SPEED_TRAP*4)/5; 
  int rot = (heading.phi*20)/PI;

  fahre_save(dir,spd,rot);
}

void dribbler() {
  if(!goal_seen || !ownGoal_seen || (!ball_seen && !gotBall)) {
    striker();
    return;  
  }
  if(gotBall) { dribble(); return; }
  kick_timer = 0;
  getBehindBall_slow();
}
