////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        PORTS         //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SDA 21
#define SCL 22

#define RXD2 16
#define TXD2 17

#define RXD0 3 
#define TXD0 1

#define LIGHT_PIN 25
#define CMPS_ADD  0x60
#define KICK_PIN  32


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    CONTROL PANELS    //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BUILTIN_LED        2
#define CONTROL_ADD_LEFT   0x23
#define CONTROL_ADD_MIDDLE 0x21
#define CONTROL_ADD_RIGHT  0x20

#define CONTROL_DEVICE_LEFT   3
#define CONTROL_DEVICE_MIDDLE 1
#define CONTROL_DEVICE_RIGHT  0

enum ControlDeviceNumber : int {
  left = CONTROL_DEVICE_LEFT, 
  middle = CONTROL_DEVICE_MIDDLE, 
  right = CONTROL_DEVICE_RIGHT
};

enum LED_Colour : int {
  off = 0, 
  green, 
  red, 
  yellow, 
  blue, 
  cyan, 
  magenta, 
  white
};

struct ControlDevice {
  bool button1;
  bool button2;
  int  led1_colour;
  int  led2_colour;
  ControlDevice(): button1(false), button2(false), led1_colour(off), led2_colour(off) {}
};

ControlDevice cD_left;
ControlDevice cD_middle;
ControlDevice cD_right;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      CONSTANTS       //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIN_SLIDE_PROP_FACTOR   0.0
#define MAX_SLIDE_PROP_FACTOR   2.0

#define GOALIE_DIST_RATIO 0.85

//[ms]
#define MIN_ACCELERATION_TIME 100
#define MAX_ACCELERATION_TIME 300
#define KICK_DELAY              5

//[%]
#define MAX_SPEED_AT_LINE 45
#define SPEED_TRAP        65

//[n]
#define MAX_ITER_FOR_UNSEEN   10

//[cm]
#define FIELD_LENGTH  193
#define FIELD_WIDTH   132

#define BALL_DIAMETER   4.27
#define GOAL_HEIGHT    12

#define CAM_HEIGHT      9
#define DIST_CAM_MIRROR 4.4

#define MIRROR_RADIUS   2.5

//[deg]
#define MIRROR_ANGLE    35
#define GOAL_ANGLE_GAMEMODESWITCH_INNER_BOARDER 130
#define GOAL_ANGLE_GAMEMODESWITCH_OUTER_BOARDER 110

//[px]
#define AI_PIC_WIDTH  416
#define AI_PIC_HEIGHT 416

#define MAX_REAL_MIDPOINT_RAD  160
#define MAX_REAL_BALL_RAD      195
const double MAX_RAD = sqrt( ((AI_PIC_WIDTH*AI_PIC_WIDTH) + (AI_PIC_HEIGHT*AI_PIC_HEIGHT))/4 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        FLAGS         //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  ball_unseen_cnt = 0, goal_unseen_cnt = 0, ownGoal_unseen_cnt = 0, midpoint_valid = false;
bool ball_seen = false, goal_seen = false, ownGoal_seen = false, line_seen = false;
bool btBall_seen = false, btGoal_seen = false, btOwnGoal_seen = false;

bool line_flag = false, line_unique = false, firstGreen = false;

bool   gotBall = false,   gotJustBack = false;
bool btGotBall = false;

bool ball_on_right_side = true;
bool rushToBall = false;

int strikingGoalLabel = 1; // 1 = yellow, 2 = blue

bool dynamicGameModeSwitch =  true;
bool lastDynamicGameModeSwitch = dynamicGameModeSwitch;
bool btDynamicGameModeSwitch = true;

bool btJustBecameOffensive = false;

bool neutralSpot_centered = true;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      DIRECTIONS      //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int   ballDir = -8,   goalDir = -8,   ownGoalDir = -8,   midpointDir = -8;
int btBallDir = -8, btGoalDir = -8, btOwnGoalDir = -8, btMidpointDir = -8;
double btBallDist = 0.0, btGoalDist = 0.0, btOwnGoalDist = 0.0, btMidpointDist = 0.0; 

int   lineDir = -8,   cmpsDir = -8,    cmps_head = 0;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      GAMEMODES       //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum  GameMode : int {
  strikerGameMode = 0,
  dribblerGameMode, 
  defenderGameMode,
  goalieGameMode
};

GameMode gameMode     = strikerGameMode;
GameMode lastGameMode = gameMode;
GameMode btGameMode   = strikerGameMode;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       OBJECTS        //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Complex   ball;
Complex   goal,   ownGoal,   midpoint;   

