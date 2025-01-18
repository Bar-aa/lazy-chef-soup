char Mymessage[5]; // Initialized variable to store received data (including space for null terminator)

void setup() {
  Serial.begin(9600);  // Begin the Serial at 9600 Baud
}

void loop() {
  if (Serial.available() > 0) {  // Check if data is available
    Serial.readBytes(Mymessage, 4);  // Read 1 byte of serial data and store in the variable
    Mymessage[4] = '\0';  // Add null terminator to make it a proper string
    Serial.println(Mymessage);  // Print data on Serial Monitor
  }
  delay(100);  // Optional small delay for stability
} 