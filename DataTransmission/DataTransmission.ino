/* Code edited by Husain Sakarwala and Shreesh Kolhatkar */
#include <WiFi.h>

const int trigPin = 12; // Trig pin of HC-SR04 connected to GPIO 12
const int echoPin = 34; // Echo pin of HC-SR04 connected to GPIO 14
const int pinIRd = 27;
int IRvalueD = 0;

const char* ssid = ""; //Hidden due to privacy reasons    
const char* password = ""; //Hidden due to privacy reasons
const char* serverAddress = ""; //Hidden due to privacy reasons
const int serverPort = ; //Hidden due to privacy reasons

WiFiClient TCPclient;

// Variable to track the last sent command
char lastSentCommand = '2'; // Initialize with a value that's different from '0' and '1'

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pinIRd, INPUT);

  Serial.println("ESP32: TCP CLIENT + SENSOR DATA");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // connect to TCP server (Arduino #2)
  if (TCPclient.connect(serverAddress, serverPort)) {
    Serial.println("Connected to TCP server");
  } else {
    Serial.println("Failed to connect to TCP server");
  }
}

void loop() {
  if (!TCPclient.connected()) {
    Serial.println("Connection is disconnected");
    TCPclient.stop();

    // reconnect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort)) {
      Serial.println("Reconnected to TCP server");
    } else {
      Serial.println("Failed to reconnect to TCP server");
    }
  }

  digitalRead(echoPin);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(0);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(0);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  // Calculate distance in centimeters
  float distance = duration * 0.034 / 2;

  IRvalueD = digitalRead(pinIRd);
  Serial.print("Digital Reading is: ");
  Serial.println(IRvalueD);

  // Check for changes in conditions
  if (distance <= 30 && IRvalueD == LOW && lastSentCommand != '1') {
    TCPclient.write('11');
    lastSentCommand = '1';
    TCPclient.flush();
    Serial.println("sent command: 1");
  } else if (!(distance <= 30 && IRvalueD == LOW) && lastSentCommand != '0') {
    TCPclient.write('00');
    lastSentCommand = '0';
    TCPclient.flush();
    Serial.println("sent command: 0");
  }
