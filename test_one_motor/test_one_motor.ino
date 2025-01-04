// Define motor pins for Motor 1 (Up-Down Motor 1)
const int MOTOR1_STEP_PIN = 3; // Step pin for Motor 1
const int MOTOR1_DIR_PIN = 2;  // Direction pin for Motor 1
const int MOTOR1_EN_PIN = 4;   // Enable pin for Motor 1

// Delay between steps in microseconds (adjust to control speed)
const int STEP_DELAY = 1000;

void setup() {
  // Set motor pins as output
  pinMode(MOTOR1_STEP_PIN, OUTPUT);
  pinMode(MOTOR1_DIR_PIN, OUTPUT);
  

  // Set initial direction to CW
  digitalWrite(MOTOR1_DIR_PIN, HIGH); // Set direction pin to CW
}

void loop() {
  // Generate continuous step pulses
  digitalWrite(MOTOR1_STEP_PIN, HIGH);
  delayMicroseconds(STEP_DELAY); // Wait for the step signal to register
  digitalWrite(MOTOR1_STEP_PIN, LOW);
  delayMicroseconds(STEP_DELAY); // Wait before the next step
}
