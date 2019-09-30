#ifdef DL

void setup()
{
  Serial.begin(115200);

  iAm = "mama-duck";
  empty.whoAmI = "mama-duck";
  // setupDuck();

  // setupDisplay();
  setupLoRa();
  // setupPortal();

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

  if(QuackPack == true)
  {
    Serial.println("MamaQuack - Loop");
    loopQuack();

//    Serial.println("MamaQuack - Payload");
//    sendPayload(empty);
  }

  // ⚠️ Parses Civilian Requests into Data Structure
  // readData();
  // if (offline.fromCiv == 1 && offline.phone != NULL && offline.phone != "")
  // {
  //   Serial.println("Start Send");
  //   sendPayload(offline);
  //   Serial.print("Sending Wifi Data from Mama\n");
  //   offline = empty;
  //   offline.fromCiv = 0;
  // }


  // Sends Duck Stat every 30 minutes
  // sendDuckStat(offline);
}

#endif
