#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Najah";              // Wi-Fi SSID
const char* username = "12028169";       // Wi-Fi username (for WPA2-Enterprise)
const char* password = "132119";       // Wi-Fi password

// Create a web server instance on port 80
WebServer server(80);

// Variables for the received data
String receivedData = "";

void setup() {
  Serial.begin(115200);  // For debugging

  // Connect to Wi-Fi with WPA2-Enterprise (using username and password)
  WiFi.begin(ssid, username, password);  // Use WiFi.begin with both username and password
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Define the endpoint to receive data from Arduino 1
  server.on("/sendData", HTTP_GET, []() {
    if (server.hasArg("data")) {
      receivedData = server.arg("data");  // Get data from Arduino 1
      Serial.println("Received from Arduino 1: " + receivedData);

      // Forward the data to Arduino 2 (another Wi-Fi request)
      forwardToArduino2(receivedData);

      server.send(200, "text/plain", "Data received and forwarded.");
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

// Function to forward data to Arduino 2 (via HTTP request)
void forwardToArduino2(String data) {
  // Assume Arduino 2 is accessible via its IP address in the same Wi-Fi network
  WiFiClient client;
  const char* arduino2IP = "192.168.1.100";  // Change this to Arduino 2's IP
  const int port = 80;

  if (client.connect(arduino2IP, port)) {
    String postData = "data=" + data;
    client.print(String("POST /receiveData HTTP/1.1\r\n") +
                 "Host: " + arduino2IP + "\r\n" +
                 "Content-Type: application/x-www-form-urlencoded\r\n" +
                 "Content-Length: " + postData.length() + "\r\n" +
                 "\r\n" + postData);
  }
  client.stop();
}
