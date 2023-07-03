/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/admin/Documents/CTD_2023/Labs/Lab8/src/Lab8.ino"
#include "Wire.h"
#include "env.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
#include "basicLibrary.ino"
#include "cmath"
#include "Blynk.h"

void setup();
void loop();
void printMsgToOled(String msg);
double nthRoot(int val, int root);
#line 9 "/Users/admin/Documents/CTD_2023/Labs/Lab8/src/Lab8.ino"
SYSTEM_THREAD(ENABLED);

#define veryClosePin D5
#define closePin D6
#define farPin D7

#define virtualDistancePin V0
#define virtualLightLevelPin V1
#define virtualIsInformationPin V2

VCNL4040 distanceSensor;

OledWingAdafruit display;

pin_t pinsForLEDGroup[3] = {farPin, closePin, veryClosePin};
LED lightsInLEDGroup[3];
LEDGroup closeVeryCloseFar(lightsInLEDGroup);

int distanceCheck;

int distance;
int lightLevel;

bool buttonCToggle;
bool buttonBToggle;
bool buttonAToggle;

void setup()
{
  closeVeryCloseFar.initialize(pinsForLEDGroup);

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
  Blynk.run();
  display.setCursor(0, 0);
  update();
  display.loop();
  display.clearDisplay();

  lightLevel = distanceSensor.getAmbient();
  distance = distanceSensor.getProximity();

  if (display.pressedA())
  {
    buttonAToggle = !buttonAToggle;
  }
  if (display.pressedB())
  {
    buttonBToggle = !buttonBToggle;
  }
  if (display.pressedC())
  {
    buttonCToggle = !buttonCToggle;
  }

  if (buttonCToggle)
  {
    Blynk.virtualWrite(virtualDistancePin, distance);
    Blynk.virtualWrite(virtualLightLevelPin, lightLevel);
    Blynk.virtualWrite(virtualIsInformationPin, true);
    buttonBToggle = false;
    buttonAToggle = false;
    printMsgToOled("Information on Blynk app");
  }
  else
  {
    Blynk.virtualWrite(virtualIsInformationPin, false);
  }

  if (buttonBToggle)
  {
    printMsgToOled("Light Level:");
    printMsgToOled((String)lightLevel);
  }

  if (buttonAToggle)
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