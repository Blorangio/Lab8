#include "Wire.h"
#include "env.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
#include "basicLibrary.ino"
#include "cmath"
#include "Blynk.h"

SYSTEM_THREAD(ENABLED);

#define veryClosePin D5
#define closePin D6
#define farPin D7

#define buttonAPin D4
#define buttonBPin D3
#define buttonCPin D2

#define virtualDistancePin V0
#define virtualLightLevelPin V1
#define virtualIsInformationPin V2

VCNL4040 distanceSensor;

OledWingAdafruit display;

pin_t pinsForLEDGroup[3] = {farPin, closePin, veryClosePin};
LED lightsInLEDGroup[3];
LEDGroup closeVeryCloseFar(lightsInLEDGroup);

Button buttonA;
Button buttonB;
Button buttonC;

int distanceCheck;

int distance;
int lightLevel;

void setup()
{
  closeVeryCloseFar.initialize(pinsForLEDGroup);

  buttonA.initialize(buttonAPin);
  buttonB.initialize(buttonBPin);
  buttonC.initialize(buttonCPin);

  display.setup();

  Blynk.begin(BLYNK_AUTH_TOKEN);

  display.clearDisplay();
  display.display();

  Wire.begin();
  Serial.begin(9600);
  while (!Serial.isConnected())
  {
  }
  if (!distanceSensor.begin())
  {
    Serial.println("Not Connected");
    while (1)
      ;
  }
  distanceSensor.powerOnAmbient();
  distanceSensor.powerOnProximity();
}

void loop()
{
  display.setCursor(0, 0);
  update();
  display.loop();
  display.clearDisplay();

  lightLevel = distanceSensor.getAmbient();
  distance = distanceSensor.getProximity();

  if (!buttonC.val)
  {
    Blynk.virtualWrite(virtualDistancePin, distance);
    Blynk.virtualWrite(virtualLightLevelPin, lightLevel);
    Blynk.virtualWrite(virtualIsInformationPin, true);
    buttonB.val = true;
    buttonA.val = true;
    printMsgToOled("Information on Blynk app");
  }
  else
  {
    Blynk.virtualWrite(virtualIsInformationPin, false);
  }

  if (!buttonB.val)
  {
    printMsgToOled("Light Level:");
    printMsgToOled((String)lightLevel);
  }

  if (!buttonA.val)
  {
    printMsgToOled("Distance:");
    printMsgToOled((String)distance);
  }

  closeVeryCloseFar.allOff();

  distanceCheck = int(nthRoot(distance - 20, 5) / 1.3);
  distanceCheck = map(distanceCheck, 0, 8, 0, 4);

  closeVeryCloseFar.oneOn(distanceCheck);

  display.display();
}

void printMsgToOled(String msg)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(msg);
}

double nthRoot(int val, int root)
{
  return std::pow(val, 1.0 / root);
}