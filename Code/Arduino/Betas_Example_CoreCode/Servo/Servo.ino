#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int pulselen;    // variable
int servo = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("0 channel Servo test!");
  pwm.begin();
  pwm.setPWMFreq(60);
}

void loop() {
  for (pulselen = 50; pulselen < 950; pulselen++) {
    pwm.setPWM(servo, 0, pulselen);
  }
  delay(2000);

  for (pulselen = 950; pulselen > 50; pulselen--) {
    pwm.setPWM(servo, 0, pulselen);
  }
  delay(2000);

}
