// Test multitasking

// TEST VALEUR PARTAGEE

int sharedVal;

TaskHandle_t hTaskOne;
TaskHandle_t hTaskTwo;

void taskOne (void *args) {
  while (true) {
    Serial.printf("[%0d] Task One  : Running on core %d\n",sharedVal,xPortGetCoreID());
    delay (1000);
  }
}

void taskTwo (void *args) {
  while (true) {
    sharedVal+=10;
    Serial.printf("[%0d] Task Two  : Running on core %d\n",sharedVal,xPortGetCoreID());
    delay (10000);
  }
}

void setup() {
  pinMode(13,OUTPUT);
	Serial.begin(115200);
  delay(1000);
  Serial.println("Setup in progress...");
  sharedVal=0;
  Serial.println("Start task One");
  xTaskCreatePinnedToCore(
    taskOne,
    "task01",
    1000,
    NULL,
    0,
    &hTaskOne,
    0);

  Serial.println("Start task Two");
  xTaskCreatePinnedToCore(
    taskTwo,
    "task02",
    1000,
    NULL,
    0,
    &hTaskTwo,
    1);
    
  Serial.println("SETUP DONE");
}

void loop() {
    Serial.printf("[%0d] Main Task : Running on core %d\n",++sharedVal,xPortGetCoreID());
  digitalWrite(13,LOW);
	delay(2000);
  digitalWrite(13,HIGH);
  delay(2000);
}
