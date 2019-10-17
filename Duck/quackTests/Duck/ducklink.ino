#ifdef DL

void setup()
{
  Serial.begin(115200);

  iAm = "duck";
  empty.whoAmI = "duck";
  setupDuck();

  setupDisplay();
  setupLoRa();
  setupPortal();

  #ifdef QUACKPACK
  setupQuack();
  QuackPack = true;
  iAm = "quackpack";
  offline.whoAmI = "quackpack";
  empty.whoAmI = "quackpack";

  Serial.println("quackPack - Setup");
  #endif

  Serial.println("Duck Online");
  u8x8.drawString(0, 1, "Duck Online");
}

void loop()
{

  if(QuackPack == true)
  {
    loopQuack();
  }

   // Parses Civilian Requests into Data Structure
   readData();
   if (offline.fromCiv == 1 && offline.phone != NULL && offline.phone != "")
   {
     Serial.println("Start Send");
     sendPayload(offline);
     Serial.print("Sending Wifi Data from Mama\n");
     offline = empty;
     offline.fromCiv = 0;
   }
}

#endif
