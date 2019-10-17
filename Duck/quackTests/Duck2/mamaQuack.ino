// If Quack is Defined in SetUp QuackPack will be Compiled with rest of MamaQuack
#ifdef MAMAQUACK
#include <Adafruit_BMP085_U.h>
//#include <Adafruit_BMP280.h>
#include "timer.h"

// Mama Sensor code

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
//Adafruit_BMP280 bmp;
auto timer2 = timer_create_default(); // create a timer with default settings

typedef struct
{
  String     deviceID; //Should send as a char
  String    sensorVal;
} Quack;
Quack payload;

void setupQuack()
{
  payload.deviceID  = empty.duckID; //Should send as a char
  payload.sensorVal = "";

  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    QuackPack = false;
    sendQuacks(payload.deviceID, uuidCreator(), "Sensor failed to start"); //Send data
    delay(1000);
    sendQuacks(payload.deviceID, uuidCreator(), "Sensor failed to start"); //Send data
  } else {
    QuackPack = true;
    Serial.println("BMP on");
  }

  if(QuackPack == true) {
    Serial.println("Start timer");
    timer2.every(300000, getSensorData);
  }
  
  Serial.begin(115200);
  Serial.print("setupMamaQuack()");

  Serial.println("\n==============\n");
}

void loopQuack()
{
  timer2.tick();
}

bool getSensorData(void *){
  float T,P;
  
  bmp.getTemperature(&T);
  Serial.println(T);
  bmp.getPressure(&P);
  Serial.println(P);

  payload.sensorVal = "Temp: " + String(T) + " Pres: " + String(P); //Store Data

  offline.path = "";
  sendQuacks(payload.deviceID, uuidCreator(), payload.sensorVal); //Send data
  offline.path = empty.path;
  
  return true;
}

#endif
