
// /*
//     ARDUINO LIMIT SWITCH TUTORIAL: NORMALLY CLOSED
//     By: TheGeekPub.com
//     More Arduino Tutorials: https://www.thegeekpub.com/arduino-tutorials/
// */
 
// #define LIMIT_SWITCH_PIN 2
 
// void setup() {
//   Serial.begin(9600);
//   pinMode(LIMIT_SWITCH_PIN, INPUT);
//   attachInterrupt(digitalPinToInterrupt(limitSwitchRight), rightSwitchISR, FALLING);
// }
// void loop() {
 
//   if (digitalRead(LIMIT_SWITCH_PIN) == HIGH)
//   {
//     //delay(1000);
//     //if (digitalRead(LIMIT_SWITCH_PIN) == HIGH){
//       Serial.println("Activated!");
//     //} 
//   }
//    else {
//          Serial.println("NOT Activated!");
//    }
//   delay(1000);
// }

void setup() {
  // Initialize the onboard LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); // Wait for 1 second

  // Turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // Wait for 1 second
}
