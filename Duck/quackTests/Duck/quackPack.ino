// If Quack is Defined in SetUp QuackPack will be Compiled with rest of MamaQuack
#ifdef QUACKPACK2

// // Simple Boilerplate for 3rd Party Devs (QuackHackers)
// #define QUACKPACK

#ifdef PAYLOADSIZE //if the macro MEDIAN_MAX_SIZE is defined
#undef PAYLOADSIZE
#define PAYLOADSIZE 1//redefine it with the new value
#endif


int     dustPin     = 0;
float   dustVal     = 0;
int     ledPower    = 2;
int     delayTime   = 280;
int     delayTime2  = 40;
float   offTime     = 9680;

void setupQuack()
{
  QuackPack = true;

  // studentID = nickFeuer;
  // token: 1234;
  //
  // project-owl.com/universityname
  //
  // login: nickFeuertest
  // pw: 1234
  //
  // duckID

  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  pinMode(dustPin, INPUT);

  // numPayloads = 1;
}

void loopQuack()
{
  // ledPower is any digital pin on the arduino connected to Pin 2 on the sensor
  digitalWrite(ledPower,LOW);
  delayMicroseconds(delayTime);
  dustVal = analogRead(dustPin);
  delayMicroseconds(delayTime2);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(offTime);

  delay(1000);
  if (dustVal>36.455)
  {
    Serial.println((float(dustVal/1024)-0.0356)*120000*0.035);
  }
}

void QuackPayload()
{
  couple(quack_B, String(dustVal));
}

#endif
