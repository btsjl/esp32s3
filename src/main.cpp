#include "LittleFS.h"
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "ArduinoJson.h"
AsyncWebServer server(80);

const char *ssid = "PC-LHQ";
const char *password = "123456789liu";
int temp = 0;
JsonDocument doc;

String processor(const String& var)
{
  if(var=="temp")
  return String(temp);
  else
  return String();
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void upload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    Serial.printf("UploadStart: %s\n", filename.c_str());
  }
  for (size_t i = 0; i < len; i++)
  {
    Serial.write(data[i]);
  }
  if (final)
  {
    Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
  }
}

void callback(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain", "Data received successfully");
}

void body(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  deserializeJson(doc, (const char *)data);
  Serial.println(doc["data"].as<const char *>());
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.on("/json", HTTP_POST, callback, upload, body);
  server.on("/",HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(LittleFS, "/new.html", "text/html",false,processor);
  });
  server.on("/dht", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200,"text/plain",String(temp).c_str());
    temp++;
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop()
{
}
