//2501ZEDD main
//Project written by Eric Nooteboom
//Optimized for Arduino Mega "ADK" 2560

#include <PS4BT.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_NeoPixel.h> 
#include <Adafruit_Soundboard.h>
#include <usbhub.h>

//#ifdef dobogusinclude
//#include <spi4teensy3.h>
//#endif

//#include <SPI.h> //need to find out if I need for breakout serial interfacing communication

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

//---------------------------------------------------------------------------------------------------------------------------------
//Motor/Drive pin numbers
//*NB is Pin 7

const byte motorL2R2SpeedPin = 24; //neck motor
const byte L2R2motorDirPin = 4; //neck turn direction??????

const byte motorL3SpeedPin = 2; //left drive motor
const byte motorR3SpeedPin = 3; //right drive motor
const byte LmotorDirPin = 22; //direction of left drive motor
const byte RmotorDirPin = 23; //direction of left drive motor

const byte LeftShoulderMotorPin = 5; //2-3-2 conversion motors
const byte RightShoulderMotorPin = 6;
const byte LeftShoulderDirPin = 26;
const byte RightShoulderDirPin = 27;

const byte LegLiftMotorPin = 8; //not sure if going to use dc motor or stepper motor for this
const byte LegLiftDirPin = 25; //due to the fact there will be no visual of the foot

//---------------------------------------------------------------------------------------------------------------------------------
//varialbes

//stick drive
int LJoyVal = 0;
int RJoyVal = 0;
int JoyValMax = 255;
int JoyValMid = 255 / 2;
int JoyValMin = 0;
int joyValMidHigh = JoyValMid + 10; //0-20
int joyValMidLow = JoyValMid - 10;

int L2TrigVal = 0;
int R2TrigVal = 0;
int TrigBaseVal = 5; //5-10 ish?????

//dc motors
byte NmotorSpeed = 0;
byte LmotorSpeed = 0;
byte RmotorSpeed = 0;
byte motorSpeedMin = 0; //based on motor 0 - 90 pwm ish
byte motorSpeedMax = 255;


//---------------------------------------------------------------------------------------------------------------------------------
//Servo stuff

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

int servo = 0;
int pulselen;
//int SERVOMIN = 0; //these are not set, defalt values are reminders
//int SERVOMAX = 180; //not used beccuase of cheap servos

//pulselength = map(degrees, 0, 180, SERVOMIN, SERVOMAX); //min-max to degree conversion

#define CLOSED_00   0
#define OPEN_00   950
#define CLOSED_01   0
#define OPEN_01   950
#define CLOSED_02   300   //?holo
#define OPEN_02   520   //?holo

//---------------------------------------------------------------------------------------------------------------------------------
//NeoPixel stuff

#define NUMPIXELS     16
#define NeoPin        46

//---------------------------------------------------------------------------------------------------------------------------------
//Sound

#define SFX_TX          18 
#define SFX_RX          19

#define SFX_RST         28        

SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX); 


//---------------------------------------------------------------------------------------------------------------------------------

void setup() {
  //serial debug for ps4 bt test
  ss.begin(9600);   //baud for sound board
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect for debud monitoring - TAKE OUT
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started")); //Take out after debug

  //motor setup
  pinMode(motorL3SpeedPin, OUTPUT);
  pinMode(LmotorDirPin, OUTPUT);
  pinMode(motorR3SpeedPin, OUTPUT);
  pinMode(RmotorDirPin, OUTPUT);

  pinMode(motorL2R2SpeedPin, OUTPUT);
  pinMode(L2R2motorDirPin, OUTPUT);

  pinMode(LeftShoulderMotorPin, OUTPUT);
  pinMode(RightShoulderMotorPin, OUTPUT);
  pinMode(LeftShoulderDirPin, OUTPUT);
  pinMode(RightShoulderDirPin, OUTPUT);

  pinMode(LegLiftMotorPin, OUTPUT);
  pinMode(LegLiftDirPin, OUTPUT);


  //Servo
  pwm1.begin();
  pwm1.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  pwm2.begin();
  pwm2.setPWMFreq(60);
  //example form the example library
  

  //Pixels
  //pixels.begin();
  
  
  //Sound
  //

}


//---------------------------------------------------------------------------------------------------------------------------------


