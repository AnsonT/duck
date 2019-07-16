#ifdef MD

void setup()
{
  Serial.begin(115200);

  iAm = "mama-duck";
  setupDuck();

  setupDisplay();
  setupLoRa();
  setupPortal();

  Serial.println("Mama Online");
  //u8x8.drawString(0, 1, "Mama Online");

  //Start BLE setup
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 30 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
}

void loop()
{
  // ⚠️ Parses Civilian Requests into Data Structure
  readData();
  if (offline.fromCiv == 1 && offline.phone != NULL && offline.phone != "")
  {
    Serial.println("Start Send");
    sendPayload(offline);
    Serial.print("Sending Wifi Data from Mama\n");
    offline.fromCiv = 0;
  }

  receive(LoRa.parsePacket());
  //  strstr(offline.path.toCharArray, empty.duckID) != NULL
  if (offline.fromCiv == 0 && offline.phone != NULL && offline.phone != "") {
    offline.path = offline.path + "," + offline.duckID;
    sendPayload(offline);
    Serial.print("I'm here");
    delay(random(0,2500));
    sendPayload(offline);
  }

  // Sends Duck Stat every 30 minutes
  sendDuckStat(offline);

  //Start BLE
  if (doConnect == true) {
   if (connectToServer(*pServerAddress)) {
    Serial.println("We are now connected to the BLE Server.");
     connected = true;
   } else {
     Serial.println("We have failed to connect to the server; there is nothin more we will do.");
   }
   doConnect = false;
 }
}

#endif
