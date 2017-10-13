#include <PS4BT.h>
#include <usbhub.h>
#include <Servo.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance 

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

//bool printAngle, printTouch;
//uint8_t oldL2Value, oldR2Value;

//drive pin numbers

//const byte joyStickPin4L3 = ????;
//const byte joyStickPinR3 = ????;

const byte motorL2R2SpeedPin = 3; //neck mtr
const byte L2R2motorDirPin = 7; //nect turn direction??????

const byte motorL3SpeedPin = 5; //left drive mtr
const byte motorR3SpeedPin = 6; //right drive mtr
const byte LmotorDirPin = 2; //direction of left drive mtr
const byte RmotorDirPin = 4; //direction of left drive mtr


//varialbes 

  //stick 
  //int JoyVal = 0;
  int JoyValMax = 1023;
  int JoyValMid = 512;
  int JoyvalMin = 0;
  int joyValMidHigh = JoyValMid + 40;
  int joyValMidLow = JoyValMid - 40;

  //dc mtr
  byte motorSpeed = 0;
  byte motorSpeedMin = 0; //based on motor 0 - 90 pwm ish
  byte motorSpeedMax = 255;


void setup() {
//serial debug for ps4 bt test
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect for debud monitoring - TAKE OUT
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started")); //Take out after debug
  
//mtr setup
  pinMode(motorL3SpeedPin,OUTPUT);
  pinMode(LmotorDirPin,OUTPUT);
  //pinMode(,OUTPUT);
  //pinMode(,OUTPUT);
  
}
void loop() {
  Usb.Task();

//code below
  if (PS4.connected()) {
    
//test for jtr, analog joystick tank drive, forward nad revers with output for driver
    if (/*PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || */PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || /*PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 ||*/ PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117) {
      
      if(PS4.getAnalogHat(LeftHatY) > joyValMidHigh ){ //drive left reverse
          motorSpeed = map();
          
        }
      if else(PS4.getAnalogHat(LeftHatY) < joyValMidLow ){ //drive left forward
          motorSpeed = map(PS4.getAnalogHat(LeftHatY), joyValMidHigh, JoyValMax);
          
        } 
      else , motorSpeedMin
      {
        analogWrite(/*motorSpeedPin*/, 0); 
      }
   /*   
      if(PS4.getAnalogHat(RightHatY) > joyValMidHigh ){ //drive right reverse
          motorSpeed = map();
        } 
      if(PS4.getAnalogHat(RightHatY) < joyValMidLow ){ //drive right foward
          motorSpeed = map();
        } 
      else 
      {
        analogWrite(, 0); 
      }
    */  
      
      //Serial.print(F("\r\nLeftHatX: "));
      //Serial.print(PS4.getAnalogHat(LeftHatX));
      Serial.print(F("\r\nLeftHatY: "));
      Serial.print(PS4.getAnalogHat(LeftHatY));
      //Serial.print(F("\tRightHatX: "));
      //Serial.print(PS4.getAnalogHat(RightHatX));
      Serial.print(F("\tRightHatY: "));
      Serial.print(PS4.getAnalogHat(RightHatY));
    }
    
  //triggers for head rotations 
/*
    if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
      Serial.print(F("\r\nL2: "));
      Serial.print(PS4.getAnalogButton(L2));
      Serial.print(F("\tR2: "));
      Serial.print(PS4.getAnalogButton(R2));
    }
*/    

  //trigger rumbles 
/*
    if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
      PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
    oldL2Value = PS4.getAnalogButton(L2);
    oldR2Value = PS4.getAnalogButton(R2);
*/

    if (PS4.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS4.disconnect();
    }
//ps disconnect above and buttons, rumble and led flash
/*    
    else {
      if (PS4.getButtonClick(TRIANGLE)) {
        Serial.print(F("\r\nTraingle"));
        PS4.setRumbleOn(RumbleLow);
      }
      if (PS4.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        PS4.setRumbleOn(RumbleHigh);
      }
      if (PS4.getButtonClick(CROSS)) {
        Serial.print(F("\r\nCross"));
        PS4.setLedFlash(10, 10); // Set it to blink rapidly
      }
      if (PS4.getButtonClick(SQUARE)) {
        Serial.print(F("\r\nSquare"));
        PS4.setLedFlash(0, 0); // Turn off blinking
      }
*/

//D-pad and colors 
/*
      if (PS4.getButtonClick(UP)) {
        Serial.print(F("\r\nUp"));
        PS4.setLed(Red);
      } if (PS4.getButtonClick(RIGHT)) {
        Serial.print(F("\r\nRight"));
        PS4.setLed(Blue);
      } if (PS4.getButtonClick(DOWN)) {
        Serial.print(F("\r\nDown"));
        PS4.setLed(Yellow);
      } if (PS4.getButtonClick(LEFT)) {
        Serial.print(F("\r\nLeft"));
        PS4.setLed(Green);
      }
*/

//L1,L3,R1,R3 buttons
/*
      if (PS4.getButtonClick(L1))
        Serial.print(F("\r\nL1"));
      if (PS4.getButtonClick(L3))
        Serial.print(F("\r\nL3"));
      if (PS4.getButtonClick(R1))
        Serial.print(F("\r\nR1"));
      if (PS4.getButtonClick(R3))
        Serial.print(F("\r\nR3"));
*/

  //share and options
/*
      if (PS4.getButtonClick(SHARE))
        Serial.print(F("\r\nShare"));
      if (PS4.getButtonClick(OPTIONS)) {
        Serial.print(F("\r\nOptions"));
        printAngle = !printAngle;
      }
*/

  //touchpad 
/*      
      if (PS4.getButtonClick(TOUCHPAD)) {
        Serial.print(F("\r\nTouchpad"));
        printTouch = !printTouch;
      }
*/

  //tilt for options
/*
      if (printAngle) { // Print angle calculated using the accelerometer only
        Serial.print(F("\r\nPitch: "));
        Serial.print(PS4.getAngle(Pitch));
        Serial.print(F("\tRoll: "));
        Serial.print(PS4.getAngle(Roll));
      }
*/
      
  //touch for touchpad   
/*
      if (printTouch) { // Print the x, y coordinates of the touchpad
        if (PS4.isTouching(0) || PS4.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
          Serial.print(F("\r\n"));
        for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
          if (PS4.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
            Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
            Serial.print(PS4.getX(i));
            Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
            Serial.print(PS4.getY(i));
            Serial.print(F("\t"));
          }
        }
      }
*/
      
    }
  }
