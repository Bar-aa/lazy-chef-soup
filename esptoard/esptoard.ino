#include <WiFi.h>

// Server - AP
const char* ssid = "Areej Baraa";
const char* password = "areejbaraa";

WiFiServer server(80);

// Hardware serial port for Arduino (Use Serial2 or another Serial port if available)
#define RXD2 16  // GPIO16 (RX pin for ESP32)
#define TXD2 17  // GPIO17 (TX pin for ESP32)

void setup() {
  Serial.begin(9600);           // For debugging
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // For Arduino communication

  WiFi.softAP(ssid, password);  // Start Wi-Fi Access Point
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    String postData = "";
    bool isPost = false;
    int contentLength = 0;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            if (isPost) {
              while (postData.length() < contentLength) {
                if (client.available()) {
                  char c = client.read();
                  postData += c;
                }
              }
              Serial.println("Received from App:");
              Serial.println(postData);

              // Forward the data to Arduino
              Serial2.println(postData);
            } else {
              // Handle GET request
              String arduinoData = "";
              if (Serial2.available()) {
                arduinoData = Serial2.readStringUntil('\n');
              }

              String httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n" + arduinoData;
              client.print(httpResponse);
              Serial.println("Sent to App:");
              Serial.println(arduinoData);
            }
            break;
          } else {
            // Parse headers
            if (currentLine.startsWith("Content-Length: ")) {
              contentLength = currentLine.substring(16).toInt();
            }

            if (currentLine.startsWith("POST ")) {
              isPost = true;
            }

            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}
