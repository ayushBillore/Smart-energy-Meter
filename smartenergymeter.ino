// Include necessary libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

// WiFi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// ThingSpeak details
unsigned long channelID = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_API_KEY";

WiFiClient client;

// Sensor pins
#define VOLTAGE_SENSOR A0  // ZMPT101B voltage sensor
#define CURRENT_SENSOR A1  // ACS712 current sensor

// Constants for sensor calibration
const float voltageFactor = 0.312; // Adjust based on calibration
const float currentFactor = 0.066; // Adjust based on ACS712 rating

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected!");
    ThingSpeak.begin(client);
}

void loop() {
    float voltage = readVoltage();
    float current = readCurrent();
    float power = voltage * current;

    Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
    Serial.print("Current: "); Serial.print(current); Serial.println(" A");
    Serial.print("Power: "); Serial.print(power); Serial.println(" W");

    ThingSpeak.setField(1, voltage);
    ThingSpeak.setField(2, current);
    ThingSpeak.setField(3, power);

    int response = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (response == 200) {
        Serial.println("Data sent successfully");
    } else {
        Serial.println("Failed to send data");
    }

    delay(15000); // Send data every 15 seconds
}

float readVoltage() {
    int sensorValue = analogRead(VOLTAGE_SENSOR);
    return sensorValue * voltageFactor;
}

float readCurrent() {
    int sensorValue = analogRead(CURRENT_SENSOR);
    return sensorValue * currentFactor;
}

