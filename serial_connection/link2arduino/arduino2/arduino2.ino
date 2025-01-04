#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Create a web server instance on port 80
WebServer server(80);

// Variable to store received data
String receivedData = "";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Define the endpoint to receive data from ESP32
  server.on("/receiveData", HTTP_POST, []() {
    if (server.hasArg("data")) {
      receivedData = server.arg("data");  // Get data sent from ESP32
      Serial.println("Received from ESP32: " + receivedData);
      server.send(200, "text/plain", "Data received.");
    } else {
      server.send(400, "text/plain", "No data received.");
    }
  });

  // Start the server
  server.begin();
}

void loop() {
  // Handle incoming HTTP requests
  server.handleClient();
}
