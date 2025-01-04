#define directionPin 3
#define stepPin 4

#define stepsPerRevolution 6400  // 6400 steps for one full revolution

void setup() {
  // put your setup code here, to run once:
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
}

void loop() {
  // Loop to rotate the stepper motor one revolution
  for (int i = 0; i < stepsPerRevolution; i++) {
    // Set the direction of rotation
    digitalWrite(directionPin, LOW);  // LOW for the opposite direction

    // Create one step pulse
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100);            // Control speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);            // Control speed
  }
}