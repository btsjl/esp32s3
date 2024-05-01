#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H
#include<Arduino.h>
#include "ArduinoJson.h"
#include "LittleFS.h"
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
String processor(const String &var);
void notFound(AsyncWebServerRequest *request);
void upload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
void callback(AsyncWebServerRequest *request);
void body(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void HttpServerStart(const char *ssid, const char *password);
#endif 