void loop() {
  Usb.Task();
  unsigned long currentMillis = millis();

  if (PS4.connected()) {

    //test for motor motion/jitter, analog joystick tank drive, forward and reverse with output

    LJoyVal = PS4.getAnalogHat(LeftHatY);
    RJoyVal = PS4.getAnalogHat(RightHatY);

    if (LJoyVal > joyValMidHigh ) { //drive left reverse   ???????????????? dont know direction yet
      LmotorSpeed = map(LJoyVal, joyValMidHigh, JoyValMax, motorSpeedMin, motorSpeedMax);
      MotorReverseL(LmotorSpeed);
    }
    else if (LJoyVal < joyValMidLow ) { //drive left forward   ???????????? dont know direction yet
      LmotorSpeed = map(LJoyVal, joyValMidLow, JoyValMin, motorSpeedMin, motorSpeedMax);
      MotorForwardL(LmotorSpeed);
    }
    else
    {
      LmotorSpeed = 0;
      MotorStopL();
    }

    if (RJoyVal > joyValMidHigh ) { //drive right reverse
      RmotorSpeed = map(RJoyVal, joyValMidHigh, JoyValMax, motorSpeedMin, motorSpeedMax);
      MotorReverseR(RmotorSpeed);
    }
    else if (RJoyVal < joyValMidLow ) { //drive right foward
      RmotorSpeed = map(RJoyVal, joyValMidLow, JoyValMin, motorSpeedMin, motorSpeedMax);
      MotorForwardR(RmotorSpeed);
    }
    else
    {
      RmotorSpeed = 0;
      MotorStopR();
    }
    /*
           Serial.print(F("\r\nLeftHatY: "));            //For Debug purposes
           Serial.print(PS4.getAnalogHat(LeftHatY));
           Serial.print(F("\tRightHatY: "));
           Serial.print(PS4.getAnalogHat(RightHatY));
           Serial.print(F("\tLMotorSpeed: "));
           Serial.print(LmotorSpeed);
           Serial.print(F("\tRMotorSpeed: "));
           Serial.print(RmotorSpeed);
    */

    //---------------------------------------------------------------------------------------------------------------------------------

    //triggers for head rotations,0 - 255 is max for pull

    L2TrigVal = PS4.getAnalogButton(L2);
    R2TrigVal = PS4.getAnalogButton(R2);

    if (R2TrigVal > TrigBaseVal) {
      NmotorSpeed = map(R2TrigVal, 0, 255, motorSpeedMin, motorSpeedMax);
      NeckMotorL(NmotorSpeed);
    }
    else if (L2TrigVal > TrigBaseVal) {
      NmotorSpeed = map(L2TrigVal, 0, 255, motorSpeedMin, motorSpeedMax);
      NeckMotorR(NmotorSpeed);
    }
    else
    {
      NmotorSpeed = 0;
      MotorStop();
    }

    /*
          if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
            Serial.print(F("\r\nL2: "));
            Serial.print(PS4.getAnalogButton(L2));
            Serial.print(F("\tR2: "));
            Serial.print(PS4.getAnalogButton(R2));
            Serial.print(F("\tMotorSpeed: "));
            Serial.print(NmotorSpeed);
            //Serial.print(F("\tTrigVal: "));
            //Serial.print();
          }

    */
    //trigger rumbles
    /*
          if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
            PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
          oldL2Value = PS4.getAnalogButton(L2);
          oldR2Value = PS4.getAnalogButton(R2);
    */
    //---------------------------------------------------------------------------------------------------------------------------------

    //ps robit SPLODE button, for interlock release and overdrive functions
    if (PS4.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));

    }

    //---------------------------------------------------------------------------------------------------------------------------------

    //buttons, rumble and led flash to be replaced by servo opperations.

    if (PS4.getButtonClick(TRIANGLE)) {
      Serial.print(F("\r\nTraingle"));
      //PS4.setRumbleOn(RumbleLow);
      {
        for (pulselen = 50; pulselen < 950; pulselen++) {
          pwm1.setPWM(servo, 0, pulselen);
        }
        delay(2000);

        for (pulselen = 950; pulselen > 50; pulselen--) {
          pwm1.setPWM(servo, 0, pulselen);
        }
        delay(2000);
      }
    }
    if (PS4.getButtonClick(CIRCLE)) {
      Serial.print(F("\r\nCircle"));
      //PS4.setRumbleOn(RumbleHigh);
      {
        for (pulselen = 50; pulselen < 950; pulselen++) {
          pwm1.setPWM(1, 0, pulselen);
        }
        delay(2000);

        for (pulselen = 950; pulselen > 50; pulselen--) {
          pwm1.setPWM(1, 0, pulselen);
        }
        delay(2000);
      }
    }
    if (PS4.getButtonClick(CROSS)) {
      Serial.print(F("\r\nCross"));
      //PS4.setLedFlash(10, 10); // Set it to blink rapidly
      {
        for (pulselen = 50; pulselen < 950; pulselen++) {
          pwm1.setPWM(servo, 0, pulselen);
        }
        delay(2000);

        for (pulselen = 950; pulselen > 50; pulselen--) {
          pwm1.setPWM(servo, 0, pulselen);
        }
        delay(2000);
      }
    }
    if (PS4.getButtonClick(SQUARE)) {
      Serial.print(F("\r\nSquare"));
      //PS4.setLedFlash(0, 0); // Turn off blinking
      {
        for (pulselen = 50; pulselen < 950; pulselen++) {
          pwm1.setPWM(servo, 0, pulselen);
        }
        delay(2000);

        for (pulselen = 950; pulselen > 50; pulselen--) {
          pwm1.setPWM(servo, 0, pulselen);
        }
        delay(2000);
      }
    }

    //---------------------------------------------------------------------------------------------------------------------------------

    //D-pad and colors to be replaced by more servo operations

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

    //---------------------------------------------------------------------------------------------------------------------------------

    //L1, L3 click, R1, R3 click buttons
    
          if (PS4.getButtonClick(L1))	//stop music 
            //Serial.print(F("\r\nL1"));
          if (PS4.getButtonClick(L3))	//volume down
            //Serial.print(F("\r\nL3"));
          if (PS4.getButtonClick(R1))	//resume track/next track
            //Serial.print(F("\r\nR1"));
          if (PS4.getButtonClick(R3))	//volume up
            //Serial.print(F("\r\nR3"));
    
    //---------------------------------------------------------------------------------------------------------------------------------

    //share and options for disconnect controller and 2-3-2 conversion sequence respectively

    if (PS4.getButtonClick(SHARE)) {
      Serial.print(F("\r\nShare"));
      PS4.disconnect();
    }

    if (PS4.getButtonClick(OPTIONS)) {
      Serial.print(F("\r\n2-3-2 Conversion Sequence"));

      //start with interlocking all movment mechanisms


      //Execute, set, and update status flags for position


      //Finish conversion and release interlock


    }


    //---------------------------------------------------------------------------------------------------------------------------------

    //touchpad, change LED color for battery indicator for controller
    
	//green = 100 - 61
	//yellow = 60 - 36
	//red = 35 - 10
	//red flashing override 9-0

          if (PS4.getButtonClick(TOUCHPAD)) {
            //Serial.print(F("\r\nTouchpad"));
            //printTouch = !printTouch;
          }
    
    //---------------------------------------------------------------------------------------------------------------------------------

    //tilt gyro tracking, unutilized
    /*
          if (printAngle) { // Print angle calculated using the accelerometer only
            Serial.print(F("\r\nPitch: "));
            Serial.print(PS4.getAngle(Pitch));
            Serial.print(F("\tRoll: "));
            Serial.print(PS4.getAngle(Roll));
          }
    */
    //---------------------------------------------------------------------------------------------------------------------------------

    //track touch for touchpad, unutilized
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
    //---------------------------------------------------------------------------------------------------------------------------------
  }
}

