//#Set to play only using the RX (from microcontroller to serial bus) on UNO
#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"

#define SFX_TX 5 //not needed
#define SFX_RX 6
// Connect to the RST pin on the Sound Board
#define SFX_RST 4//not needed but could use for a manual reset

// we'll be using software serial
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

void setup() {
  ss.begin(9600);
}

void loop() {
sfx.playTrack("YES     OGG"); //soundfile *capitols only
delay(4000);
sfx.playTrack("NO      OGG"); //soundfile
delay(4000);
}
