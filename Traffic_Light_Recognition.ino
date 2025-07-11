#include <Wire.h>
#include "Adafruit_TCS34725.h" 
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);
SoftwareSerial mySerial(10, 11);
DFRobotDFPlayerMini mp3;

int lastPlayed = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  if (!tcs.begin()) {
    Serial.println("Sensor not found! Check wiring.");
    while (1);
  }
  Serial.println("RGB Sensor Connected!");

  if (!mp3.begin(mySerial)) {
    Serial.println("DFPlayer Mini not found! Check wiring.");
    while (1);
  }
  Serial.println("MP3 Module Connected!");
  mp3.volume(30);
}

void loop() {
  uint16_t red, green, blue, clear;
  tcs.getRawData(&red, &green, &blue, &clear);

  float r = (clear > 0) ? ((float)red / clear * 255.0) : 0;
  float g = (clear > 0) ? ((float)green / clear * 255.0) : 0;
  float b = (clear > 0) ? ((float)blue / clear * 255.0) : 0;

  Serial.print("Red: "); Serial.print(r);
  Serial.print(" Green: "); Serial.print(g);
  Serial.print(" Blue: "); Serial.println(b);

  if (r > 120 && r > g * 1.3 && r > b * 1.3) {
    Serial.println("Red Light Detected");
    if (lastPlayed != 1) {
      mp3.play(1);
      lastPlayed = 1;
    }
  } else if (g > 120 && g > r * 1.3 && g > b * 1.3) {
    Serial.println("Green Light Detected");
    if (lastPlayed != 2) {
      mp3.play(2);
      lastPlayed = 2;
    }
  } else if (r > 100 && g > 100 && r > b * 1.2 && g > b * 1.2) {
    Serial.println("Yellow Light Detected");
    if (lastPlayed != 3) {
      mp3.play(3);
      lastPlayed = 3;
    }
  } else {
    Serial.println("No Traffic Light Color Detected");
    lastPlayed = 0;
  }

  Serial.println("-------------------------");
  delay(1000);
}
