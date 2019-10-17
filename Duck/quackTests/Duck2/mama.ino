#ifdef MD
#include "timer.h"

auto timer = timer_create_default(); // create a timer with default settings

String deviceID = "mama_a";

void setup()
{
  Serial.begin(115200);

  iAm = "mama-duck";
  empty.whoAmI = "mama-duck";
  setupDuck();
  offline.duckID = "100";
  empty.duckID = "100";

  // String deviceID = "mama_a";

  //Setup interfaces
  setupDisplay(); //Should probably turn off
  setupLoRa();
  setupPortal();

  #ifdef QUACKPACKSIM
  QuackPack = true;
  setupQuack();
  Serial.println("MamaQuack - Setup");
  #endif

  if(QuackPack == false) timer.every(1800000, imAlive); //Report still running
  timer.every(43200000, reboot);
  //timer.every(10800000, reboot);

  Serial.println("Mama Online");
  u8x8.drawString(0, 1, "Mama Online");
  // sendQuacks(empty.duckID, uuidCreator(), "1"); //Send data
  // LoRa.setSyncWord(0xF4);           // ranges from 0-0xFF, default 0x34, see API docs
}

bool notFromMe(String deviceID, String Path)
{
  for(int i = 0; i < Path.length(); i++)
  {
    if(Path.substring(i, deviceID.length()) == deviceID)
    {

      Serial.println("Line 43 -notFromMe Test - False");
      Serial.println("Path: " + Path.substring(i, deviceID.length()) + " , local Address: " + deviceID);
      return false;
    }
  }
  // qtest.path = qtest.path + deviceID;
  Serial.println("Line 48 -notFromMe Test - True");
  Serial.println("Path: " + Path + " , local Address: " + deviceID);
  return true;
}

void loop()
{
  if(QuackPack == true)
  {
    loopQuack();
  }
  else
  {
    timer.tick();
  }

  //Parses Civilian Requests into Data Structure
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
  // if(offline.whoAmI == "quackpack" && offline.path.indexOf("M") < 0)
  // {
  //   sendQuacks(qtest.deviceID, qtest.messageID, qtest.payload);
  //   // offline.whoAmI = empty.whoAmI;
  //   // offline.path = empty.path;
  // }
  Serial.println("Testing Receive: " + qtest.path);
  if(notFromMe(deviceID, qtest.path) == true)
  {
    qtest.path = qtest.path + deviceID;

    Serial.println("Device ID: " + qtest.deviceID + "\nMessage ID: " + qtest.messageID + "\nPayload: " + qtest.payload);
    sendQuacks(qtest.deviceID, qtest.messageID, qtest.payload);
  }
  else if (offline.fromCiv == 0 && offline.phone != NULL && offline.phone != "" && offline.path.indexOf("M") < 0)
  {
    if(offline.path != NULL)
    {
      offline.path = offline.path + "," + empty.duckID;
    }
    else
    {
      offline.path = empty.duckID;
    }
    sendPayload(offline);
    Serial.print("I'm here");
    delay(random(0,2500));
    sendPayload(offline);
    offline = empty;
  }
}

bool imAlive(void *)
{
  sendQuacks(empty.duckID, uuidCreator(), "1"); //Send data
  Serial.print("alive");
  return true;
}


#endif
