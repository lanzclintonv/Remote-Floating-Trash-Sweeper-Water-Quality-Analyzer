#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Arduino Serial
SoftwareSerial arduinoSerial(4, 5); // RX, TX

// Variables
String dataFromArduino;

// Web server
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  arduinoSerial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Web server
  server.on("/", handleRoot);
  server.on("/Forward", handleForward);
  server.on("/Left", handleLeft);
  server.on("/Right", handleRight);
  server.on("/Backward", handleBackward);
  server.on("/Stop", handleStop);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  if (arduinoSerial.available()) {
    dataFromArduino = arduinoSerial.readString();
  }
}

void handleRoot() {
  server.send(200, "text/plain", dataFromArduino);
}

void handleForward() {
  arduinoSerial.print('1');
  server.send(200, "text/plain", "Forward");
}

void handleLeft() {
  arduinoSerial.print('2');
  server.send(200, "text/plain", "Left");
}

void handleRight() {
  arduinoSerial.print('3');
  server.send(200, "text/plain", "Right");
}

void handleBackward() {
  arduinoSerial.print('4');
  server.send(200, "text/plain", "Backward");
}

void handleStop() {
  arduinoSerial.print('5');
  server.send(200, "text/plain", "Stop");
}
