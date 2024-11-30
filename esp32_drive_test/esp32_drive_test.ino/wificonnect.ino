#include <WiFi.h>
#include <WebServer.h>

void func(){
  front(1000);
  stop();
  right();
  stop();
  left();
  stop();
  back(1000);
  stop();
  delay(5000);
}

void handlePostRequest() {
  if (server.hasArg("plain")) {  // Check if the body has plain text
    String body = server.arg("plain");
    Serial.println("Received POST data: " + body);

    server.send(200, "text/plain", "Data received: " + body);
    if (body == "back") {
      back(1000);
    }
    if (body == "right") {
      right();
    }
    if (body == "left") {
      left();
    }
    if (body == "straight") {
      front(1000);
    }
    stop();
  } else {
    server.send(400, "text/plain", "Bad Request: No body found");
  }
}

void setupwifi() {
  // Start the serial communication
  Serial.begin(115200);
  Serial.println("\nConnecting to Wi-Fi...");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  // Wait for the connection to establish
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print connection details
  Serial.println("\nWi-Fi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", webpage);
  });

  server.on("/path", HTTP_POST, handlePostRequest);

  server.begin();
}
