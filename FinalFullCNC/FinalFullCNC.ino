#include <Stepper.h>
#include <Servo.h>

// Define steps per revolution for the motors
#define STEPS_PER_REV 200

// Predefined positions for shelves and spice boxes
const int shelfPositions[] = {0, 700, 3900}; // Vertical positions (Home, Shelf 1, Shelf 2)
const int spiceBoxPositionsUpper[] = {0, 300, 650, 1050}; // Horizontal positions for upper shelf

// Stepper motor objects (6 motors with different pin assignments)

// Initialize the stepper library for two motors
Stepper motorRight(STEPS_PER_REV, 22, 23, 24, 25); // Motor 1 (Right) pins
Stepper motorLeft(STEPS_PER_REV, 28, 29, 30, 31);  // Motor 2 (Left) pins

Stepper stepper_telescopic(STEPS_PER_REV, 38, 39, 40, 41);
Stepper stepper_rotational(STEPS_PER_REV, 32, 33, 34, 35);
Stepper stepper_left_elevated(STEPS_PER_REV, 44, 45, 46, 47);
Stepper stepper_left_ground(STEPS_PER_REV, 50, 51, 52, 53);

// Limit switch pins
const int limitSwitchRight = 2;
const int limitSwitchLeft = 3;

// Flags to indicate when a limit switch is triggered
volatile bool rightMotorTriggered = false;
volatile bool leftMotorTriggered = false;

// Servo motor for the gripper
Servo gripper;

// Button pin
const int buttonPin = 10; // Connect your button to digital pin 10
int buttonState = 0; // Variable to store the button state

void setup() {

  // Set speed for both motors
  motorRight.setSpeed(80); // Set Motor 1 (Right) speed in RPM
  motorLeft.setSpeed(80);  // Set Motor 2 (Left) speed in RPM
  stepper_left_elevated.setSpeed(30); // Set Motor 5 speed in RPM
  stepper_telescopic.setSpeed(30);
  stepper_left_ground.setSpeed(30);
  stepper_rotational.setSpeed(30);
  // Attach servo motor
  gripper.attach(5); // Servo connected to pin 5

  // Set limit switches as input with pull-up resistors
  pinMode(limitSwitchRight, INPUT_PULLUP);
  pinMode(limitSwitchLeft, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  // Attach interrupts to limit switches
  attachInterrupt(digitalPinToInterrupt(limitSwitchRight), rightSwitchISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitSwitchLeft), leftSwitchISR, FALLING);

  // Initialize serial communication for debugging
  Serial.begin(9600);  
}

void loop() {
  // Read the button state
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) { // Button pressed (LOW if using INPUT_PULLUP)
    Serial.println("Button pressed! Starting full operation...");
    delay (3000);

    //do_all_things(2,3,1);
    stepper_rotational.step(230);
    delay(85);
    stepper_rotational.step(-230);
    // Small debounce delay to prevent re-triggering
    delay(300);
  }

  //gripperAction( 1);
}
void do_all_things(int shelf, int box ,int mode){
  //startgripperingSequence(shelf,box);
  //centerLeftelevated(box);
  //moveLeftGroundMotorground(1500);
  gripperAction( mode);


}
// Interrupt Service Routine for the right limit switch
void rightSwitchISR() {
  rightMotorTriggered = true;
}

// Interrupt Service Routine for the left limit switch
void leftSwitchISR() {
  leftMotorTriggered = true;
}

// Function to move both motors to their home positions
void homeBothMotors() {
  Serial.println("Homing both motors...");

  // Reset flags to ensure correct operation
  rightMotorTriggered = false;
  leftMotorTriggered = false;

  // Move motors until limit switches are triggered
  while (!rightMotorTriggered || !leftMotorTriggered) {
    if (!rightMotorTriggered) {
      motorRight.step(5); // Move Right motor CCW (toward home position)
    }
    if (!leftMotorTriggered) {
      motorLeft.step(5); // Move Left motor CCW (toward home position)
    }
    delay(10); // Small delay for smooth operation
  }
  Serial.println("Both motors homed.");
}

