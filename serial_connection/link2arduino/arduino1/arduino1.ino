#include <WiFi.h>
#include <WiFiEnterprise.h>

const char* ssid = "your_SSID";
const char* username = "your_USERNAME";  // Username for the enterprise network
const char* password = "your_PASSWORD";  // Password for the enterprise network

void setup() {
  Serial.begin(115200);

  // Connecting to WPA2-Enterprise network
  WiFi.begin(ssid, username, password);
  Serial.println("Attempting to connect to Wi-Fi...");

  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }
}

void loop() {
  // Nothing in loop yet, just a connection test
}
