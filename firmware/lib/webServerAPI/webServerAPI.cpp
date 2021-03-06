#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "sensorUmidadeSolo.h"

int port = 5000;
ESP8266WebServer server(port);

void setHeaders () {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Headers", "*");
    server.sendHeader("Access-Control-Max-Age", "600");
    server.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
}

void handleNotFounded() {
    server.send(404, "application/json", "Not found");
};

void blinkLed() {
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH); 
}

void sensors() {
    float humidity = getPercentHumidity();
    setHeaders();
    String data = "{\"humidity\":" + String(humidity) + "}";
    server.send(200, "application/json", data);
    blinkLed();
}



void setRoutes () {
    server.on("/sensors", HTTP_GET, sensors);
    server.onNotFound(handleNotFounded);
}

void webServerInit() {
    setRoutes();
    setHeaders();
    server.begin();
    String ipLocal = WiFi.localIP().toString();
    String ipMac = WiFi.macAddress();
    Serial.println("HTTP server started at " + ipLocal + ":" + String(port) + "\n\nMAC: " + ipMac );
}

void handleClient () {
    server.handleClient();
}
