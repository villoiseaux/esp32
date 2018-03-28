#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "OLEDDisplay.h"


QueueHandle_t queue;

SSD1306  display(0x3c,23,22);

TaskHandle_t hCollectTask;

// define playload sent over queue

#define DATASIZE 128
#define QUEUSIZE 5

struct Spectrum {
  unsigned long timestamp;
  unsigned char data[DATASIZE];
};


void collectTask (void *args) {
  struct Spectrum mesure;
  while (true) {
    mesure.timestamp=millis();
    for (int i=0; i<DATASIZE; i++) {
      mesure.data[i]=i;
    }
    Serial.printf("COLLECTER [core# %d] Collect task iteration\n",xPortGetCoreID());    
    xQueueSend(queue,(void*)&mesure,  (TickType_t)0 );
    Serial.printf("COLLECTER [core# %d] data sent (timestamp is %d ms)\n",xPortGetCoreID(),mesure.timestamp);    
    delay (2000);
  }
}


void setup() {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0,0,"ESP multitask OLED reset!!!!");
  display.display();    
	Serial.begin(115200);
  delay(1000);
  Serial.println("SETUP in progress...");
  Serial.println("SETUP collect task");
  queue = xQueueCreate(QUEUSIZE, sizeof(Spectrum));

  xTaskCreatePinnedToCore(
    collectTask,
    "collect",
    4096,
    NULL,
    1,
    &hCollectTask,
    0);
  
  Serial.println("SETUP DONE");
}

void loop() {
  struct Spectrum mesure;
  if (queue != NULL){    
    if (xQueueReceive(queue, &mesure, (TickType_t)10) == pdTRUE){
      Serial.printf("RECEIVER  [core# %d] Someting to read in the queue:\n",xPortGetCoreID());
      Serial.printf("RECEIVER  [core# %d] received timestamp %d\n",xPortGetCoreID(), mesure.timestamp);
      display.clear();
      for (int i=0; i<DATASIZE; i++) {
        Serial.printf("%d,",mesure.data[i]);       
        display.drawVerticalLine(i,64-map(mesure.data[i],0,256,0,64), 64);
      }
      Serial.print("\n");
      display.setFont(ArialMT_Plain_10); 
      display.drawString(0,0,String(mesure.timestamp));
      display.display();  
    }
  }
}
