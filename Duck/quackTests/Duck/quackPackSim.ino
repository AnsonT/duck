// If Quack is Defined in SetUp QuackPack will be Compiled with rest of MamaQuack
#ifdef QUACKPACKSIM

// // Simple Boilerplate for 3rd Party Devs (QuackHackers)

typedef struct
{
  String deviceID;
  int    sensorVal;
} Quack;
Quack payload;

void setupQuack()
{
  QuackPack = true;

  payload.deviceID  = "20seven";
  payload.sensorVal = 0;

  Serial.print("setupQuack()");
  Serial.println(" - Sensor Val: " + payload.deviceID);

  Serial.println("\n==============\n");
}

void loopQuack()
{
  Serial.println("loopQuack()");

  payload.sensorVal += 5;

  Serial.print(" - Sensor Val: ");
  Serial.println(payload.sensorVal);

  Serial.println("\n==============\n");

  String quacket = payload.sensorVal + ",";

  // Use SendQuacks to send your data through the Duck network
  sendQuacks(payload.deviceID, String(random(999)), quacket); //Send data

  delay(10000);
}

#endif
