#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
 
#define RATIO 2
SSD1306  display(0x3c,23,22);
 
const int analogInPin = A13;  
int sensorValue = 0;       
void setup() {
 Serial.begin(115200);
  Serial.println();
  Serial.println("ready");
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
}
 
void loop() {
  sensorValue = analogRead(analogInPin);
  Serial.println(sensorValue);
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, String(sensorValue));
  display.setFont(ArialMT_Plain_16); display.drawString(0,16, String(sensorValue/(4096.0/3.29)));  display.setFont(ArialMT_Plain_10);  display.drawString(50,16+4, "V  (input A13)");
  display.setFont(ArialMT_Plain_16); display.drawString(0,32, String(RATIO*sensorValue/(4096.0/3.29)));  display.setFont(ArialMT_Plain_10);   display.drawString(50,32+4, "V  (vbat)");
  display.display();  
  delay(1000);
}
