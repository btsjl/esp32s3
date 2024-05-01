#include <Arduino.h>
#include "Http_Server.h"
#include "Max6675.h"
const char *ssid = "PC-LHQ";
const char *password = "123456789liu";
void setup()
{
  Serial.begin(115200);
  HttpServerStart(ssid, password);
  Max6675Init();
}
void loop()
{
  GetTempValue();
}
