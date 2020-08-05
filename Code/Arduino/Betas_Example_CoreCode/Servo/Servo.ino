#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41); //fuking stupid, its offset by one so 0xA0 = 0x41 (**dumb)

int num;
int pulselen;    // variable

void setup() {
  Serial.begin(9600);
  Serial.println("0 channel Servo test!");
  pwm1.begin();
  pwm1.setPWMFreq(60);
}

void loop() {
 for(num = 0;num < 6;num++){
  for (pulselen = 49; pulselen < 950; pulselen++) {
    pwm1.setPWM(num, 0, pulselen);
  }
  delay(2000);

  for (pulselen = 951; pulselen > 50; pulselen--) {
    pwm1.setPWM(num, 0, pulselen);
  }
  delay(2000);
}
  Serial.println("loop");
  num = 0;
}
