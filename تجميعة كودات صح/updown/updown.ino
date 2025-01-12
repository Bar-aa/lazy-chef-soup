#include <Stepper.h>

// Set the steps per revolution for your motors
const int stepsPerRevolution = 200; // Replace with your motor's specification

const int firstShel = 700;
const int secondShelf =4500;
const int spice_box_positions_upper[] = {0, 300, 650, 1050}; // Positions on upper shelf


// Initialize the stepper library for two motors
Stepper motorRight(stepsPerRevolution, 22, 24, 26, 28); // Motor 1 (Right) pins
Stepper motorLeft(stepsPerRevolution, 23, 25, 27, 29);  // Motor 2 (Left) pins

// Limit switch pins
const int limitSwitchRight = 2; // Interrupt-capable pin for Motor 1 (Right)
const int limitSwitchLeft = 3;  // Interrupt-capable pin for Motor 2 (Left)
const int limitSwitchElevated = 7;

// Flags to indicate when a limit switch is triggered
volatile bool rightMotorTriggered = false;
volatile bool leftMotorTriggered = false;
volatile bool elevatedMotorTriggered = false;

void setup() {
  // Set speed for both motors
  motorRight.setSpeed(20); // Set Motor 1 (Right) speed in RPM
  motorLeft.setSpeed(20);  // Set Motor 2 (Left) speed in RPM
 
  // Set limit switches as input with pull-up resistors
  pinMode(limitSwitchRight, INPUT_PULLUP);
  pinMode(limitSwitchLeft, INPUT_PULLUP);
  pinMode(limitSwitchElevated, INPUT_PULLUP);
 
  

  // Attach interrupts to limit switches
  attachInterrupt(digitalPinToInterrupt(limitSwitchRight), rightSwitchISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitSwitchLeft), leftSwitchISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitSwitchElevated), elevatedSwitchISR, FALLING);

  // Initialize serial communication for debugging
  Serial.begin(9600);

}

void loop() {

  // Example loop logic: Move both motors after homing
  homeBothMotors();
  motorRight.step(-500);
  motorLeft.step(-500);
  // delay(500);
  // moveTogether(secondShelf, true); // Move up 200 steps
  // moveTogether(secondShelf, false); // Move up 200 steps
  // delay(500);
  // //homeBothMotors();
  // delay(500);
  // moveTogether(firstShel, true); // Move up 200 steps
  // delay(500);
}

// Function to move both motors to their home positions
void homeBothMotors() {
  Serial.println("Homing both motors...");
  
  // Reset flags to ensure correct operation
  rightMotorTriggered = false;
  leftMotorTriggered = false;

  // Move motors until limit switches are triggered
  while (!rightMotorTriggered || !leftMotorTriggered || !elevatedMotorTriggered) {
    if (!rightMotorTriggered) {
      motorRight.step(5); // Move Right motor CCW (toward home position)
    }
    if (!leftMotorTriggered) {
      motorLeft.step(5); // Move Left motor CCW (toward home position)
    }
    if (!elevatedMotorTriggered) {
      //motorLeft.step(5); // Move Left motor CCW (toward home position)
    }
    delay(10); // Small delay for smooth operation
  }
  Serial.println("Both motors homed.");
}

// Interrupt Service Routine for the right limit switch
void rightSwitchISR() {
  rightMotorTriggered = true;
  Serial.print("hhelo from right\n");
}

// Interrupt Service Routine for the left limit switch
void leftSwitchISR() {
  leftMotorTriggered = true;
  Serial.print("left\n");
}
void elevatedSwitchISR() {
  Serial.print("elevated\n");
  elevatedMotorTriggered = true;
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
