// #ifdef cd
// // Copyright 2018 Bryan Knouse, Magus Pereira, Charlie Evans, Taraqur Rahman, Nick Feuer
// //
// // Licensed under the Apache License, Version 2.0 (the "License");
// // you may not use this file except in compliance with the License.
// // You may obtain a copy of the License at
// //
// //     http://www.apache.org/licenses/LICENSE-2.0
// //
// // Unless required by applicable law or agreed to in writing, software
// // distributed under the License is distributed on an "AS IS" BASIS,
// // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// // See the License for the specific language governing permissions and
// // limitations under the License.
//
// #include <Arduino.h>
// #include <SPI.h>
// #include <LoRa.h>
// #include <WiFi.h>
// #include <U8x8lib.h>
//
// /***************************************************
//   un/comment lines to compile PAPA/MAMA/MAMAQUACK
// ***************************************************/
//
// // Recommendation First compile Mama board, then reverse and compile Papa board
// //#define MAMA
// //#define MAMAT
// // #define PAPA
// // #define PAPAT
//
// //To test the quackpack features comment MAMA/PAPA Definitions  and uncomment BOTH the MAMAQUACK and QUACK Definitions
// //#define MAMAQUACK
// //#define QUACK
//
// #define SS      18
// #define RST     14
// #define DI0     26
// #define BAND    915E6
//
// //Test QuackPack
// #ifdef QUACKPACK
// void setupSerial()
// {
//   Serial.begin(115200);
// }
// #define Quack
//
// #endif
//
// // Structure with message data
// typedef struct
// {
//   String fname;
//   String street;
//   String phone;
//   String occupants;
//   String danger;
//   String vacant;
//   String firstaid;
//   String water;
//   String food;
//   String msg;
// } Data;
//
// byte fname_B      = 0xB1;
// byte street_B     = 0xB2;
// byte phone_B      = 0xB3;
// byte occupants_B  = 0xB4;
// byte danger_B     = 0xC1;
// byte vacant_B     = 0xC2;
// byte firstaid_B   = 0xD1;
// byte water_B      = 0xD2;
// byte food_B       = 0xD3;
// byte msg_B        = 0xE4;
//
// // the OLED used
// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
//
// void setupDisplay()
// {
//   u8x8.begin();
//   u8x8.setFont(u8x8_font_chroma48medium8_r);
// }
//
// // Initial LoRa settings
// void setupLoRa()
// {
//   SPI.begin(5, 19, 27, 18);
//   LoRa.setPins(SS, RST, DI0);
//
//   //Initialize LoRa
//   if (!LoRa.begin(BAND))
//   {
//     u8x8.clear();
//     u8x8.drawString(0, 0, "Starting LoRa failed!");
//     Serial.println("Starting LoRa failed!");
//     while (1);
//   }
//   else
//   {
//     Serial.println("LoRa On");
//   }
//
//   //  LoRa.setSyncWord(0xF3);         // ranges from 0-0xFF, default 0x34
//   LoRa.enableCrc();             // Activate crc
// }
//
// #endif
