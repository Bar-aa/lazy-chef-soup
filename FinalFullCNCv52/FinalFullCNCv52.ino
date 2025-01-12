#include <Stepper.h>
#include <Servo.h>

// Define steps per revolution for the motors
#define STEPS_PER_REV 200

// Motor speeds (RPM)
#define SPEED_MAIN 40
#define SPEED_AUX 30

// Servo angles
#define GRIPPER_OPEN_ANGLE 70
#define GRIPPER_CLOSE_ANGLE 55

// Delays
#define ACTION_DELAY 1000
#define DEBOUNCE_DELAY 300

// Define the distance between shelves and gripper in steps
#define DISTANCE_BETWEEN_SHELVES_AND_GRIPPER 5200

// Define the shelf position
#define shelfPosition 2
#define mixPosition 1

#define mixtype1 1
#define mixtype2 2

// Define motor pins for Motor 1 (Up-Down Motor 1)
#define MOTOR1_STEP_PIN  9 // Step pin for Motor 1
#define MOTOR1_DIR_PIN  8  // Direction pin for Motor 1

// Define constants for spices
const int spiceTypes[] = {0, 1, 2, 3}; // Salt (0), Pepper (1), Onion (2), Chicken (3)


// Positions for shelves and spice boxes
const int shelfPositions[] = {0, 750, 3800,5250,-3200}; // Vertical positions
const int spiceBoxPositionsUpper[] = {0, 300, 650, 1070}; // Horizontal positions
const int spiceBoxPositionslower[] = {0,300,1050}; // Horizontal positions



const long int TOTAL_STEPS = 50000 ;  // Number of steps to move


Stepper stepperRotational(STEPS_PER_REV, 32, 34, 36, 38);
Stepper stepperTelescopic(STEPS_PER_REV, 33, 35, 37, 39);

Stepper stepperLeftElevated(STEPS_PER_REV, 42, 44, 46, 48);
Stepper stepperLeftGround(STEPS_PER_REV, 43, 45, 47, 49);

// Limit switch pins
const int limitSwitchRight = 2;
const int limitSwitchLeft = 3;
const int limitSwitchElevated = 21;


// Button pin
const int buttonPin = 53;

// Servo motor for the gripper
Servo gripper;

// Flags for limit switch triggers
volatile bool rightMotorTriggered = false;
volatile bool leftMotorTriggered = false;
volatile bool ElevatedMotorTriggered = false;

int itisup=0;

void setup() {

  // Set motor pins as output
  pinMode(MOTOR1_STEP_PIN, OUTPUT);
  pinMode(MOTOR1_DIR_PIN, OUTPUT);

  stepperTelescopic.setSpeed(50);
  stepperRotational.setSpeed(20);
  stepperLeftElevated.setSpeed(SPEED_AUX);
  stepperLeftGround.setSpeed(SPEED_AUX);

  // Attach servo motor
  gripper.attach(7);

  // Initialize limit switches and button
  pinMode(limitSwitchRight, INPUT_PULLUP);
  pinMode(limitSwitchLeft, INPUT_PULLUP);
  pinMode(limitSwitchElevated,INPUT_PULLUP);

  pinMode(buttonPin, INPUT_PULLUP);

  // // Attach interrupts to limit switches
  attachInterrupt(digitalPinToInterrupt(limitSwitchRight), rightSwitchISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitSwitchLeft), leftSwitchISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitSwitchElevated), elevatedSwitchISR, FALLING);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {

  delay (5000); 
  homeBothMotors();
}



void performOperation(int shelf, int box, int mode) {
  moveToSpicePosition(shelf, box);
  startgripperingSequence();
  centerLeftElevated(box);
  gripperAction(mode);
  returnBox(box);
  mixing(1);
}

void moveToSpicePosition(int shelf, int box) {
  if (isValidIndex(shelf, shelfPositions, sizeof(shelfPositions) / sizeof(shelfPositions[0]))) {
    if (itisup == 0) {
      moveBothMotors(shelfPositions[shelf], true);
      itisup = 1;
    }
  } else {
    Serial.println("Invalid shelf position!");
    return;
  }

  // Select the appropriate spice box positions array
  if (shelf == 2) {
    if (isValidIndex(box, spiceBoxPositionsUpper, sizeof(spiceBoxPositionsUpper) / sizeof(spiceBoxPositionsUpper[0]))) {
      moveStepper(stepperLeftElevated, spiceBoxPositionsUpper[box]);
    } else {
      Serial.println("Invalid box position!");
      return;
    } 
  } else if (shelf == 1) {
    if (isValidIndex(box, spiceBoxPositionslower, sizeof(spiceBoxPositionslower) / sizeof(spiceBoxPositionslower[0]))) {
      moveStepper(stepperLeftElevated, spiceBoxPositionslower[box]);
    } else {
      Serial.println("Invalid box position!");
      return;
    } 
  } else {
    Serial.println("Invalid shelf value!");
    return;
  }
  Serial.println("Spice position reached.");
}

void homeBothMotors() {
  Serial.println("Homing motors...");
  leftMotorTriggered = false;
  int directionPin=MOTOR1_DIR_PIN;
  int stepPin=MOTOR1_STEP_PIN;
  while (!leftMotorTriggered) {
    if (!leftMotorTriggered) {
      // Move right motor step (CCW for homing)
      digitalWrite(directionPin, HIGH); // Adjust HIGH/LOW based on wiring
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(100);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(100);
    }
    delay(10); // Optional for debounce
  }
}
void homeElevated() {
  Serial.println("Homing Elevated...");
  ElevatedMotorTriggered = false;
  while (!ElevatedMotorTriggered ) {
    if (!ElevatedMotorTriggered) stepperLeftElevated.step(5); // CCW
    delay(10);
  }
  Serial.println("Motors homed.");
}


