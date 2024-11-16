#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char* ssid = "NCEP_ESP";
const char* password = "12345678";

IPAddress local_IP(192,168,1,22);
IPAddress gateway(192,168,1,5);
IPAddress subnet(255,255,255,0);

String webpage = "<!DOCTYPE html>"
"<html>"
"  <head>"
"    <title>Page Title</title>"
"  </head>"
"  <body style='background-color: #eeeeee'>"
"    <span style='color: #003366'"
"      ><h1>Send a Number to ESP32</h1>"
"      <p>"
"        <input type='number' id='inputNumber' placeholder='Enter a number' />"
"      </p>"
"      <p>"
"        <button type='button' id='BTN_SEND_BACK'>Send Number to ESP32</button>"
"      </p></span"
"    >"
"    <script>"
"      var Socket;"
"      document"
"        .getElementById('BTN_SEND_BACK')"
"        .addEventListener('click', button_send_back);"
"      function init() {"
"        Socket = new WebSocket('ws://' + window.location.hostname + ':81/');"
"      }"
"      function button_send_back() {"
"        var inputNumber = document.getElementById('inputNumber').value;"
"        var msg = { number: parseInt(inputNumber) };"
"        console.log(msg);"
"        Socket.send(JSON.stringify(msg));"
"      }"
"      window.onload = function (event) {"
"        init();"
"        console.log('Pinging');"
"      };"
"    </script>"
"  </body>"
"</html>";

int interval = 1000;
unsigned long previousMillis = 0;

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
  Serial.begin(115200);

  Serial.print("Setting up Access Point ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Starting Access Point ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

  Serial.print("IP address = ");
  Serial.println(WiFi.softAPIP());
  
  server.on("/", []() {
    server.send(200, "text/html", webpage);
  });
  server.begin();
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  server.handleClient();
  webSocket.loop();

  unsigned long now = millis();
  if ((unsigned long)(now - previousMillis) > interval) {
    previousMillis = now;
  }
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {
  Serial.println(String(type));
  Serial.println(String(num));
  Serial.println(String(length));
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Client " + String(num) + " disconnected");
      break;
    case WStype_CONNECTED:  
      Serial.println("Client " + String(num) + " connected");
      break;
    case WStype_TEXT: {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      // Extract the number sent from the client
      int receivedNumber = doc["number"];
      Serial.println("Received number from user " + String(num) + ": " + String(receivedNumber));
      break;
    }
  }
}
