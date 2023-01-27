#include <Wire.h>

#include "person_sensor.h"

#include <ArduinoRS485.h> // the ArduinoDMX library depends on ArduinoRS485
#include <ArduinoDMX.h>

const int universeSize = 9;

// How long to wait between reading the sensor. The sensor can be read as
// frequently as you like, but the results only change at about 5FPS, so
// waiting for 200ms is reasonable.
const int32_t SAMPLE_DELAY_MS = 200;

int offset_x = 128, offset_y = 180;

float min_x = 32, max_x = 64, min_y = 0, max_y = 24;
int min_confidence = 75;
float spot_x = (min_x + max_x) / 2;
float spot_y = (min_y + max_y) / 2;
float correction_factor = 0.05;

void setup() {
  // You need to make sure you call Wire.begin() in setup, or the I2C access
  // below will fail.
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(9600);
  //  while (!Serial);
  delay(500);

  // initialize the DMX library with the universe size
  if (!DMX.begin(universeSize)) {
    Serial.println("Failed to initialize DMX!");
    while (1); // wait for ever
  }

  Serial.println("START");
}

void loop() {
  DMX.beginTransmission();
  DMX.write(1, int(spot_x)); // X axis
  DMX.write(2, int(spot_y)); // Yaxis
  DMX.write(6, 25); // Brightness
  DMX.write(5, 255); // Switch on light
  DMX.write(4, 16); // Gobo selection
  DMX.endTransmission();

  person_sensor_results_t results = {};
  // Perform a read action on the I2C address of the sensor to get the
  // current face information detected.
  if (!person_sensor_read(&results)) {
    Serial.println("No person sensor results found on the i2c bus");
    delay(SAMPLE_DELAY_MS);
    return;
  }
  const person_sensor_face_t* face = &results.faces[0];


  if (face->box_confidence > min_confidence) {
    float face_x = (face->box_left + face->box_right) / 2;
    float face_y = (face->box_top + face->box_bottom) / 2;

    float correction_x = (offset_x - face_x) * correction_factor;
    float correction_y = (offset_y - face_y) * correction_factor;
    spot_x -= correction_x;
    if (spot_x < min_x) spot_x = min_x;
    if (spot_x > max_x) spot_x = max_x;
    spot_y += correction_y;
    if (spot_y < min_y) spot_y = min_y;
    if (spot_y > max_y) spot_y = max_y;

    Serial.print(face->box_confidence);
    Serial.print(" confidence, (");
    Serial.print(face_x);
    Serial.print(", ");
    Serial.print(face_y);
    Serial.print("), ");

    Serial.print(correction_x);
    Serial.print(" ");
    Serial.println(correction_y);

    Serial.print(spot_x);
    Serial.print(" ");
    Serial.println(spot_y);
  }

  delay(SAMPLE_DELAY_MS);
}
