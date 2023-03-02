# Robotics Assignment 1
All code was written, compiled, and uploaded in Arduino IDE 1.8.16
## Mode 1
### Xbee-Uno-RC.ino
The Code for the Aruino for any manual control. 
Paired with both Xbee-Zumo-RC.ino and Zumo-SemiAuti-Prox-Working.ino
The Arduino will send input data from a joystick through the Xbee to the Zumo to later be split and mapped to values. 

### Xbee-Zumo-RC.ino
The code for the Zumo for mode 1
The zumo receives inputs from the Arduino through the Xbee, then maps the values to motor values and will then set motor speeds accordingly. 
## Mode 2
### Zumo-SemiAuto-Prox-Working.ino
The code for the Zumo for mode 2
The Zumo will follow the left line until it either hits a wall or detect a cup/person.
If the Zumo hits a wall, it will revert back to manual control and await a direction to be told to turn (right or left) based off the joystick inputs from the Arduino. 
If the Zumo detects a person/cup directly in front of it, it will beep then turn aproximately 90° right and continue forward until the it detects a line to follow or determines it "hit a wall."
**For this mode to work you will need the Arduino with Xbee-Uno-RC.ino**
## Mode 3
### Zumo-BorderDetect-HugLeft-Prox-Working.ino
The code for the Zumo for mode 3
Similarly to Mode 2, the Zumo will follow lines and detect people/cups however when it determines it hit a wall, it will turn aprozimately 90° right and require no intervention from the user/opperator. 

## Usage Instructions
Every file prefixed with Zumo or including Zumo in the name is to be uploaded to a Zumo 32U4 robot to fully function properly. The File Xbee-Uni-RC.ino is to be uploaded to an Arduino board that is wired to an Xbee using Software Serial ports and a joystick. 
#### Uploading to Zumo
I've found that the best way to upload to the Zumo is by utilizing Arduino IDE 1.8.16 as this will not cause the Zumo to derecognize itself when entering bootloader mode. 

### Once Uploaded
After you've uploaded a sketch to the Zumo, place it within the lines of the maze, ideally with a little overlap of the left sensor, and press the A button on the Zumo while making sure to not lift the front while pressing it. To prevent this you should press down on the front while you press the A button. Once the A button has been released it will begin to navigate the maze/building by following the lines. **For manual mode:** you do not need to do anything except press inputs on the joystick on the arduino.
