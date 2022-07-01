 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     POSITIONING      //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool at_penalty_area() {
  return (goal_seen && penalty_matrix[lineDir+7][goalDir+7]);
}
bool at_own_penalty_area() {
  return (ownGoal_seen && penalty_matrix[lineDir+7][ownGoalDir+7]);
}
bool ball_in_own_penalty_area() {
  return (ownGoal_seen && penalty_matrix[ballDir+7][ownGoalDir+7]);
}

bool waiting_for_referee() {
  return !waiting_matrix[lineDir+7][ballDir+7]; 
}

bool ballInside() {
  if(!line_unique || line_flag || !ball_seen) return false;
  int dir_diff = 0;
  if(ballDir+8 >= lineDir) dir_diff = ballDir - lineDir;
  else dir_diff = lineDir - ballDir;
  dir_diff = abs(dir_diff);
  return (dir_diff > 4 && dir_diff < 12);
}

bool ballNearMidpoint() {
  if(!midpoint_valid) return false;
  int dir_diff = 0;
  if(ballDir+8 >= midpointDir) dir_diff = ballDir - midpointDir;
  else dir_diff = midpointDir - ballDir;
  dir_diff = abs(dir_diff);
  return !(dir_diff > 4 && dir_diff < 12);
}

bool blockByStayingInFrontOfBall() {
  if(!goal_seen || !ball_seen) return false;
  if(goal.rad/MAX_REAL_MIDPOINT_RAD > 1.0) return false;
  int dir_diff = 0;
  if(ballDir+8 >= goalDir) dir_diff = ballDir - goalDir;
  else dir_diff = lineDir - goalDir;
  dir_diff = abs(dir_diff);
  return (dir_diff > 4 && dir_diff < 12);
}

bool kickOutOfBounce() {
  if(!goal_seen || !ball_seen) return false;
  if(goal.rad/MAX_REAL_MIDPOINT_RAD > 1.0) return false;
  int dir_diff = 0;
  if(ballDir+8 >= goalDir) dir_diff = ballDir - goalDir;
  else dir_diff = ballDir - goalDir;
  dir_diff = abs(dir_diff);
  return (dir_diff == 5 || dir_diff == 11);
}

GameMode offensiveGameModeSwitch() {
  if(!dynamicGameModeSwitch) return gameMode;
  
  int angleDiff = abs( (ownGoal.phi*180)/PI - (goal.phi*180)/PI );
  if(angleDiff > 180) angleDiff = 360-angleDiff;
  if(angleDiff > GOAL_ANGLE_GAMEMODESWITCH_INNER_BOARDER) return gameMode;        //bot in inner zone
  if(angleDiff < GOAL_ANGLE_GAMEMODESWITCH_OUTER_BOARDER) return strikerGameMode; //bot in outer zone
  return dribblerGameMode; //bot in half-outer zone
}

GameMode defensiveGameModeSwitch() {
  if(!dynamicGameModeSwitch) return gameMode;
  
  if(abs(ballDir) < 7 && ball.rad/MAX_REAL_MIDPOINT_RAD < 0.5) return defenderGameMode;
  else                                                         return goalieGameMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     WHITE LINE       //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void avoid_line_fahre(bool real, const int& d, const int& s, const int& r) {
  if(real) fahre_real(d,s,r);
  else     fahre(d,s,r);
}

void avoid_line(bool real) {
  int spd = MAX_SPEED_AT_LINE;
  if(line_flag) spd = 100;
    //hier ball mit einbeziehen wenn ball nicht im strafraum ist
  if(!line_flag && //line_timer <= 1000 && 
                   ownGoal_seen && 
                   ownGoal.rad/MAX_REAL_MIDPOINT_RAD < GOALIE_DIST_RATIO && 
                   ball_seen && !gotBall && 
                   abs(ballDir) < 4 && 
                   abs(midpointDir) < 4){ 
    avoid_line_fahre(real,bound_direction(ballDir+side(ballDir)),spd/4,3*goalDir);
    return;
  }
     
  if(midpoint_valid)  avoid_line_fahre(real,midpointDir,spd,3*goalDir);
  else {
    int d = bound_direction(lineDir+8);
    if(goal_seen && goal.rad > MAX_REAL_MIDPOINT_RAD) d = goalDir;
    if(ownGoal_seen && ownGoal.rad > MAX_REAL_MIDPOINT_RAD) d = ownGoalDir;
    avoid_line_fahre(real,d,spd,3*goalDir);
   }
}

