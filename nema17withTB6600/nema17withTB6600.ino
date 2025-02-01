// Define motor pins for Motor 1 (Up-Down Motor 1)
const int MOTOR1_STEP_PIN = 23; // Step pin for Motor 1
const int MOTOR1_DIR_PIN = 22;  // Direction pin for Motor 1
const long int TOTAL_STEPS = 50000;  // Number of steps to move

const int limitSwitchLeft = 3; // Pin for the limit switch

// Move stepper motor for a specific number of steps
void moveStepper( long int steps, bool isUp) {
  int directionPin=MOTOR1_DIR_PIN;
   int stepPin= MOTOR1_STEP_PIN;
  // Set the direction based on isUp parameter
  if (isUp) {
    digitalWrite(directionPin, LOW); // Up (CW)
    for (long int i = 0; i < steps; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(100); // Adjust delay for speed
      digitalWrite(stepPin, LOW);
      delayMicroseconds(100); // Adjust delay for speed
    }
  } else {
    digitalWrite(directionPin, HIGH); // Down (CCW)
    for (long int i = 0; i < steps; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(100); // Adjust delay for speed
      digitalWrite(stepPin, LOW);
      delayMicroseconds(100); // Adjust delay for speed
    }
  }  
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Set motor pins as output
  pinMode(MOTOR1_STEP_PIN, OUTPUT);
  pinMode(MOTOR1_DIR_PIN, OUTPUT);

  // Set limit switch pin as input with pull-up resistor
  pinMode(limitSwitchLeft, INPUT_PULLUP);
}

void loop() {
  // Move motor down
    // Move motor up
  delay(5000);
  Serial.println("Moving motor up...");
  moveStepper(22600, false);
  Serial.println("Moving motor down...");
  moveStepper(22600, true);
}
