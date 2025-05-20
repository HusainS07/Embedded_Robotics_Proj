//Edited by Husain, Shreesh, Vedant & Harshit

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>          

#define SERVER_PORT 4080
LiquidCrystal_I2C lcd(0x27, 16, 2);
int ena = 27;
int in1 = 26;
int in2 = 25;
int receivedData;
int parityBit;
const char* ssid = "";     // CHANGE TO YOUR WIFI SSID
const char* password = ""; // CHANGE TO YOUR WIFI PASSWORD

WiFiServer TCPserver(SERVER_PORT);

void setup() {
  Serial.begin(9600);
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.println("ESP32 #2: TCP SERVER + AN LED");
   lcd.init();         // initialize the LCD
  lcd.backlight();    // Turn on the blacklight  // Connect to Wi-Fi

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print your local IP address:
  Serial.print("ESP32 #2: TCP Server IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

  // Start listening for a TCP client (from ESP32 #1)
  TCPserver.begin();
}

void loop() {
  
  // Wait for a TCP client from ESP32 #1:
  WiFiClient client = TCPserver.available();

  if (client) {
    // Read the command from the TCP client:
    char command = client.read();
    Serial.print("ESP32 #2: - Received command: ");
    Serial.println(command);

  int sum = 0;
  int tempData = receivedData;  // Store a copy for later comparison

  while (receivedData > 0) {
        sum = (sum + receivedData % 10) % 2;  // Calculate the sum modulo 2
        receivedData /= 10;
    }

  if (sum == parityBit) {
        printf("Received data is error-free.\n");
        printf("Received message: %d\n", tempData);
    } else {
        printf("Received data contains errors.\n");
    }

    if (receivedData == '1'){

    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    analogWrite(ena, 134); //Setting the speed of the motor
    lcd.clear();                     
     lcd.setCursor (3, 0);             
     lcd.print("Diversion!"); // printing the message
     lcd.setCursor (1, 1);            
     lcd.print("Control Speed");
      }
      // Turn LED on
    else if (receivedData == '0'){

      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(ena, 180);
     lcd.clear();                      
     lcd.setCursor (4, 0);          
     lcd.print("Mainline");    
     lcd.setCursor (2, 1);           
     lcd.print("Maintain MPS");
    }
    client.stop();
  }
}

