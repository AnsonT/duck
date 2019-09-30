#ifdef MD

void setup()
{
  Serial.begin(115200);

  iAm = "mama-duck";
  empty.whoAmI = "mama-duck";
  setupDuck();
  setupDisplay();
  setupLoRa();
  setupPortal();

  // if(QuackPack == true)
  // {
  //   Serial.println("MamaQuack - Setup");
  //   setupQuack();
  // }

  #ifdef QUACKPACK
    setupQuack();
    Serial.println("MamaQuack - Setup");
  #endif


  // Serial.println("Mama Online");
  // u8x8.drawString(0, 1, "Mama Online");
}

void loop()
{

  // if(QuackPack == true)
  // {
  //   Serial.println("MamaQuack - Loop");
  //   loopQuack();
  //
  //   Serial.println("MamaQuack - Payload");
  //   sendPayload(empty);
  // }

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
  if(offline.whoAmI == "quackpack")
  {
    LoRa.beginPacket();
    for(int i = 0; i < PAYLOADSIZE; i++)
    {
      couple(quack_B, quackArray[i]);
    }
    byte b = LoRa.random();
    LoRa.endPacket();
  }
  else if (offline.fromCiv == 0 && offline.phone != NULL && offline.phone != "" && offline.path.indexOf(empty.duckID) < 0) {
    offline.path = offline.path + "," + empty.duckID;
    sendPayload(offline);
    Serial.print("I'm here");
    delay(random(0,2500));
    sendPayload(offline);
    offline = empty;
  }

  // Sends Duck Stat every 30 minutes
  sendDuckStat(offline);
}

#endif
