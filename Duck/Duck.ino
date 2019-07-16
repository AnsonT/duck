#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

// Ducklink
#include <DNSServer.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "indexBeta.h"
#include "credentials.h"

/***************************************************
  un/comment lines to compile Ducklink/Mama/Papa
***************************************************/

// Recommendation First compile Mama board, then reverse and compile Papa board
//#define DL
//const char *AP = " 🆘 DUCK EMERGENCY PORTAL";

#define MD
const char *AP = " 🆘 MAMA EMERGENCY PORTAL";

//#define PD
//const char *AP = " 🆘 PAPA EMERGENCY PORTAL";

#define THIRTYMIN (1000UL * 60 * 30);
unsigned long rolltime = millis() + THIRTYMIN;

#define SS      18
#define RST     14
#define DI0     26
#define BAND    915E6

IPAddress apIP(192, 168, 1, 1);
WebServer webServer(80);

DNSServer dnsServer;
const byte DNS_PORT = 53;

/**
   Hotspot/Access Point (🐥 DuckLink 🆘 )
   Local DNS (duck.local)
*/
//const char *AP   = " 🆘 EMERGENCY PORTAL";


const char *DNS  = "duck";

String portal = MAIN_page;
String id = "";
String iAm = "Civ";
String runTime;

// Structure with message data
typedef struct
{
  // Duck
  String whoAmI;   // offline Classifier (Default is Civ for Civilian)
  String duckID;   // Duck (ESP32) Mac Address
  String whereAmI; // Geo-coordinates (Default in 0,0 - Null Island) - Under construction - Omer's App
  String runTime;  // How long have I been on?

  // Check to see if message is from Civilian or Duck
  int fromCiv = 0;
  String messageId;
  // Civilian
  String fname;
  String street;
  String phone;
  String occupants;
  String danger;
  String vacant;
  String firstaid;
  String water;
  String food;
  String msg;
  String path;
} Data;

Data offline;

byte whoAmI_B     = 0xA1;
byte duckID_B     = 0xA2;
byte whereAmI_B   = 0xA3;
byte runTime_B    = 0xA4;

byte fromCiv_B    = 0xA5;

byte fname_B      = 0xB1;
byte street_B     = 0xB2;
byte phone_B      = 0xB3;
byte occupants_B  = 0xB4;
byte danger_B     = 0xC1;
byte vacant_B     = 0xC2;
byte firstaid_B   = 0xD1;
byte water_B      = 0xD2;
byte food_B       = 0xD3;
byte msg_B        = 0xE4;

byte msgId_B      = 0xF4;
byte path_B       = 0xF3;

// the OLED used
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

/*
 * Variables for BLE N5 transmission 
 */

#include "BLEDevice.h"
//#include "BLEScan.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("00000000-0000-1000-8000-00805f9b34fb");
static BLEUUID serviceUUID2("8c860000-b9f4-4a5e-a3d2-76034a04ea5d");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("8c860002-b9f4-4a5e-a3d2-76034a04ea5d");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;

/*
 * End BLE variables
 * Start BLE functions
 */

 /**
 * A BLE client example that is rich in capabilities.
 */


static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
//    uint8_t senNumber = pData[0];
//    uint8_t sensorCode[2]={ pData[1], pData[2]};
//    float value, rawValue,maxLimit,minLimit;
//
//    memcpy(&value,pData+3,4);
//    memcpy(&rawValue,pData+7,4);
//    memcpy(&maxLimit,pData+11,4);
//    memcpy(&minLimit,pData+15,4);
//
//    Serial.print("Sensor Num = ");
//    Serial.print(senNumber);
//
//    Serial.print(" Sensor code = ");
//    Serial.print(sensorCode[0],HEX);
//    Serial.print(sensorCode[1],HEX);
//
//    Serial.print(" Sensor Value = ");
//    Serial.print(value);
//
//    Serial.print(" Sensor rawValue = ");
//    Serial.print(rawValue);
//
//    Serial.print(" Sensor maxLimit = ");
//    Serial.print(maxLimit);
//
//    Serial.print(" Sensor minLimit = ");
//    Serial.println(minLimit);
}

