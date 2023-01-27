/*
  DMX Blink

  This sketch toggles the value of DMX channel 1 between 255 and 0.

  Circuit:
   - DMX light
   - MKR board
   - MKR 485 shield
     - ISO GND connected to DMX light GND (pin 1)
     - Y connected to DMX light Data + (pin 2)
     - Z connected to DMX light Data - (pin 3)
     - Jumper positions
       - Z \/\/ Y set to ON

  created 5 July 2018
  by Sandeep Mistry
*/

#include <ArduinoRS485.h> // the ArduinoDMX library depends on ArduinoRS485
#include <ArduinoDMX.h>

const int universeSize = 9;
unsigned long last_update = millis();

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial);

  // initialize the DMX library with the universe size
  if (!DMX.begin(universeSize)) {
    Serial.println("Failed to initialize DMX!");
    while (1); // wait for ever
  }
  pinMode(9, OUTPUT);
}

int x = 128;
int y = 128;

void loop() {
  // set channel 1 value to 255
  DMX.beginTransmission();
  DMX.write(1, x); // X axis
  DMX.write(2, y); // Yaxis
  DMX.write(6, 25); // Brightness
  DMX.write(5, 255); // Switch on light
  DMX.write(4, 16); // Gobo selection
  //  DMX.write(5, 10);
  DMX.endTransmission();


  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);

  //  digitalWrite(9, HIGH);

  delaySerial(1000);
  //
  //  // set channel 1 value to 0
  //  DMX.beginTransmission();
  //  DMX.write(1, 0);
  //  DMX.write(2, 0);
  //  //  DMX.write(5, 0);
  //  DMX.endTransmission();
  //  digitalWrite(9, LOW);
  //
  //  delaySerial(1000);

}

void delaySerial(int del) {
  while (millis() - last_update < del) {
    if (Serial.available() > 0) {
      delay(100);
      x = Serial.parseInt();
      y = Serial.parseInt(SKIP_ALL);
      while(Serial.available())Serial.read();
      //      Serial.println(Serial.parseInt());


      // set channel 1 value to 255
      //      DMX.beginTransmission();
      //      DMX.write(ch, val);
      //      DMX.endTransmission();
    }
  }
  last_update = millis();
}
