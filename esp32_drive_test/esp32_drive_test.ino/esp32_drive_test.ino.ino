#include <WebServer.h>
#include <WiFi.h>

int pin1 = 27;
int pin2 = 26;
int pin3 = 25;
int pin4 = 33;
// Replace with your network credentials
const char* ssid = "Davies Dev";
const char* password = "xandoozy";

WebServer server(80);

void setup() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  setupwifi();
}

void loop() {
  server.handleClient();
}