void stayInBounce() {
  if(!line_seen) return;
  if(gameMode == defenderGameMode && !line_flag && line_unique) return;
  
  firstGreen = false;
  gotJustBack = true;
  
  if(line_flag || !line_unique) avoid_line(true);
  else {
    
    if(line_timer > 1000) avoid_line(false); 
    else {
      if(ballInside() && ballNearMidpoint()) { gotJustBack = false; return; }
      if(ballInside() && !ballNearMidpoint() && abs(lineDir) > 4) { fahre_save(4*side(ballDir), 75, 3*goalDir); return; }
      
      if(!at_penalty_area() && !at_own_penalty_area() && (!waiting_for_referee() || ((ball.rad/MAX_REAL_MIDPOINT_RAD) > 0.45))) {
        avoid_line(false);
        return;
      }
      
      if(at_penalty_area()) { 
        if(midpoint_valid) { avoid_line(false); return; }
        fahre(side(goalDir)*6,100,cmpsDir*4); 
        return; 
      }
        
      if(at_own_penalty_area()) { 
        if(midpoint_valid) { avoid_line(false); return; }
        fahre(side(goalDir)*2,100,cmpsDir*4); 
        return; 
      }
      int rot = (goal_seen ? (goal.phi*30)/PI: goalDir*4);
      fahre(0,0,rot);
      line_timer = 0;
    }
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       MOVING         //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int dirOffset() {
  int offset = 0;
  if(ball.rad/MAX_REAL_MIDPOINT_RAD < 0.6) offset = 2;
  if(ball.rad/MAX_REAL_MIDPOINT_RAD < 0.5) offset = 3;
  if(abs(ballDir) < 2)                     offset /= 2;
  
  if(line_seen)                            offset = 1;
  if(ballDir == 0 || rushToBall)           offset = 0;
  
  return offset;
}

void getBehindBall_old() { 
  if(!ball_seen) {
    if(goal_seen) fahre(0,0,4*goalDir);
    else          fahre(0,0,0);
    return;
  }

  if(ballDir == 0)     rushToBall = true;
  if(abs(ballDir) > 1) rushToBall = false;

  //evaluating the direction for the movement
  int ball_angle = (ball.phi*180)/PI;
  int sign_for_0 = 0;
  if(abs(ball_angle) != 0) sign_for_0 = ball_angle/abs(ball_angle);
  int sign_for_8 = (midpoint_valid && midpointDir != 0 ? -midpointDir/abs(midpointDir) : 1);
  
  int dir_side = 0;
  if(ballDir%8 != 0) dir_side = side(ballDir);
  if(ballDir == 0)   dir_side = sign_for_0;
  if(ballDir == 8)   dir_side = sign_for_8;
  
  int dir = bound_direction(ballDir+(dirOffset()*dir_side));

  //evaluating the rotation for the movement
  int rot = (goal_seen ? (goal.phi*30)/PI: goalDir*4);
  if(ballDir == 0 && !gotBall) rot /= 4;
    
  if(goal_seen && abs(goalDir) < 2 && ball_seen && abs(ball_angle) < 45 && abs(ball_angle) > 5 && ball.rad/MAX_REAL_MIDPOINT_RAD < 0.3) {
    int turnToBall = sqrt(double(abs(ball_angle)/45))*20;
    rot = (ball_angle > 0 ? turnToBall : -turnToBall);
  }

  //evaluating the speed for the movement
  int spd = SPEED_TRAP-rot;
  if(ball.rad/MAX_REAL_MIDPOINT_RAD < 0.8) {
    if(abs(ball_angle) > 5 && abs(ballDir) < 4) spd = pow(abs(ballDir)+1,2)*(spd)/(70*pow(1-(ball.rad/MAX_REAL_MIDPOINT_RAD),2));
    if(abs(ball_angle) <= 5) spd /= 2;
  }
  if(spd < 10) spd = 10;
    
  fahre_save(dir,spd,rot);
}

void getBehindBall_slow() { 
  if(!ball_seen) {
    if(goal_seen) fahre(0,0,4*goalDir);
    else          fahre(0,0,0);
    return;
  }
  //                           0    1    2    3    4    5    6    7    8
  static double distArr[9] = { 0, 0.7, 1.5, 2.5, 4.0, 5.0, 6.0, 7.0, 8.0};
  double factor = 0.1 * distArr[abs(ballDir)];

  Complex ownGoalToGoal = (goal - ownGoal);
  Complex ballOffset = ownGoalToGoal*factor;
  Complex spot = ball - ballOffset;
  int spotDir = dirOfObj(spot);

  //moving spot a bit to the side of the ball so that bot gets behind it and does not push the ball slowly diagonal
  double spotDir_factor = double(4-abs(abs(spotDir)-4) ) / 4.0;
  spot += Complex(side(spotDir)*ballOffset.rad,0)*spotDir_factor;
  
  int ball_angle = (ball.phi*180)/PI;
  
  int dir = dirOfObj(spot);
  if(abs(ballDir) > 4 && (ballDir <= bound_direction(dir+2) || ballDir >= bound_direction(dir-2) ) )
    dir = bound_direction(dir+(2*side(ballDir)));

  
  int sign_for_8 = (midpoint_valid && midpointDir != 0 ? midpointDir/abs(midpointDir) : 1);
  if(ballDir == 8) dir = 4*sign_for_8;

  int rot = goalDir*4;
  if(goal_seen)        rot = (goal.phi*20)/PI;
  if(midpoint_valid)   rot = (ownGoalToGoal.phi*20)/PI;
  if(abs(ballDir) < 4 && abs(goalDir) < 3) rot = (ball.phi*20)/PI;

  double spdFactor        = spot.rad/MAX_REAL_MIDPOINT_RAD;
  double spdFactor_weight = 1 - (abs(ballDir)/8);
  
  int spd = (abs(ball_angle) <= 5 ? 100 : min(max(10, int(100.0*spdFactor*spdFactor_weight)), SPEED_TRAP-rot) );
  
  fahre_save(dir,max(10,(spd*3)/5),rot);
}

void getBehindBall() { 
  if(!ball_seen) {
    if(goal_seen) fahre(0,0,4*goalDir);
    else          fahre(0,0,0);
    return;
  }
  //                           0    1    2    3    4    5    6    7    8
  static double distArr[9] = { 0, 0.5, 2.0, 2.5, 4.0, 5.0, 6.0, 7.0, 8.0};
  double factor = 0.1 * distArr[abs(ballDir)];

  Complex ownGoalToGoal = (goal - ownGoal);
  Complex ballOffset = ownGoalToGoal*factor;
  Complex spot = ball - ballOffset;
  int spotDir = dirOfObj(spot);

  //moving spot a bit to the side of the ball so that bot gets behind it and does not push the ball slowly diagonal
  double spotDir_factor = double(4-abs(abs(spotDir)-4) ) / 4.0;
  spot += Complex(side(spotDir)*ballOffset.rad/2,0)*spotDir_factor;
  
  int ball_angle = (ball.phi*180)/PI;
  
  int dir = dirOfObj(spot);
  if(abs(ballDir) > 4 && (ballDir <= bound_direction(dir+2) || ballDir >= bound_direction(dir-2) ) )
    dir = bound_direction(dir+(2*side(ballDir)));

  
  int sign_for_8 = (midpoint_valid && midpointDir != 0 ? midpointDir/abs(midpointDir) : 1);
  if(ballDir == 8) dir = 4*sign_for_8;

  int rot = goalDir*4;
  if(goal_seen)      rot = (goal.phi*20)/PI;
  if(midpoint_valid) rot = (ownGoalToGoal.phi*20)/PI;

  double spdFactor        = spot.rad/MAX_REAL_MIDPOINT_RAD;
  double spdFactor_weight = 1 - (abs(ballDir)/8);
  
  int spd = (abs(ball_angle) <= 5 ? 100 : min(max(10, int(100.0*spdFactor*spdFactor_weight)), SPEED_TRAP-rot) );

  fahre_save(dir,spd,rot);
}
