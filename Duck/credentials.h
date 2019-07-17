// Please do not commit credentials to public GH

#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__

#define SSID        "nick_owl" // Type your SSID
#define PASSWORD    "helloDuck11" // Type your Password

//#define MQTT_MAX_PACKET_SIZE 1000;

#define ORG         "9c6nfo"                  // "quickstart" or use your organisation
#define DEVICE_ID   "ltt"
#define DEVICE_TYPE "PAPA"                // your device type or not used for "quickstart"
#define TOKEN       "DI2pFVAkmo&?D36h9z"      // your device token or not used for "quickstart"#define SSID        "nick_owl" // Type your SSID

char server[]           = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[]            = "iot-2/evt/sensorup/fmt/json";
char authMethod[]       = "use-token-auth";
char token[]            = TOKEN;
char clientId[]         = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

#endif
