#ifdef DETECTOR
#include "timer.h"

auto timer = timer_create_default(); // create a timer with default settings
bool ponger = false;

int ledR = 25;
int ledG = 2;
int ledB = 4;

void setup() {
  Serial.begin(115200);

  iAm = "duckDetector";
  empty.whoAmI = "duckDetector";
  
  setupDuck();
  setupLoRa();
  setupLED();
  
  timer.every(3000, ping);

  Serial.print("Detector Online");
}

void loop() {
  timer.tick();

  receive(LoRa.parsePacket());

  if(rssi_g != 0) {
    ponger = true;
    ledOn(rssi_g);
    Serial.print(offline.msg);
    rssi_g = 0;
  }
}

bool ping(void *) {
  Serial.print("Ping!");
  if(ponger) {
    ponger = false;
    Serial.print("Pong!");
  }
  else {
    setColor(0,0,25);
  }
  LoRa.beginPacket();
  couple(iamhere, "0");
  LoRa.endPacket();

  return true;
}

void setupLED() {
  ledcAttachPin(ledR, 1); // assign RGB led pins to channels
  ledcAttachPin(ledG, 2);
  ledcAttachPin(ledB, 3);
  
  // Initialize channels 
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);
  ledcSetup(3, 12000, 8);
}

void ledOn(int incoming) {
  Serial.println("Run LED");
  Serial.println(incoming);
  int rssi = incoming;
  //Serial.print(rssi);
  
  if(rssi > -95) {
    setColor(0,50,0);
    Serial.println("Rssi good");
  }
  else if(rssi <= -95 && rssi > -108) {
    setColor(25,0,50);
    Serial.println("Rssi okay");
  }
  else if(rssi <= -108) {
    setColor(50,0,0);
    Serial.println("Rssi bad");
  }
}

void setColor(int red, int green, int blue)
{
  ledcWrite(1, red);
  ledcWrite(2, green);
  ledcWrite(3, blue);  
}


#endif
