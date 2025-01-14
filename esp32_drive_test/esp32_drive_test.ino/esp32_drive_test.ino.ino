#include <WebServer.h>
#include <WiFi.h>
#include <ESP32Servo.h>

int pin1 = 27;
int pin2 = 26;
int pin3 = 25;
int pin4 = 33;

#define TRIG_PIN 23 // ESP32 pin GPIO23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 22 // ESP32 pin GPIO22 connected to Ultrasonic Sensor's ECHO pin
#define SERVO_PIN 18

// Replace with your network credentials
const char* ssid = "Davies Dev";
const char* password = "xandoozy";
float duration_us, distance_cm;
float large_distance_threshold = 50.0;
float distance_threshold = 20.0;

Servo servoMotor;
WebServer server(80);

void setup() {
  setupwifi();
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);

  servoMotor.attach(SERVO_PIN);  // attaches the servo on ESP32 pin
  set_straight();
}

void loop() {
  server.handleClient();
}