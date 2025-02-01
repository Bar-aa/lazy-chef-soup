

// // // #include <Keypad.h>
// // // #include <Stepper.h> 
// // // #include <Wire.h>
// // // #include <RTClib.h>
// // // #include <string.h> 
// // // #include <SoftwareSerial.h>

// // // #define TRIG_PIN 9
// // // #define ECHO_PIN 10

// // // RTC_DS3231 rtc;



// // // long measureDistance(int trigPin, int echoPin) {
// // //   digitalWrite(trigPin, LOW);
// // //   delayMicroseconds(2);
// // //   digitalWrite(trigPin, HIGH);
// // //   delayMicroseconds(10);
// // //   digitalWrite(trigPin, LOW);
// // //   long duration = pulseIn(echoPin, HIGH);
// // //   long distance = duration * 0.034 / 2; 
// // //   return distance;
// // // }

// // // String measureAllDistances() {
// // //   int trigPins[] = {TRIG_PIN, TRIG_PIN, TRIG_PIN};
// // //   int echoPins[] = {ECHO_PIN, ECHO_PIN, ECHO_PIN};
// // //   String result = "";
// // //   for (int i = 0; i < 3; i++) {
// // //     pinMode(trigPins[i], OUTPUT);
// // //     pinMode(echoPins[i], INPUT);
// // //     long distance = measureDistance(trigPins[i], echoPins[i]);
// // //     result += String(distance);
// // //     if (i < 2) {
// // //       result += ","; // Add a comma as a delimiter
// // //     }
// // //   }
// // //   return result;
// // // }

// // // void setup() {
// // //   pinMode(TRIG_PIN, OUTPUT);
// // //   pinMode(ECHO_PIN, INPUT);
// // //   Serial.begin(9600);
// // // }

// // // void loop() {
// // //   if (Serial.available() > 0) {
// // //     String input = Serial.readStringUntil('\n');
// // //     input.trim();
// // //     if (input == "R") {
// // //       String distances = measureAllDistances();
// // //       Serial.println(distances); // Send distances as a single string
// // //     }
// // //   }
// // // }


// Define pins for the ultrasonic sensor

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Measure the distance
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    Serial.println(input);
    if (input == "R") {
      
      Serial.println("distances");
    }
 }
}



