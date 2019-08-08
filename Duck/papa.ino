#ifdef PD

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

WiFiClientSecure wifiClient;
PubSubClient client(server, 8883, wifiClient);

void setup()
{
  Serial.begin(115200);
  aes_init();

  iAm = "papa-duck";
  setupDuck();

  pinMode(LED_BUILTIN, OUTPUT);

  setupDisplay();
  setupLoRa();

  // setupPortal(); // Toggled Captive Portal from PapaDuck

  setupWiFi();

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
    Serial.print("Reconnecting client to "); Serial.println(server);
    while ( ! (ORG == "quickstart" ? client.connect(clientId) : client.connect(clientId, authMethod, token)))
    {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
  }
}

void loop()
{
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
  if (offline.fromCiv == 0 && offline.phone != NULL && offline.phone != "")
  {
    offline.path = offline.path + "," + empty.duckID;
    Serial.println(offline.path);
    jsonify(offline);
    duckData(offline);
    u8x8.setCursor(0, 16);
    u8x8.print("Name: " + offline.fname);
    Serial.print("Parsing LoRa Data");
    offline = empty;
  }

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

  root["uuid"]                 = offline.messageId;

  JsonObject& civilian = root.createNestedObject("civilian");

  JsonObject& civilian_info   = civilian.createNestedObject("info");
  civilian_info["name"]       = offline.fname;
  civilian_info["phone"]      = offline.phone;
  civilian_info["location"]   = offline.street;
  civilian_info["occupants"]  = offline.occupants;

  JsonObject& civilian_status = civilian.createNestedObject("status");
  civilian_status["danger"]   = offline.danger;
  civilian_status["vacant"]   = offline.vacant ;

  JsonObject& civilian_need   = civilian.createNestedObject("needs");
  civilian_need["first-aid"]  = offline.firstaid;
  civilian_need["water"]      = offline.water;
  civilian_need["food"]       = offline.food;
  civilian["message"]         = decrypt(offline.msg, aes_iv);

  root["path"]                = offline.path;

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
  
//  if (client.publish(topic2, jsonstat.c_str()))
//  {
//    Serial.println("Publish ok");
//    root.prettyPrintTo(Serial);
//    Serial.println("");
//  }
//  else
//  {
//    Serial.println("Publish failed");
//  }
}

String makeId() {
  char items[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  char uuid[9];

  for(int i = 0; i <= 3; i++) {
    uuid[i] = items[random(0,35)];
  }
  uuid[9] = '\0';

  String str = String(uuid);
  
  return  str;
  
}



//void publishData(String data)
//{
//  if (client.publish(topic, data.c_str()))
//  {
//    Serial.println("Publish ok");
//    root.prettyPrintTo(Serial);
//    Serial.println("");
//  }
//  else
//  {
//    Serial.println("Publish failed");
//  }
//}

#endif