void moveBothMotors(long int steps, bool up) {
  Serial.println(up ? "Moving both motors up..." : "Moving both motors down...");

  int directionPin=MOTOR1_DIR_PIN;
  int stepPin=MOTOR1_STEP_PIN;
  if (up) {
    digitalWrite(directionPin, LOW); // Up (CW)
  } else {
    digitalWrite(directionPin, HIGH); // Down (CCW)
  }

  // Perform the steps
  for (long int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100); // Adjust delay for speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
  }
}

void centerLeftElevated(int box) {
  static const int boxSteps[] = {-600, -250, 100, 550,-600,500};
  if (isValidIndex(box, boxSteps, sizeof(boxSteps) / sizeof(boxSteps[0]))) {
    stepperLeftElevated.step(boxSteps[box]);
    moveStepper(stepperLeftGround, 1500); // Adjust ground motor
  } else {
    Serial.println("Invalid box value!");
  }
}

void gripperAction(int mode) {
  const int steps = 230;
  const int delayTimes[] = {85, 130, 130};
  const int repetitions = (mode == 3) ? 2 : 1;
  delay(3000);
  for (int i = 0; i < repetitions; ++i) {
    stepperRotational.step(steps);    // Move forward
    delay(delayTimes[mode - 1]);
    delay(3000);
    stepperRotational.step(-steps);   // Move backward
    delay(delayTimes[mode - 1]);
  }
  Serial.println("Gripper action complete.");
}

// Function to start the full operation sequence
void startgripperingSequence() {
  Serial.println("Starting operation sequence...");
  // Open servo to grip position
  gripper.write(GRIPPER_OPEN_ANGLE); // Open to 60 degrees
  delay(ACTION_DELAY);       // Wait for servo to fully move
  // Move stepper motor forward
  stepperTelescopic.step(DISTANCE_BETWEEN_SHELVES_AND_GRIPPER); // Move forward 1000 steps
  delay(ACTION_DELAY); // Ensure movement completion
  // Close servo to release position
  gripper.write(GRIPPER_CLOSE_ANGLE); // Return to 55 degrees
  delay(ACTION_DELAY);       // Wait for servo to fully move
  // Move stepper motor backward
  stepperTelescopic.step(-DISTANCE_BETWEEN_SHELVES_AND_GRIPPER); // Move backward 1000 steps
  delay(ACTION_DELAY);
  Serial.println("Operation sequence complete.");
}

void moveStepper(Stepper &stepper, int steps) {
  stepper.step(steps > 0 ? -steps : steps); // Negative for CW
}

bool isValidIndex(int index, const  int arr[], int size) {
    return index >= 0 && index < size;
}

void rightSwitchISR() { rightMotorTriggered = true;Serial.println("DOOONNEER"); }
void leftSwitchISR() { leftMotorTriggered = true; Serial.println("DOOONNEEL");}
void elevatedSwitchISR(){ElevatedMotorTriggered=true; Serial.println("DOOONNEEEEE");}

void returnBox(int box){ 
  static const int boxSteps[] = {550, 250, -100, -550}; 
  moveBothMotors(200,true);
  stepperLeftElevated.step(boxSteps[box]);
  stepperLeftGround.step(1500); // Adjust ground motor
  stepperTelescopic.step(DISTANCE_BETWEEN_SHELVES_AND_GRIPPER);
  gripper.write(GRIPPER_OPEN_ANGLE); // Open to 60 degrees
  delay(ACTION_DELAY); 
  stepperTelescopic.step(-DISTANCE_BETWEEN_SHELVES_AND_GRIPPER);
  homeElevated();
}

void returnMixing(int box){
  static const int boxSteps[] = {0,600, -500};                                                                
  moveBothMotors(shelfPositions[4],true);
  stepperLeftElevated.step(boxSteps[box]);
  stepperLeftGround.step(1500); // Adjust ground motor
  stepperTelescopic.step(DISTANCE_BETWEEN_SHELVES_AND_GRIPPER);
  gripper.write(GRIPPER_OPEN_ANGLE); // Open to 60 degrees
  delay(ACTION_DELAY); 
  stepperTelescopic.step(-DISTANCE_BETWEEN_SHELVES_AND_GRIPPER);
  homeElevated();
}


void mixing(int type){
  //homeBothMotors();
  moveToSpicePosition(mixPosition,type);
  startgripperingSequence();
  moveBothMotors(shelfPositions[3],true);
  centerLeftElevated(type+3);
  moveBothMotors(shelfPositions[4],false );

  stepperTelescopic.step(4000);
  for(int i=0;i<2;i++){
     stepperLeftElevated.step(-300);
     stepperLeftElevated.step(600);
     stepperLeftElevated.step(-600);
     stepperLeftElevated.step(300);
     stepperLeftElevated.step(-300);
     stepperLeftElevated.step(600);
     stepperLeftElevated.step(-600);
     stepperLeftElevated.step(300);
     stepperTelescopic.step(3000);
     stepperTelescopic.step(-6000);
     stepperTelescopic.step(3000);
  }
  returnMixing(type);
}