Complex lastBall;

Complex mirrorOffset(0,0);

double ball_size = 0, goal_size = 0, ownGoal_size = 0;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    TIMES & TIMER     //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
elapsedMillis cam_timeout         =  0;
elapsedMillis line_timer          =  0;

elapsedMillis acceleration_timer  =  0;
int acceleration_time             = -1;
elapsedMillis break_timer         =  0;

elapsedMillis kick_lockdown_timer =  0;
elapsedMillis kick_timer          =  0;

elapsedMillis gotBall_timer       =  0;

elapsedMillis button_timer        =  0;

elapsedMillis playing_timer               = 0;
elapsedMillis btDoGameModeSwitch_timer    = 0;
elapsedMillis btJustBecameOffensive_timer = 0;
#define BLOCK_GAMEMODE_SWITCH_MILLIS 1000

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      MATRICIES       //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int waiting_matrix[16][16] = {
                                //0                                 //8
  { 1,  0,  0,  0,  1,  1,  1,    1,    1,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  0,  0,  0,  1,  1,    1,    1,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  0,  0,  0,  0,    1,    1,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  0,  0,  0,    0,    1,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  0,  0,    0,    0,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  0,    0,    0,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  1,    0,    0,  0,  1,  1,  1,  1,  1,    1},
  
  { 1,  1,  1,  1,  0,  0,  0,    0,    0,  0,  0,  1,  1,  1,  1,    1},//0
  
  { 1,  1,  1,  1,  1,  0,  0,    0,    1,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  0,    0,    0,  1,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  0,    0,    0,  0,  1,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  1,    0,    0,  0,  0,  1,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  1,    1,    0,  0,  0,  0,  1,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  1,    1,    1,  1,  0,  0,  0,  1,  1,    1},
  { 1,  1,  1,  1,  1,  1,  1,    1,    1,  1,  1,  0,  0,  0,  1,    1},
  
  { 0,  0,  0,  0,  1,  1,  1,    1,    1,  1,  1,  0,  0,  0,  0,    0}//8

}; 

const int penalty_matrix[16][16] = {
                                //0                                 //8
  { 1,  1,  1,  0,  0,  0,  0,    0,    0,  0,  0,  0,  0,  0,  1,    1},
  { 1,  1,  1,  1,  0,  0,  0,    0,    0,  0,  0,  0,  0,  0,  0,    1},
  { 1,  1,  1,  1,  1,  0,  0,    0,    0,  0,  0,  0,  0,  0,  0,    0},
  { 0,  1,  1,  1,  1,  1,  0,    0,    0,  0,  0,  0,  0,  0,  0,    0},
  { 0,  0,  1,  1,  1,  1,  1,    0,    0,  0,  0,  0,  0,  0,  0,    0},
  { 0,  0,  0,  1,  1,  1,  1,    1,    0,  0,  0,  0,  0,  0,  0,    0},
  { 0,  0,  0,  0,  1,  1,  1,    1,    1,  0,  0,  0,  0,  0,  0,    0},
  
  { 0,  0,  0,  0,  0,  1,  1,    1,    1,  1,  0,  0,  0,  0,  0,    0},//0
  
  { 0,  0,  0,  0,  0,  0,  1,    1,    1,  1,  1,  0,  0,  0,  0,    0},
  { 0,  0,  0,  0,  0,  0,  0,    1,    1,  1,  1,  1,  0,  0,  0,    0},
  { 0,  0,  0,  0,  0,  0,  0,    0,    1,  1,  1,  1,  1,  0,  0,    0},
  { 0,  0,  0,  0,  0,  0,  0,    0,    0,  1,  1,  1,  1,  1,  0,    0},
  { 0,  0,  0,  0,  0,  0,  0,    0,    0,  0,  1,  1,  1,  1,  1,    0},
  { 0,  0,  0,  0,  0,  0,  0,    0,    0,  0,  0,  1,  1,  1,  1,    1},
  { 1,  0,  0,  0,  0,  0,  0,    0,    0,  0,  0,  0,  1,  1,  1,    1},
  
  { 1,  1,  0,  0,  0,  0,  0,    0,    0,  0,  0,  0,  0,  1,  1,    1}//8

}; 



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        OTHERS        //--------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int lastDir = 0, lastSpd = 0, lastRot = 0;

double last_save_speedFactor = 0;

bool btSwitch;

bool play = false;
bool btPlay = false;
bool debug = false;
bool ena_leds = false;


// um zu kompilieren ---------------------------
int X, Y, Z;
//-----------------------------------------------
