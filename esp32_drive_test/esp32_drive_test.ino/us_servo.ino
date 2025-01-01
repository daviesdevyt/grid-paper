#include <ESP32Servo.h>

#define TRIGGER_PIN 5
#define Echo_pin 18
#define SERVO_PIN 2
#define SERVO_NORMAL 90
#define SERVO_RIGHT_MAX 0
#define SERVO_LEFT_MAX 180
#define MIN_DISTANCE 10

Servo servoMotor;

long duration;
int distance;

int getDistance()
{
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    duration = pulseIn(Echo_pin, HIGH);
    distance = duration * 0.034 / 2;
}

void avoidObstacle()
{
    int rightDistance, leftDistance;

    servoMotor.write(SERVO_RIGHT_MAX);
    delay(500);
    rightDistance = getDistance();
    Serial.print("Right Distance: ");
    Serial.println(rightDistance);

    servoMotor.write(SERVO_LEFT_MAX);
    delay(500);
    leftDistance = getDistance();
    Serial.print("Left Distance: ");
    Serial.println(leftDistance);

    if (rightDistance > leftDistance)
    {
        Serial.println("Moving right");
        right();
    }
    else
    {
        Serial.println("Moving left");
        left();
    }

    servoMotor.write(SERVO_NORMAL);
    delay(500);
}

void setupUs_servo()
{
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(Echo_pin, INPUT);
    servoMotor.attach(SERVO_PIN);
}   

void loopUs_servo()
{
    distance = getDistance();

    if (distance > 0 && distance <= MIN_DISTANCE)
    {
        Serial.println("Obstacle detected! Avoiding... ");
        stop();
        avoidObstacle();
    }

    delay(50);
}