// Define motor pins for Motor 1 (Up-Down Motor 1)
const int MOTOR1_DIR_PIN = 8;  // Direction pin for Motor 1
const int MOTOR1_STEP_PIN = 9; // Step pin for Motor 1
const int MOTOR1_EN_PIN = 10;   // Enable pin for Motor 1

// Define motor pins for Motor 2 (Up-Down Motor 2)
const int MOTOR2_DIR_PIN = 11;  // Direction pin for Motor 2
const int MOTOR2_STEP_PIN = 12; // Step pin for Motor 2
const int MOTOR2_EN_PIN = 13;   // Enable pin for Motor 2

// Define limit switch pins
const int LIMIT_SWITCH1_PIN = 2; // Limit switch for Motor 1
const int LIMIT_SWITCH2_PIN = 3; // Limit switch for Motor 2

// Delay between steps in microseconds (adjust to control speed)
const int STEP_DELAY = 1000;

void setup() {
  // Set motor pins as output for both motors
  pinMode(MOTOR1_STEP_PIN, OUTPUT);
  pinMode(MOTOR1_DIR_PIN, OUTPUT);
  pinMode(MOTOR1_EN_PIN, OUTPUT);

  pinMode(MOTOR2_STEP_PIN, OUTPUT);
  pinMode(MOTOR2_DIR_PIN, OUTPUT);
  pinMode(MOTOR2_EN_PIN, OUTPUT);

  // Set limit switch pins as input with pull-up resistors
  pinMode(LIMIT_SWITCH1_PIN, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH2_PIN, INPUT_PULLUP);

  // Enable both motors
  digitalWrite(MOTOR1_EN_PIN, LOW);  // Enable Motor 1
  digitalWrite(MOTOR2_EN_PIN, LOW);  // Enable Motor 2
}

void loop() {
  // Example usage
  moveUp(MOTOR1_STEP_PIN, MOTOR1_DIR_PIN, 1000);   // Move Motor 1 up (CW)
  moveUp(MOTOR2_STEP_PIN, MOTOR2_DIR_PIN, 1000);   // Move Motor 2 up (CW)
  delay(2000);                                    // Wait for 2 seconds

  goHome(MOTOR1_STEP_PIN, MOTOR1_DIR_PIN, MOTOR1_EN_PIN, LIMIT_SWITCH1_PIN); // Move Motor 1 home (CCW)
  goHome(MOTOR2_STEP_PIN, MOTOR2_DIR_PIN, MOTOR2_EN_PIN, LIMIT_SWITCH2_PIN); // Move Motor 2 home (CCW)
  delay(2000);                                    // Wait for 2 seconds
}

// Function to move a motor up (CW direction)
void moveUp(int stepPin, int dirPin, int steps) {
  digitalWrite(dirPin, HIGH); // Set direction to CW (UP)

  for (int i = 0; i < steps; i++) {
    // Generate one step pulse
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY);
  }
}

// Function to move a motor to the home position using limit switch (CCW direction)
void goHome(int stepPin, int dirPin, int enPin, int limitSwitchPin) {
  // Set direction to CCW (DOWN)
  digitalWrite(dirPin, LOW);

  // Keep stepping until the limit switch is triggered
  while (digitalRead(limitSwitchPin) == HIGH) {
    // Generate one step pulse
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY);
  }

  // Disable the motor after reaching home position
  digitalWrite(enPin, HIGH); // Disable the motor
}
