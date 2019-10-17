#ifdef todo

// Test

if (byteCode == user_ID)
{
  offline.whoAmI = "quackpack";

  qtest.deviceID  = readMessages(mLength);
  Serial.print(qtest.deviceID);
}
else if(byteCode == message_ID) {
  qtest.messageID = readMessages(mLength);
  Serial.print(qtest.messageID);
}
else if(byteCode == quacket_B) {
  qtest.payload = readMessages(mLength);
  Serial.print(qtest.payload);
}

if(offline.whoAmI == "quackpack")
{
  sendQuacks(qtest.deviceID, qtest.messageID, qtest.payload);
  offline.whoAmI = empty.whoAmI;
}
else if (offline.fromCiv == 0 && offline.phone != NULL && offline.phone != "" && offline.path.indexOf(empty.duckID) < 0)
{
  offline.path = offline.path + "," + empty.duckID;
  sendPayload(offline);
  Serial.print("I'm here");
  delay(random(0,2500));
  sendPayload(offline);
  offline = empty;
}

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your hardware/network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(172, 16, 0, 100);
IPAddress server(172, 16, 0, 2);

void callback(char* topic, byte* payload, unsigned int length)
{
  // handle message arrived
}

EthernetClient ethClient;
PubSubClient client(ethClient);

long lastReconnectAttempt = 0;

boolean reconnect()
{
  if (client.connect("arduinoClient"))
  {
    // Once connected, publish an announcement...
    client.publish("outTopic","hello world");
    // ... and resubscribe
    client.subscribe("inTopic");
  }
  return client.connected();
}

boolean reconnectTest()
{
    if (client.connect(clientId, authMethod, token))
    {
      // Once connected, publish an announcement...
      client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    }
    return client.connected();
}

void setup()
{
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(1500);
  lastReconnectAttempt = 0;
}

void sendQuacks(String deviceID, String messageID, String payload)
{
  if(payload.length() > 140)
  {
    Serial.println("The length of the payload is too long.");
  }
  else
  {
    Serial.println("Sending Payload");
    LoRa.beginPacket();
    couple(user_ID, deviceID);
    couple(message_ID, messageID);
    couple(quacket_B, payload);
    couple(path_Q, qtest.path + "," + deviceID);
    LoRa.endPacket();
  }
}

if(qtest.deviceID != NULL and notfromme == true)
{
  sendQuacks(qtest.deviceID, qtest.messageID, qtest.payload);

}

if(qtest.deviceID != NULL && notFromMe(qtest.deviceID, qtest.path) == true)
{
  sendQuacks(qtest.deviceID, qtest.messageID, qtest.payload);
  qtest
}

bool notFromMe(String deviceID, String Path)
{
  Serial.println("Hello from notFromMe");

  for(int i = 0; i < Path.length(); i++)
  {
    if(Path.substring(i, deviceID.length()) == deviceID)
    {
      Serial.println("Hello from notFromMe - False");
      return false;
    }
  }
  Serial.println("Hello from notFromMe - True");
  return true;
}


void loop()
{
  if (!client.connected())
  {
    long now = millis();
    if (now - lastReconnectAttempt > 5000)
    {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnectTest())
      {
        Serial.print("Reconnecting client to "); Serial.println(server);
        lastReconnectAttempt = 0;
      }
      else
      {
        Serial.print(".");
        delay(500);
      }
    }
  }
  else
  {
    // Client connected

    client.loop();
  }

}

#endif
