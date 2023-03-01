#include <Zumo32U4.h>

//Serial1 communicates over XBee
//Serial communicates over USB cable
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

int alt;
bool xDeadzone;
bool yDeadzone;
Zumo32U4Motors motors;
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  int incomingByte = 0; // for incoming serial data
  
  // send data only when you receive data:
  if (Serial1.available() > 0) {
  // read the incoming byte:
  incomingByte = Serial1.read();

  char in = (char)incomingByte;   
  rec.concat(in);

  if(rec.indexOf('_')!=-1){
  xValue=rec.substring(0,rec.indexOf('-')).toInt();
  yValue=rec.substring(rec.indexOf('-')+1,rec.indexOf('=')).toInt();
  bValue=rec.substring(rec.indexOf('=')+1,rec.indexOf('_')).toInt();
  rec="";
  //Serial.print(xValue);Serial.print(" "+yValue);Serial.println(rec);
  //if(bValue=="1"){Serial.println("Button is Pressed");}
  motorY = map(yValue, 0, 1023, turnSpeed, -turnSpeed);
  motorX = map(xValue, 0, 1023, maxSpeed, -maxSpeed);

  //motors.setSpeeds(xValue,xValue);
  //*
  
  xDeadzone=motorX<deadzone && motorX>deadzoneN;
  yDeadzone=motorY<deadzone && motorY>deadzoneN;

  //both in deadzone
  if(xDeadzone && yDeadzone){motors.setSpeeds(0,0);delay(10);}
  //x out of deadzone
  else if(!xDeadzone && yDeadzone){
    if(motorX>deadzone){motors.setSpeeds(motorX, motorX); delay(10);}
    else if(motorX<deadzoneN){motors.setSpeeds(motorX, motorX); delay(10);}
  }
  //y out of deadzone
  else if(!yDeadzone && xDeadzone){
    alt=motorY*-1;
    if(motorY>deadzone){motors.setSpeeds(motorY,-motorY); delay(10);}
    else if(motorY<deadzoneN){motors.setSpeeds(motorY,-motorY); delay(10);}
  }
  //both out of deadzone
  else{
    motors.setSpeeds(motorX, motorY);
  }
  //*/
  //printing
  Serial.print("MotorX:");
  Serial.print(motorX);
  Serial.print("\t");
  Serial.print("MotorY:");
  Serial.print(motorY);
  Serial.print("\t");
  Serial.print(motorY*-1);
  Serial.print("\t");
  Serial.print("Button:");
  Serial.println(bValue);
  }
    
}
//else{Serial.println("Serial1 not available"); delay(500);}
}
