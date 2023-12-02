#include "WiFiS3.h"
#include "HttpClient.h"
#include "WiFiClient.h"
#include "arduino_secrets.h"
#include <ArduinoJson.h>

String makeRequest(std::string server_url, std::string path, std::string method, std::string content_type, std::string body, int port) {
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, server_url.c_str(), port ? port : 80);

    client.beginRequest();    

    switch (method.c_str()[0])
    {
    case 'G':
        client.get(path.c_str());
        break;
    case 'P':
        client.post(path.c_str());
        client.sendHeader("authorization", ARDUINO_BACKEND_TOKEN);
        client.sendHeader("content-type", content_type.c_str());
        client.sendHeader("content-length", body.length());
        client.endRequest();
        client.print("{\"value\": 1}");
        break;
    case 'D':
        client.del(path.c_str());
        break;
    default:
        break;
    }

    client.sendHeader("authorization", ARDUINO_BACKEND_TOKEN);
    client.sendHeader("content-type", content_type.c_str());
    client.endRequest();

    int statusCode = client.responseStatusCode();

    Serial.print("Status code: ");
    Serial.println(statusCode);

    if (statusCode < 0) {
        Serial.print("Failed to connect");
        Serial.println(statusCode);
        return "";
    }

    String response = client.responseBody();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        Serial.print("Failed to deserialize JSON: ");
        Serial.println(error.c_str());
        return "";
    }

    String desserialized;
    serializeJson(doc, desserialized);

    const char* nome = doc["message"];

    Serial.print("Response: ");
    Serial.println(nome);

    return response;
}