//---------------------------------------------------------------------------------------------------------------------------------

//Drive Motor Functions
void MotorForwardL( byte Spd)
{
  digitalWrite(LmotorDirPin, HIGH);
  analogWrite(motorL3SpeedPin, Spd);
}

void MotorReverseL( byte Spd)
{
  digitalWrite(LmotorDirPin, LOW);
  analogWrite(motorL3SpeedPin, Spd);
}
void MotorForwardR( byte Spd)
{
  digitalWrite(RmotorDirPin, HIGH);
  analogWrite(motorR3SpeedPin, Spd);
}

void MotorReverseR( byte Spd)
{
  digitalWrite(RmotorDirPin, LOW);
  analogWrite(motorR3SpeedPin, Spd);
}

void MotorStopL()
{
  analogWrite(motorL3SpeedPin, 0);
}

void MotorStopR()
{
  analogWrite(motorR3SpeedPin, 0);
}

//Neck Motor Functions
void NeckMotorL( byte Spd)
{
  digitalWrite(L2R2motorDirPin, HIGH);
  analogWrite(motorR3SpeedPin, Spd);
}

void NeckMotorR( byte Spd)
{
  digitalWrite(L2R2motorDirPin, LOW);
  analogWrite(motorR3SpeedPin, Spd);
}

void MotorStop()
{
  analogWrite(motorL2R2SpeedPin, 0);
}

//---------------------------------------------------------------------------------------------------------------------------------

