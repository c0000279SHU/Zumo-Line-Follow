#include <Wire.h>
#include <Zumo32U4.h>

// This might need to be tuned for different lighting conditions,
// surfaces, etc.
#define QTR_THRESHOLD     700  // microseconds

// These might need to be tuned for different motor types.
#define REVERSE_SPEED     150  // 0 is stopped, 400 is full speed
#define TURN_SPEED        100
#define TURN_DURATION     100  // ms

int FORWARD_SPEED = 100;
int HALF_SPEED = FORWARD_SPEED/2;
int REVERSE_DURATION = 100;
int WALL_DURATION = 200;
int WALL_SPEED = 300;
int SLOW_TURN = 100;

int forwardCount;
int turnCount;
int turnDirection;

bool rightSen;
bool leftSen;
bool midSen;
bool initManualMode;

//----------------RC vars------------------
int deadzone = 30;
int deadzoneN = deadzone*-1;
int maxSpeed=300;
int turnSpeed=150;
String rec;
int xValue;
int yValue;
int bValue;
int motorX;
int motorY;
int incomingByte = 0; // for incoming serial data
bool xDeadzone;
bool yDeadzone;

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonA buttonB;
Zumo32U4ButtonA buttonC;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

#define NUM_SENSORS 3
unsigned int lineSensorValues[NUM_SENSORS];

void waitForButtonAndCountDown()
{
  ledYellow(1);
  buttonA.waitForButton();
  ledYellow(0);
}

void setup()
{
  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();
  waitForButtonAndCountDown();

  turnDirection=-1;
  initManualMode=false;

  Serial1.begin(9600);
  Serial.begin(9600);
}

void turnNintyR(){
  buzzer.playNote(NOTE_G(3), 200, 15);
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(REVERSE_DURATION);
  motors.setSpeeds(WALL_SPEED, -WALL_SPEED);
  delay(WALL_DURATION);
}
void turnNintyL(){
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(REVERSE_DURATION);
  motors.setSpeeds(-WALL_SPEED, WALL_SPEED);
  delay(WALL_DURATION);
}
void uturn(){
  buzzer.playNote(NOTE_G(5), 200, 15);
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(REVERSE_DURATION);
  motors.setSpeeds(WALL_SPEED, -WALL_SPEED);
  delay(WALL_DURATION*2);
}


void readLines(){
  lineSensors.read(lineSensorValues);
  rightSen = lineSensorValues[NUM_SENSORS - 1] > QTR_THRESHOLD;
  leftSen = lineSensorValues[0] > QTR_THRESHOLD;
  midSen = lineSensorValues[1] > QTR_THRESHOLD;
}


//-----------------------LOOP-------------------------
void loop()
{
  //start/pause
  if (buttonA.isPressed())
  {
    motors.setSpeeds(0, 0);
    buttonA.waitForRelease();
    waitForButtonAndCountDown();
  }

  //if not in manual mode -> auto mode
  if(!initManualMode){

  //if a turn direction was sent (via RC) turn that direction 
  if(turnDirection==0||turnDirection==1){
    if(turnDirection==1){turnNintyR();turnDirection=-1;}
    else if(turnDirection==0){turnNintyL();turnDirection=-1;}
  }
  else{

  //detect lines
  readLines();

  //if middle sensor enter manual control
  if(midSen){
    buzzer.playNote(NOTE_G(5), 200, 15);
    initManualMode=true;
    motors.setSpeeds(0,0);
    delay(500);
  }

  //follow left line
  else if(leftSen&&!midSen){
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED-40);
    //delay(50);
  }

  //turn left then move forward until line detected
  else if(rightSen&&!leftSen&&!midSen){
    motors.setSpeeds(-FORWARD_SPEED,FORWARD_SPEED);
    delay(200);
    motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
    do{
      readLines();
    }while(!midSen&&!leftSen&&!rightSen);
  }

  //if all 3 detected uturn 
  //--should never occur--
  else if(leftSen&&rightSen&&midSen){
    uturn();
  }
  
  //if nothing detected
  else if(!leftSen&&!midSen&&!rightSen){
    
    //turn until you find something
    while(!leftSen&&!midSen&&!rightSen&&turnCount<100){
      readLines();
      motors.setSpeeds(-SLOW_TURN, SLOW_TURN);
      turnCount+=1;
    }
    
    //if turning TOO much, move forward
    if(turnCount>=100){
      turnCount=0;
      motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
      //delay(50);
    }
  }

  //read proximity Sensors
  proxSensors.read();
  int pLeftSen = proxSensors.countsLeftWithLeftLeds();
  int pCenLeftSen = proxSensors.countsFrontWithLeftLeds();
  int pCenRightSen = proxSensors.countsFrontWithRightLeds();
  int pRightSen = proxSensors.countsRightWithRightLeds();

  //if detect in front -> turn 90* and continue until you see a line
  if(pCenLeftSen==6&&pCenRightSen==6){
    turnNintyR();
    motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
    do{
      readLines();
    }while(!midSen&&!leftSen&&!rightSen);
  }
  //if detect to right or left -> beep
  else if(pLeftSen>=5||pRightSen>=5){
    buzzer.playNote(NOTE_G(5), 200, 15);
  }
  
  }}//end auto mode
  else{
  //enter manual input
  if (Serial1.available() > 0) {
  // read the incoming byte:
  incomingByte = Serial1.read();

  char in = (char)incomingByte;   
  rec.concat(in);

  //strips "line" into values
  if(rec.indexOf('_')!=-1){
  xValue=rec.substring(0,rec.indexOf('-')).toInt();
  yValue=rec.substring(rec.indexOf('-')+1,rec.indexOf('=')).toInt();
  bValue=rec.substring(rec.indexOf('=')+1,rec.indexOf('_')).toInt();
  rec="";
  Serial.print(xValue);Serial.print(" "+yValue);Serial.println(rec);
  //if(bValue=="1"){Serial.println("Button is Pressed");}
  motorY = map(yValue, 0, 1023, turnSpeed, -turnSpeed);
  motorX = map(xValue, 0, 1023, maxSpeed, -maxSpeed);

  //determines deadzone
  xDeadzone=motorX<deadzone && motorX>deadzoneN;
  yDeadzone=motorY<deadzone && motorY>deadzoneN;

  if(yDeadzone){motors.setSpeeds(0,0);delay(10);}
  
  //y out of deadzone
  else if(!yDeadzone){
    if(motorY>deadzone){turnDirection=1; initManualMode=false;}
    else if(motorY<deadzoneN){turnDirection=0; initManualMode=false;}
  }
  
  }}}
}
