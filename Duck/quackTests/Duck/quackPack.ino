// If Quack is Defined in SetUp QuackPack will be Compiled with rest of MamaQuack
#ifdef QUACKPACK
#include <SFE_BMP180.h>
#include "timer.h"

// // Simple Boilerplate for 3rd Party Devs (QuackHackers)

SFE_BMP180 pressure;
auto timer = timer_create_default(); // create a timer with default settings

typedef struct
{
  String    deviceID;
  String    sensorVal;
} Quack;
Quack payload;

void setupQuack()
{
  QuackPack = true;

  payload.deviceID  = "20seven";
  payload.sensorVal = "";

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
      // Oops, something went wrong, this is usually a connection problem,
      // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }


  timer.every(5000, getSensorData);

  Serial.begin(115200);
  Serial.print("setupQuack()");
  Serial.println(" - Sensor Val: " + payload.deviceID);

  Serial.println("\n==============\n");
}

void loopQuack()
{
  timer.tick();
}

bool getSensorData(void *){
  char status;
  double T,P;

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("Temp: ");
      Serial.print(T);
      Serial.print("C");

    status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  payload.sensorVal = "Temp: " + String(T) + " Pres: " + String(P); //Store Data

  sendQuacks(payload.deviceID, "message id here", payload.sensorVal); //Send data

  return true;
}

#endif
