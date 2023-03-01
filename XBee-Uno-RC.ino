#include <SoftwareSerial.h> //setting pins to act as TX and RX
#include <XBee.h>

#define ssRX 2
#define ssTX 3
/* Create object named xbee_ss of the class SoftwareSerial */ 
SoftwareSerial xbee_ss(ssRX, ssTX); /* Define pins for software serial instance named xbee-ss(any name of your choice) to be connected to xbee */
/* ssTx of Arduino connected to Din (pin 3 of xbee) */
/* ssRx of Arduino connected to Dout (pin 2 of xbee) */

XBee xbee = XBee(); //Create an object of class XBee

#define joyX A1
#define joyY A0
#define joySw 7
int xValue;
int yValue;
int bValue;
int motorX;
int motorY;

 
void setup() {
  Serial.begin(9600); /* Define baud rate for serial communication */
  xbee_ss.begin(9600); /* Define baud rate for software serial communication */
  xbee.setSerial(xbee_ss); /* Define serial communication to be used for communication with xbee */
  /* In this case, software serial is used. You could use hardware serial as well by writing "Serial" in place of "xbee_ss" */
  /* For UNO, software serialis required so that we can use hardware serial for debugging and verification */
  /* If using a board like Mega, you can use Serial, Serial1, etc. for the same, and there will be no need for software serial */
  pinMode(joySw, INPUT);
  digitalWrite(joySw, HIGH);
}

void loop() {
  /*xbee_ss.write(inc);
  delay(2000);
  inc=inc+1;*/
  
  xValue = analogRead(joyX); 
  yValue = analogRead(joyY);
  bValue = digitalRead(joySw);
  
  
  //xbee_ss.print(527);
  //xbee_ss.print(analogRead(joyX));
  xbee_ss.print(xValue);
  xbee_ss.print('-');
  xbee_ss.print(yValue);
  xbee_ss.print('=');
  xbee_ss.print(bValue);
  xbee_ss.print('_');


  //Zumo Code
  motorY = map(yValue, 0, 1023, 400, -400);
  motorX = map(xValue, 0, 1023, 400, -400);
  
  

  //*/print the values with to plot or view
  Serial.print("Button:");
  Serial.print(bValue);
  Serial.print("\t");
  
  Serial.print("x:");
  Serial.print(xValue);
  Serial.print("\t");
  Serial.print("y:");
  Serial.print(yValue);
  
  Serial.print("\t");
  Serial.print("MotorX:");
  Serial.print(motorX);
  Serial.print("\t");
  Serial.print("MotorY:");
  Serial.println(motorY);//*/
  
}
