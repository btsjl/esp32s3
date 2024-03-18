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
AsyncWebServer server(80);

const char *ssid = "PC-LHQ";
const char *password = "123456789liu";
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

String Merge_Data(void)
{
  int Humidity = 50;    // 此处应为获取温度代码
  int Temperature = 26; // 此处应为获取湿度代码
  // 将温湿度打包为一个HTML显示代码
  String dataBuffer = "<p>";
  dataBuffer += "<h1>传感器数据</h1>";
  dataBuffer += "<b>温度: </b>";
  dataBuffer += String(Temperature);
  dataBuffer += "<br/>";
  dataBuffer += "<b>湿度: </b>";
  dataBuffer += String(Humidity);
  dataBuffer += "<br /></p>";
  // 最后要将数组返回出去
  return dataBuffer;
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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS,"/hello.html","text/html"); });
  // 设置反馈的信息，在HTML请求这个Ip/dht这个链接时，返回打包好的传感器数据
  server.on("/dht", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", Merge_Data().c_str()); });
  server.begin();
  server.onNotFound(notFound);
  server.begin();
}

void loop()
{
}