bool connectToServer(BLEAddress pAddress) {
    Serial.print("Forming a connection to ");
    Serial.println(pAddress.toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    // Connect to the remove BLE Server.
    pClient->connect(pAddress);
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID2);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      return false;
    }
    Serial.println(" - Found our characteristic");

    //Read the value of the characteristic.
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());

    const uint8_t v[]={0x1,0x0};
    pRemoteCharacteristic->registerForNotify(notifyCallback);
    pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)v,2,true);
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {

      // 
      Serial.print("Find other service");
      if (advertisedDevice.isAdvertisingService(serviceUUID2)){
        Serial.println("Service UUID 2 found");
      } else {
        Serial.println("Not found");
      }
      Serial.print("Found our device!  address: "); 
      advertisedDevice.getScan()->stop();

      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

/*
 * End BLE functions
 */

void setupDisplay()
{
  //u8x8.begin();
  //u8x8.setFont(u8x8_font_chroma48medium8_r);
}

// Initial LoRa settings
void setupLoRa()
{
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);
  LoRa.setTxPower(20);
  //LoRa.setSignalBandwidth(62.5E3);

  //Initialize LoRa
  if (!LoRa.begin(BAND))
  {
    //u8x8.clear();
    //u8x8.drawString(0, 0, "Starting LoRa failed!");
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else
  {
    Serial.println("LoRa On");
  }

  //  LoRa.setSyncWord(0xF3);         // ranges from 0-0xFF, default 0x34
  LoRa.enableCrc();             // Activate crc
}

/**
   showReceivedstat
   Displays Received stat on OLED and Serial Monitor
*/
void showReceivedData()
{

  Serial.println("Class: "        +  offline.whoAmI     );
  Serial.println("ID : "          +  offline.duckID    );
  Serial.println("Location: "     +  offline.whereAmI     );
  Serial.println("On for: "       +  offline.runTime + " milliseconds\n" );
  Serial.println("Message ID: "   +  offline.messageId );

  Serial.println("Name: "         +  offline.fname     );
  Serial.println("Street: "       +  offline.street    );
  Serial.println("Phone: "        +  offline.phone     );
  Serial.println("Occupants: "    +  offline.occupants );
  Serial.println("Dangers: "      +  offline.danger    );
  Serial.println("Vacant: "       +  offline.vacant    );
  Serial.println("First Aid: "    +  offline.firstaid  );
  Serial.println("Water: "        +  offline.water     );
  Serial.println("Food: "         +  offline.food      );
  Serial.println("Mess: "         +  offline.msg       );
  Serial.println("Time: milliseconds\n");

  Serial.println("Path: "         +  offline.path      );

  Serial.print("FromCiv: ");
  Serial.println(offline.fromCiv    );
}

void setupPortal()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP);
  delay(200); // wait for 200ms for the access point to start before configuring

  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  Serial.println("Created Hotspot");

  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.onNotFound([]()
  {
    webServer.send(200, "text/html", portal);
  });

  webServer.on("/id", []() {
    webServer.send(200, "text/html", offline.duckID + "," + offline.whoAmI);
  });

  webServer.on("/restart",[]()
  {
    webServer.send(200,"text/plain", "Restarting...");
    delay(1000);
    ESP.restart();
  });

  webServer.on("/mac", []() {
    String    page = "<h1>Duck Mac Address</h1><h3>Data:</h3> <h4>" + offline.duckID + "</h4>";
    webServer.send(200, "text/html", page);
  });

  // Test 👍👌😅

  webServer.begin();

  if (!MDNS.begin(DNS))
  {
    Serial.println("Error setting up MDNS responder!");
  }
  else
  {
    Serial.println("Created local DNS");
    MDNS.addService("http", "tcp", 80);
  }
}

/**
   readyData
   Reads WebServer Parameters and couples into Data Struct
   @return coupled Data Struct
*/
void readData()
{

  dnsServer.processNextRequest();
  webServer.handleClient();

  String webId = webServer.arg(0);

  if (id != webId)
  {
    //u8x8.clear();
    //u8x8.drawString(0, 4, "New Response");
    offline.fromCiv    = 1;
    offline.messageId  = webServer.arg(0);
    offline.fname      = webServer.arg(1);
    offline.street     = webServer.arg(2);
    offline.phone      = webServer.arg(3);
    offline.occupants  = webServer.arg(4);
    offline.danger     = webServer.arg(5);
    offline.vacant     = webServer.arg(6);
    offline.firstaid   = webServer.arg(7);
    offline.water      = webServer.arg(8);
    offline.food       = webServer.arg(9);
    offline.msg        = "";
    offline.path       = offline.duckID;

    //u8x8.setCursor(0, 16);
    //u8x8.print("Name: " + offline.fname);

    Serial.println("Before____ID: " + id + " Webserver: " + webServer.arg(0));

    id = webId;

    Serial.println("After_____ID: " + id + " Webserver: " + webServer.arg(0));
    showReceivedData();

  }
  //  return offlineA;
}

void couple(byte byteCode, String outgoing)
{
  LoRa.write(byteCode);               // add byteCode
  LoRa.write(outgoing.length());      // add payload length
  LoRa.print(outgoing);               // add payload

  //   Displays Sent Data on OLED and Serial Monitor
  //   Serial.println("Parameter: " + outgoing);
}

/**
   sendPayload
   Sends Payload (offline Data Struct as Bytes)
   Shows Sent Data
*/

