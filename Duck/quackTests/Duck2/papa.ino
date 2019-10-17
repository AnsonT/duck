#ifdef PD

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "timer.h"

auto timer = timer_create_default(); // create a timer with default settings

WiFiClientSecure wifiClient;
PubSubClient client(server, 8883, wifiClient);

bool disconnected = false;
double timeOff = 0;

void setup()
{
  Serial.begin(115200);

  iAm = "papa-duck";
  setupDuck();

  pinMode(LED_BUILTIN, OUTPUT);

  setupDisplay();
  setupLoRa();

  // setupPortal(); // Toggled Captive Portal from PapaDuck

  setupWiFi();

  timer.every(10800000, reboot);

  Serial.println("PAPA Online");
  u8x8.drawString(0, 1, "PAPA Online");
}

/**
setupWiFi
Connects to local SSID
*/
void setupWiFi()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(SSID);

  // Connect to Access Poink
  WiFi.begin(SSID, PASSWORD);
  u8x8.drawString(0, 1, "Connecting...");

  while (WiFi.status() != WL_CONNECTED)
  {
    timer.tick(); //Advance timer to reboot after awhile
    delay(500);
    Serial.print(".");
  }

  // Connected to Access Point
  Serial.println("");
  Serial.println("WiFi connected - PAPA ONLINE");
  u8x8.drawString(0, 1, "PAPA Online");
}

/**
setupMQTT
Sets Up MQTT
*/
void setupMQTT()
{
  if (!!!client.connected())
  {
    if(disconnected) {
      timeOff = millis();
    }
    Serial.print("Reconnecting client to "); Serial.println(server);
    while ( ! (ORG == "quickstart" ? client.connect(clientId) : client.connect(clientId, authMethod, token)))
    {
      timer.tick(); //Advance timer to reboot after awhile
      Serial.print(".");
      delay(500);
    }
    if(disconnected) {
      qtest.payload = "Papa disconnected from WiFi for:" + String(millis() - timeOff); //Record that lost wifi and then reconnected
      papaHealth();  
    }
    else {
      qtest.payload = "Papa disconnected from MQTT for:" + String(millis() - timeOff); //Record that lost connection to MQTT and then reconnected
      papaHealth();
    }
    Serial.println();
  }
}

void loop()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("WiFi disconnected, reconnecting to local network: ");
    Serial.print(SSID);
    setupWiFi();
    disconnected = false;
  }
  setupMQTT();

  // ⚠️ Parses Civilian Requests into Data Structure
  readData();
  if (offline.fromCiv == 1 && offline.phone != NULL && offline.phone != "")
  {
    jsonify(offline);
    Serial.print("Parsing Wifi Data");
    offline = empty;
    offline.fromCiv = 0;
  }

  receive(LoRa.parsePacket());
  if(offline.whoAmI == "quackpack")
  {
    quackJson();
    offline.whoAmI = empty.whoAmI;
  }
  else if (offline.fromCiv == 0 && offline.phone != NULL && offline.phone != "")
  {
    offline.path = offline.path + "," + empty.duckID;
    Serial.println(offline.path);
    jsonify(offline);
    //duckData(offline);
    u8x8.setCursor(0, 16);
    u8x8.print("Name: " + offline.fname);
    Serial.print("Parsing LoRa Data");
    offline = empty;
  }

  timer.tick();
}

/**
jsonify
Serializes and Parses stat Struct Values into JSON
@return JSON String
*/
void jsonify(Data offline)
{
  const int bufferSize = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 2 * JSON_OBJECT_SIZE(4);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.createObject();

  root["DeviceID"]        = offline.duckID;
  root["MessageID"]       = offline.messageId;
  root["Payload"]         = offline.fname + "*" + 
                            offline.phone + "*" +
                            offline.street + "*" +
                            offline.occupants + "*" +
                            offline.danger + "*" +
                            offline.vacant + "*" +
                            offline.firstaid + "*" +
                            offline.water + "*" +
                            offline.food + "*" +
                            offline.msg + "*";

  root["path"]            = offline.path;

  offline.path = "";

  String jsonstat;
  root.printTo(jsonstat);
  root.prettyPrintTo(Serial);

  if (client.publish(topic, jsonstat.c_str()))
  {
    Serial.println("Publish ok");
    root.prettyPrintTo(Serial);
    Serial.println("");
  }
  else
  {
    Serial.println("Publish failed");
  }
}

void duckData(Data offline)
{
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4);
  DynamicJsonBuffer jsonBuffer(capacity);

  JsonObject& root = jsonBuffer.createObject();
  root["uuid"]                   = makeId();
  root["linked"]                 = offline.messageId;

  JsonArray& Duck_Data = root.createNestedArray("Duck Data");

  JsonObject& Duck_Stat     = Duck_Data.createNestedObject();
  Duck_Stat["Class"]        = offline.whoAmI;
  Duck_Stat["Duck ID"]      = offline.duckID;
  Duck_Stat["Time on"]      = offline.runTime;
  Duck_Stat["Coordinates"]  = offline.whereAmI;

  String jsonstat;
  root.printTo(jsonstat);
}

String makeId() {
  char items[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  char uuid[9];

  for(int i = 0; i <= 3; i++)
  {
    uuid[i] = items[random(0,35)];
  }
  uuid[9] = '\0';

  String str = String(uuid);

  return  str;

}

void papaHealth() {
  qtest.deviceID = empty.whoAmI;
  qtest.messageID = uuidCreator();
  quackJson();
}

void quackJson()
{
  const int bufferSize = 4 * JSON_OBJECT_SIZE(1);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.createObject();


  root["DeviceID"]        = qtest.deviceID;
  root["MessageID"]       = qtest.messageID;
  root["Payload"]         = qtest.payload;

  root["path"]            = offline.path + "," + empty.duckID;

  offline.path = "";

  String jsonstat;
  root.printTo(jsonstat);
  root.prettyPrintTo(Serial);

  if (client.publish(topic, jsonstat.c_str()))
  {
    Serial.println("Publish ok");
    root.prettyPrintTo(Serial);
    Serial.println("");
  }
  else
  {
    Serial.println("Publish failed");
  }
}

#endif
