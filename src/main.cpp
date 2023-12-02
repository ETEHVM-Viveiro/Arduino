// C Libs
#include <string>
#include <any>

// Arduino Libs
#include <Arduino.h>
#include "WiFiS3.h"
#include <HttpClient.h>

// Project Functions
#include <functions/wifiStatus.h>
#include <functions/request.h>

// Arduino Secrets
#include "arduino_secrets.h" 

// Sensores & Pinos
int speaker_port = 2;
int rele_port = 3;
int capacitive_sensor_port = A0;
int capacitive_sensor_value = 0;


// Wifi
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);

  pinMode(speaker_port, OUTPUT);
  pinMode(rele_port, OUTPUT);
  pinMode(capacitive_sensor_port, INPUT);


  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  
  printWifiStatus();
 }

void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Verifique a conexão Wi-Fi

    // Declare the variable before using it
    String verifyIrrigationRequests = makeRequest("192.168.15.59", "/api/arduino/get-irrigation-request", "G", "application/json", "{\"value\": 1}", 3333);
    Serial.println(verifyIrrigationRequests);

    

    // makeRequest("192.168.15.59", "/api/arduino/register-data", "P", "application/json", "{\"value\": 1}", 3333);
  }


  // digitalWrite(rele_port, HIGH);
  // capacitive_sensor_value = analogRead(capacitive_sensor_port);
  // if (capacitive_sensor_value > 300) {
  //   digitalWrite(speaker_port, HIGH);
  //   delay(1000);
  //   digitalWrite(speaker_port, LOW);
  //   delay(1000);
  // }
  // else {
  //   digitalWrite(speaker_port, LOW);
  // }

  // if(WiFi.status()== WL_CONNECTED){

  //   Serial.println("Connected to the WiFi network");
  // }


  delay(10000);
}