void sendPayload(Data offline)
{
  LoRa.beginPacket();
  couple(msgId_B, offline.messageId);
  couple(whoAmI_B, offline.whoAmI);
  couple(duckID_B, offline.duckID);
  couple(whereAmI_B, offline.whereAmI);
  couple(runTime_B, offline.runTime);

  //couple(fromCiv_B, 0);

  couple(fname_B, offline.fname);
  couple(street_B, offline.street);
  couple(phone_B, offline.phone);
  couple(occupants_B, offline.occupants);

  couple(danger_B, offline.danger);
  couple(vacant_B, offline.vacant);

  couple(firstaid_B, offline.firstaid);
  couple(water_B, offline.water);
  couple(food_B, offline.food);

  couple(msg_B, offline.msg);

  couple(path_B, offline.path);
  LoRa.endPacket();

  delay(5000);
}

//Send duckStat every 30 minutes
void sendDuckStat(Data offline)
{
  if ((long)(millis() - rolltime) >= 0)
  {
    LoRa.beginPacket();
    couple(whoAmI_B, offline.whoAmI);
    couple(duckID_B, offline.duckID);
    couple(whereAmI_B, offline.whereAmI);
    couple(runTime_B, offline.runTime);
    LoRa.endPacket();

    rolltime += THIRTYMIN;
  }
}

String duckID()
{
  char id1[15];
  char id2[15];

  uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip = (uint16_t)(chipid >> 32);

  snprintf(id1, 15, "%04X", chip);
  snprintf(id2, 15, "%08X", (uint32_t)chipid);

  String ID1 = id1;
  String ID2 = id2;

  return ID1 + ID2;
}

void setupDuck()
{
  offline.whoAmI   = iAm;
  offline.duckID   = duckID().substring(3, 7);
  offline.whereAmI = "0,0"; // Until further dev, default is null island
  offline.runTime  = millis();

  // Test - Print to serial
  Serial.println("\nClass: "        +  offline.whoAmI     );
  Serial.println("ID : "            +  offline.duckID    );
  Serial.println("Location: "       +  offline.whereAmI     );
  Serial.println("On for: "         +  offline.runTime + " milliseconds\n\n" );
}

String readMessages(byte mLength)
{
  String incoming = "";

  for (int i = 0; i < mLength; i++)
  {
    incoming += (char)LoRa.read();
  }
  //Serial.println(incoming);

  return incoming;
}
// Mama and Papa

/**
   receive
   Reads and Parses Received Packets
   @param packetSize
*/
void receive(int packetSize)
{
  if (packetSize != 0)
  {
    byte byteCode, mLength;
    Serial.print("Packet Received");
    // read packet
    int rssi;
    float snr;
    long freqErr;
    int availableBytes;

    rssi = LoRa.packetRssi();
    snr = LoRa.packetSnr();
    freqErr = LoRa.packetFrequencyError();
    availableBytes = LoRa.available();

    while (LoRa.available())
    {
      byteCode = LoRa.read();
      mLength  = LoRa.read();

      if (byteCode == whoAmI_B)
      {
        offline.whoAmI = readMessages(mLength);
      }
      else if (byteCode == duckID_B)
      {
        offline.duckID = readMessages(mLength);
      }
      else if (byteCode == whereAmI_B)
      {
        offline.whereAmI = readMessages(mLength);
      }
      else if (byteCode == runTime_B)
      {
        offline.runTime = readMessages(mLength);
      }
      else if (byteCode == fromCiv_B)
      {
        offline.fromCiv = 0;
      }
      else if (byteCode == fname_B)
      {
        offline.fname = readMessages(mLength);
      }
      else if (byteCode == street_B)
      {
        offline.street = readMessages(mLength);
      }
      else if (byteCode == phone_B)
      {
        offline.phone = readMessages(mLength);
      }
      else if (byteCode == occupants_B)
      {
        offline.occupants = readMessages(mLength);
      }
      else if (byteCode == danger_B)
      {
        offline.danger = readMessages(mLength);
      }
      else if (byteCode == vacant_B)
      {
        offline.vacant = readMessages(mLength);
      }
      else if (byteCode == firstaid_B)
      {
        offline.firstaid = readMessages(mLength);
      }
      else if (byteCode == water_B)
      {
        offline.water = readMessages(mLength);
      }
      else if (byteCode == food_B)
      {
        offline.food = readMessages(mLength);
      }
      else if (byteCode == msg_B)
      {
        offline.msg = readMessages(mLength) + rssi + ",";
        offline.msg = offline.msg + snr + ",";
      }
      else if (byteCode == path_B)
      {
        offline.path = readMessages(mLength);
      }
      else if (byteCode == msgId_B)
      {
        offline.messageId = readMessages(mLength);
      }
    }
    showReceivedData();
    //jsonify(offline);
  }
  else
  {
    return;
  }
}
