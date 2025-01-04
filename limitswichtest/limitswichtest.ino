
/*
    ARDUINO LIMIT SWITCH TUTORIAL: NORMALLY CLOSED
    By: TheGeekPub.com
    More Arduino Tutorials: https://www.thegeekpub.com/arduino-tutorials/
*/
 
#define LIMIT_SWITCH_PIN 2
 
void setup() {
  Serial.begin(9600);
  pinMode(LIMIT_SWITCH_PIN, INPUT);
}
void loop() {
 
  if (digitalRead(LIMIT_SWITCH_PIN) == HIGH)
  {
    //delay(1000);
    //if (digitalRead(LIMIT_SWITCH_PIN) == HIGH){
      Serial.println("Activated!");
    //} 
  }
   else {
         Serial.println("NOT Activated!");
   }
  delay(1000);
}