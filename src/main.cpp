#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
#include "car.h"
#include "driver.h"

char auth[] = "7qEJyqyofHbv_awb61PIGOA-4jn6ZNbv";
char ssid[] = "VIP COFFEE 2";
char psk[] = "visuckhoeban";
Car car(D1, D2, D3, D4);
int irSensorPins[] = {D5, D6, D7};
Driver driver(irSensorPins);
bool isAutopilot = true;
int command;
int lastCommand = -1;

void setup()
{
  Blynk.begin(auth, ssid, psk);
  car.setup();
  driver.setup();
}

void autoPilotLoop()
{
  command = driver.drive();
  switch (command)
  {
  case COMMAND_MOVE_FORWARD:
    car.setState(STATE_MOVING_FORWARD);
    break;
  case COMMAND_TURN_LEFT:
    car.setState(STATE_TURNING_LEFT);
    break;
  case COMMAND_TURN_RIGHT:
    car.setState(STATE_TURNING_RIGHT);
    break;
  }
}

void loop()
{
  Blynk.run();
  if (isAutopilot)
  {
    autoPilotLoop();
  }
  car.loop();
  if (command != lastCommand)
  {
    lastCommand = command;
    // delay(200);
  }
}

BLYNK_WRITE(V0)
{
  const int value = param.asInt();
  isAutopilot = (value != 0);
  if (isAutopilot)
  {
    car.setState(STATE_STOP);
  }
  else
  {
    car.setState(STATE_MANUAL);
    car.stopLeftWheel();
    car.stopRightWheel();
  }
}

BLYNK_WRITE(V1)
{
  const int pwmValue = param.asInt();
  car.setPwmValue(pwmValue);
}

BLYNK_WRITE(V2)
{
  if (isAutopilot)
  {
    return;
  }
  const int y = param[1].asInt();
  if (y == 0)
  {
    car.stopLeftWheel();
  }
  else
  {
    car.moveLeftWheel(y > 0);
  }
}

BLYNK_WRITE(V3)
{
  if (isAutopilot)
  {
    return;
  }
  const int y = param[1].asInt();
  if (y == 0)
  {
    car.stopRightWheel();
  }
  else
  {
    car.moveRightWheel(y > 0);
  }
}