// Function to move both motors simultaneously
void moveTogether(int steps, bool up) {
  Serial.println(up ? "Moving both motors up..." : "Moving both motors down...");

  int increment = 10; // Small step size for smoother movement
  int totalSteps = abs(steps);

  for (int i = 0; i < totalSteps; i += increment) {
    int direction = up ? -1 : 1; // CW for up, CCW for down
    motorRight.step(direction * increment); // Move right motor
    motorLeft.step(direction * increment);  // Move left motor

    delay(5); // Short delay to synchronize movement
  }
}

// Function to move the left elevated motor horizontally
void moveLeftElevatedMotor(int steps) {
  Serial.print("Moving left_elevated Motor horizontally by steps: ");
  Serial.println(steps);

  int direction = steps > 0 ? -1 : 1; // Determine direction based on steps
  stepper_left_elevated.step(direction * abs(steps));

  Serial.println("Horizontal movement complete.");
}

// Function to move to a specific spice position
void moveToSpicePosition(int shelf, int box) {
  Serial.print("Moving to Shelf: ");
  Serial.print(shelf);
  Serial.print(", Box: ");
  Serial.println(box);

  // Home motors before moving to ensure accuracy
  homeBothMotors();

  // Move vertically to the specified shelf
  if (shelf >= 1 && shelf < sizeof(shelfPositions) / sizeof(shelfPositions[0])) {
    moveTogether(shelfPositions[shelf], true);
  } else {
    Serial.println("Invalid shelf position!");
    return;
  }

  // Move horizontally to the specified box
  if (box >= 0 && box < sizeof(spiceBoxPositionsUpper) / sizeof(spiceBoxPositionsUpper[0])) {
    moveLeftElevatedMotor(spiceBoxPositionsUpper[box]);
  } else {
    Serial.println("Invalid box position!");
    return;
  }

  Serial.println("Reached spice position.");
}

// Function to start the full operation sequence
void startgripperingSequence(int shelf, int box) {
  Serial.println("Starting operation sequence...");
  // Additional steps, e.g., moving to a spice position
  moveToSpicePosition(shelf, box);
  // Open servo to grip position
  gripper.write(70); // Open to 60 degrees
  delay(1000);       // Wait for servo to fully move
  // Move stepper motor forward
  stepper_telescopic.step(12000); // Move forward 1000 steps
  delay(1000); // Ensure movement completion
  // Close servo to release position
  gripper.write(55); // Return to 55 degrees
  delay(1000);       // Wait for servo to fully move
  // Move stepper motor backward
  stepper_telescopic.step(-12000); // Move backward 1000 steps
  delay(1000);
  Serial.println("Operation sequence complete.");
}

// Function to move the left ground motor horizontally 
void moveLeftGroundMotorground(int steps) {
  Serial.print("Moving left_elevated Motor horizontally by steps: ");
  Serial.println(steps);
  int direction = steps > 0 ? -1 : 1; // Determine direction based on steps
  stepper_left_ground.step(direction * abs(steps));
  Serial.println("Horizontal movement complete.");
}

void centerLeftelevated(int box) {
  // Array of step values for each box (index corresponds to the box value)
  int steps[] = {-550, -250, 100, 550};

  // Check if the box value is valid (between 0 and 3)
  if (box >= 0 && box < sizeof(steps) / sizeof(steps[0])) {
    Serial.print("Centering left elevated motor by ");
    Serial.print(steps[box]);
    Serial.println(" steps.");
    stepper_left_elevated.step(steps[box]);
    Serial.println("Motor movement complete.");
  } else {
    Serial.println("Invalid box value!");
  }
}

// Function to perform the gripper action based on the input mode
void gripperAction(int mode) {
  int steps = 230;     // Fixed steps for movement
  int delayTime = 85;  // Default delay time

  // Adjust delay and repetitions based on the mode
  int repetitions = 1; // Default repetitions
  if (mode == 2) {
    delayTime = 130; // Longer delay
  } else if (mode == 3) {
    delayTime = 130; // Longer delay
    repetitions = 2; // Perform action twice
  }

  // Execute the action based on repetitions
  for (int i = 0; i < repetitions; i++) {
    // Move forward to release salt
    stepper_rotational.step(steps);
    delay(delayTime);

    // Move backward to return to the initial position
    stepper_rotational.step(-steps);
    delay(delayTime);

    // Debugging output for each repetition
    Serial.print("Gripper action mode ");
    Serial.print(mode);
    Serial.print(", repetition ");
    Serial.println(i + 1);
  }
  // Debugging output after completion
  Serial.println("Gripper action completed.");
}