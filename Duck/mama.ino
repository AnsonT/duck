#ifdef MD

int smokeA0 = 39;
// Your threshold value
int sensorThres = 800;
int count = 5;

void setup()
{
  Serial.begin(115200);

  iAm = "mama-duck";
  empty.whoAmI = "mama-duck";
  setupDuck();

  setupDisplay();
  setupLoRa();
  setupPortal();

  Serial.println("Mama Online");
  u8x8.drawString(0, 1, "Mama Online");

  //Setup Gas
  pinMode(smokeA0, INPUT);
}

void loop()
{
  int analogSensor = analogRead(smokeA0);
  // ⚠️ Parses Civilian Requests into Data Structure
  readData();
  if (offline.fromCiv == 1 && offline.phone != NULL && offline.phone != "")
  {
    Serial.println("Start Send");
    sendPayload(offline);
    Serial.print("Sending Wifi Data from Mama\n");
    offline = empty;
    offline.fromCiv = 0;
  }

  receive(LoRa.parsePacket());
  //  strstr(offline.path.toCharArray, empty.duckID) != NULL
  if (offline.fromCiv == 0 && offline.phone != NULL && offline.phone != "" && offline.path.indexOf(empty.duckID) < 0) {
    offline.path = offline.path + "," + empty.duckID;
    sendPayload(offline);
    Serial.print("I'm here");
    delay(random(0,2500));
    sendPayload(offline);
    offline = empty;
  }

  // Sends Duck Stat every 30 minutes
  //sendDuckStat(offline);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    offline.fname = analogSensor;
    offline.messageId = "Sensor";
    offline.fromCiv = 1;
    offline.phone = analogSensor;
    sendPayload(offline);
  }
  else
  {

  }
  
  if(count >= 5) {
    offline.fname = analogSensor;
    offline.messageId = "Sensor";
    offline.fromCiv = 1;
    offline.phone = analogSensor;
    offline.path = offline.duckID + ",";
    sendPayload(offline);
    offline = empty;
    count = 0;
  } else {
    count++;
  }
  
}

#endif
