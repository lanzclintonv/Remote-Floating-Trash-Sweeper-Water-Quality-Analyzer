#include <SoftwareSerial.h>

// Pins
const int pH_Sensor_Pin = A0;
const int turbiditySensorPin = A1;
const int trigPin = 2;
const int echoPin = 3;
const int motor1_Pin1 = 4;
const int motor1_Pin2 = 5;
const int motor2_Pin1 = 6;
const int motor2_Pin2 = 7;

// Variables
float pH_Value;
int turbidityValue;
int distance;
char command;

// ESP8266 Serial
SoftwareSerial esp8266(10, 11); // RX, TX

void setup() {
  Serial.begin(115200);
  esp8266.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(motor1_Pin1, OUTPUT);
  pinMode(motor1_Pin2, OUTPUT);
  pinMode(motor2_Pin1, OUTPUT);
  pinMode(motor2_Pin2, OUTPUT);
}

void loop() {
  distance = measureDistance();
  turbidityValue = analogRead(turbiditySensorPin);
  pH_Value = readPHValue();

  // Send data to ESP8266
  esp8266.print("pH:");
  esp8266.print(pH_Value);
  esp8266.print(",Turbidity:");
  esp8266.print(turbidityValue);
  esp8266.print(",Distance:");
  esp8266.println(distance);

  // Check for commands from ESP8266
  if (esp8266.available()) {
    command = esp8266.read();
    controlMotors(command);
  }

  delay(2000); // Adjust the delay as needed
}

float readPHValue() {
  int rawValue = analogRead(pH_Sensor_Pin);
  float voltage = rawValue * (5.0 / 1023.0);
  float pH = 3.5 * voltage; // Calculate pH value, adjust the formula based on your sensor
  return pH;
}

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.0344 / 2;

  return distance;
}

void controlMotors(char command) {
  switch (command) {
    case '1': // Forward
      digitalWrite(motor1_Pin1, HIGH);
      digitalWrite(motor1_Pin2, LOW);
      digitalWrite(motor2_Pin1, HIGH);
      digitalWrite(motor2_Pin2, LOW);
      break;
    case '2': // Left
      digitalWrite(motor1_Pin1, HIGH);
      digitalWrite(motor1_Pin2, LOW);
      digitalWrite(motor2_Pin1, LOW);
      digitalWrite(motor2_Pin2, LOW);
      break;
    case '3': // Right
      digitalWrite(motor1_Pin1, LOW);
      digitalWrite(motor1_Pin2, LOW);
      digitalWrite(motor2_Pin1, HIGH);
      digitalWrite(motor2_Pin2, LOW);
      break;
    case '4': // Backward
      digitalWrite(motor1_Pin1, LOW);
      digitalWrite(motor1_Pin2, HIGH);
      digitalWrite(motor2_Pin1, LOW);
      digitalWrite(motor2_Pin2, HIGH);
      break;
    case '5': // Stop
      digitalWrite(motor1_Pin1, LOW);
      digitalWrite(motor1_Pin2, LOW);  
  }