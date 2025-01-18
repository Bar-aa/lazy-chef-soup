#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
String inputBuffer = ""; // To hold serial input

void setup() {
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC. Check your connections.");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the default time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
  }

  Serial.println("Send time in format: YYYY/MM/DD HH:MM:SS");
}

void loop() {
  // Check for Serial input
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') { // End of input
      processInput(inputBuffer); // Process the time input
      inputBuffer = ""; // Clear the buffer
    } else {
      inputBuffer += c; // Append to buffer
    }
  }

  // Print the current time
  DateTime now = rtc.now();
  // Serial.print("Current time: ");
  // Serial.print(now.year());
  // Serial.print('/');
  // Serial.print(now.month());
  // Serial.print('/');
  // Serial.print(now.day());
  // Serial.print(' ');
  // Serial.print(now.hour());
  // Serial.print(':');
  // Serial.print(now.minute());
  // Serial.print(':');
  // Serial.println(now.second());

  delay(1000); // Update every second
}

void processInput(String input) {
  int year, month, day, hour, minute, second;

  // Parse the input string
  if (sscanf(input.c_str(), "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6) {
    Serial.println("Turn buzzer on");
    if (now.hour() == hour && now.minute() == minute) {
      digitalWrite(buzzerPin, HIGH); // Buzzer ON
    } else {
      digitalWrite(buzzerPin, LOW); // Buzzer OFF
    }
  } else {
    Serial.println("Invalid format. Use: YYYY/MM/DD HH:MM:SS");
  }
}


// #include <Wire.h>
// #include <RTClib.h>

// RTC_DS3231 rtc;
// const int buzzerPin = 13;

// void setup() {
//   pinMode(buzzerPin, OUTPUT);
//   digitalWrite(buzzerPin, LOW);
  
//   Serial.begin(9600);

//   if (!rtc.begin()) {
//     Serial.println("Couldn't find RTC");
//     while (1);
//   }

//   if (rtc.lostPower()) {
//     Serial.println("RTC lost power, setting the time!");
//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//   }
// }

// void loop() {
//   DateTime now = rtc.now();

//   Serial.print("Current time: ");
//   Serial.print(now.hour());
//   Serial.print(":");
//   Serial.print(now.minute());
//   Serial.print(":");
//   Serial.println(now.second());

//   // Turn buzzer on at 7:00:00 and off at 7:05:00
//   if (now.hour() == 9 && now.minute() >= 50 && now.minute() <51 ) {
//     digitalWrite(buzzerPin, HIGH); // Buzzer ON
//   } else {
//     digitalWrite(buzzerPin, LOW); // Buzzer OFF
//   }

//   delay(1000); // Update every second
// }



// #include <Wire.h>
// #include <RTClib.h>

// RTC_DS3231 rtc;

// void setup() {
//   Serial.begin(9600);

//   if (!rtc.begin()) {
//     Serial.println("Couldn't find RTC. Check your connections.");
//     while (1);
//   }

//   if (rtc.lostPower()) {
//     Serial.println("RTC lost power, setting the time!");
//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
//   }
// }

// void loop() {
//   DateTime now = rtc.now();

//   Serial.print("Current time: ");
//   Serial.print(now.year());
//   Serial.print('/');
//   Serial.print(now.month());
//   Serial.print('/');
//   Serial.print(now.day());
//   Serial.print(' ');
//   Serial.print(now.hour());
//   Serial.print(':');
//   Serial.print(now.minute());
//   Serial.print(':');
//   Serial.println(now.second());

//   delay(1000); // Update every second
// }
