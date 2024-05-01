#include <Http_Server.h>
AsyncWebServer server(80);
int temp = 0;
JsonDocument doc;

String processor(const String &var)
{
    if (var == "temp")
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
    temp = doc["data"].as<int>();
    Serial.println(doc["data"].as<const char *>());
}

void HttpServerStart(const char *ssid, const char *password)
{
    Serial.begin(115200);
    IPAddress staticIP(192, 168, 137, 10);
    IPAddress gateway(192, 168, 137, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.config(staticIP, gateway, subnet) == false)
    {
        Serial.println("STA Failed!");
        return;
    }

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
    Serial.print("IP Address: http://");
    Serial.println(WiFi.localIP());
    server.on("/json", HTTP_POST, callback, upload, body);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/new.html", "text/html", false, processor); });
    server.on("/dht", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", String(temp).c_str()); });
    server.onNotFound(notFound);
    server.begin();
}