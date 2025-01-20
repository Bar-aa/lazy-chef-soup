#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
String inputBuffer = ""; // To hold serial input
const int buzzerPin = 13; // Pin for buzzer

int targetHour, targetMinute; // Target time
bool isTargetSet = false; // Flag to check if the target time is set

void setup() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Serial1.begin(9600);
Serial.begin(9600);
  /*if (!rtc.begin()) {
    Serial.println("Couldn't find RTC. Check your connections.");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the default time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
  }

  Serial.println("Send time in format: HH:MM to set buzzer time");*/
}

void loop() {
  // Check for Serial input
  
  
    //Serial.println("serialInput = ");
  if (Serial1.available()>0) {
    String serialInput = Serial1.readStringUntil('\n');
    serialInput.trim();
    Serial.println("serialInput = ");
    Serial.println(serialInput);
    /*if (c == '\n') { // End of input
      processInput(inputBuffer); // Process the time input
      inputBuffer = ""; // Clear the buffer
    } else {
      inputBuffer += c; // Append to buffer
    }*/
  }
}
/*
  // Get the current time
  DateTime now = rtc.now();

  // Debug: Print the current time
  Serial.print("Current time: ");
  Serial.print(now.hour());
  Serial.print(':');
  Serial.println(now.minute());

  // Check if target time matches the current time
  if (isTargetSet &&
      now.hour() == targetHour &&
      now.minute() == targetMinute) {
    digitalWrite(buzzerPin, HIGH); // Buzzer ON
    Serial.println("Buzzer ON! Matching time detected.");
  } else {
    digitalWrite(buzzerPin, LOW); // Buzzer OFF
  }

  delay(1000); // Update every second
}

void processInput(String input) {
  // Debug: Print the received input
  Serial.print("Received input: ");
  Serial.println(input);

  // Parse the input string
  if (sscanf(input.c_str(), "%d:%d", &targetHour, &targetMinute) == 2) {
    isTargetSet = true; // Set the target time flag
    Serial.println("Buzzer time set.");
    Serial.print("Target time: ");
    Serial.print(targetHour);
    Serial.print(':');
    Serial.println(targetMinute);
  } else {
    Serial.println("Invalid format. Use: HH:MM");
  }
}
*/
