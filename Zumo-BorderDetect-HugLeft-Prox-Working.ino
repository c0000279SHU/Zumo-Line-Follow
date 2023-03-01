/** This example uses the Zumo's line sensors to detect the white
border around a sumo ring.  When the border is detected, it
backs up and turns. */

#include <Wire.h>
#include <Zumo32U4.h>

// This might need to be tuned for different lighting conditions,
// surfaces, etc.
#define QTR_THRESHOLD     700  // microseconds

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

bool rightSen;
bool leftSen;
bool midSen;

Zumo32U4ButtonA buttonA;
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
  

  //detect lines
  readLines();

  //if middle sensor turn 90* to the right
  if(midSen){
    turnNintyR();
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
  
  
}
