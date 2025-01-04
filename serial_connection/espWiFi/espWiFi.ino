void setup() {
  Serial.begin(115200);    // For debugging via USB
  Serial1.begin(9600);     // Communication with Arduino 1 (Sender)
  Serial2.begin(9600);     // Communication with Arduino 2 (Receiver)
}

void loop() {
  // If data is available from Arduino 1
  if (Serial1.available()) {
    char key = Serial1.read();  // Read key from Arduino 1
    Serial.print("Received key from Arduino 1: ");
    Serial.println(key);  // Print received key for debugging

    // Send the key to Arduino 2
    Serial2.write(key);  // Forward key to Arduino 2
  }
}
