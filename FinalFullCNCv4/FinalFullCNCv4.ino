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



// Define constants for spices
const int spiceTypes[] = {0, 1, 2, 3}; // Salt (0), Pepper (1), Onion (2), Chicken (3)


// Positions for shelves and spice boxes
const int shelfPositions[] = {0, 750, 3800,5250}; // Vertical positions
const int spiceBoxPositionsUpper[] = {0, 300, 650, 1070}; // Horizontal positions
const int spiceBoxPositionslower[] = {0,300,7050}; // Horizontal positions


// Stepper motor objects
Stepper motorRight(STEPS_PER_REV, 22, 24, 26, 28); // Motor 1 (Right)
Stepper motorLeft(STEPS_PER_REV, 23, 25, 27, 29);  // Motor 2 (Left)

Stepper stepperRotational(STEPS_PER_REV, 32, 34, 36, 38);
Stepper stepperTelescopic(STEPS_PER_REV, 33, 35, 37, 39);

Stepper stepperLeftElevated(STEPS_PER_REV, 42, 44, 46, 48);
Stepper stepperLeftGround(STEPS_PER_REV, 43, 45, 47, 49);

// Limit switch pins
const int limitSwitchRight = 2;
const int limitSwitchLeft = 3;

// Button pin
const int buttonPin = 0;

// Servo motor for the gripper
Servo gripper;

// Flags for limit switch triggers
volatile bool rightMotorTriggered = false;
volatile bool leftMotorTriggered = false;

int itisup=0;

void setup() {
  // Initialize motor speeds
  motorRight.setSpeed(SPEED_MAIN);
  motorLeft.setSpeed(SPEED_MAIN);
  stepperTelescopic.setSpeed(50);
  stepperRotational.setSpeed(20);
  stepperLeftElevated.setSpeed(SPEED_AUX);
  stepperLeftGround.setSpeed(SPEED_AUX);

  // Attach servo motor
  gripper.attach(7);

  // Initialize limit switches and button
  pinMode(limitSwitchRight, INPUT_PULLUP);
  pinMode(limitSwitchLeft, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  // Attach interrupts to limit switches
  attachInterrupt(digitalPinToInterrupt(limitSwitchRight), rightSwitchISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitSwitchLeft), leftSwitchISR, FALLING);
  //attachInterrupt(digitalPinToInterrupt(limitSwitchElevated), elevatedSwitchISR, FALLING);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {

  //stepperTelescopic.step(-200);
  delay(7000);
  Serial.println("Running.............");
  digitalWrite(buttonPin, LOW);
  //if (digitalRead(buttonPin) == LOW) { // Button pressed
    Serial.println("Button pressed! Starting operation...");
    delay(DEBOUNCE_DELAY);
    //performOperation(2, 2, 3); // the second number for the spice type 0 is the first 
    char input[] = "A13#";  // Format: <Character><Mod><Exclude># 
    /*homeBothMotors();
    handleCharacters(input);*/
    gripperAction(1);

  //}  
}
// Function to handle multiple character inputs
void handleCharacters(char characters[]) {
  //homeBothMotors(); // Ensure starting position
  if (characters[3]=='#'){
    int mod=characters[1] -'0';
    int exclude = characters[2] -'0'; 
    switch (characters[0]) {
        case 'A':
          for (int i = 1; i < 2; i++) {
            if (i != 2 && i != exclude) {
              performOperation(shelfPosition, i, mod);
            }
          }
          break;
        case 'K':
          for (int i = 0; i < 2; i++) {
            if (i != exclude) {
              performOperation(shelfPosition, i, mod);
            }
          }
          break;
        case 'D':
        case 'S': 
          for (int i = 0; i < 4; i++) {
            if (i!= exclude) {
              performOperation(shelfPosition, i, mod);
            }
          }
          break;
        default:
          Serial.println("Invalid character received!");
          break;
      }
  }        
}

void performOperation(int shelf, int box, int mode) {
  moveToSpicePosition(shelf, box);
  startgripperingSequence();
  centerLeftElevated(box);
  gripperAction(mode);
  returnBox(box);
}

void moveToSpicePosition(int shelf, int box) {
  if (isValidIndex(shelf, shelfPositions, sizeof(shelfPositions) / sizeof(shelfPositions[0]))) {
    if (itisup==0){
      moveBothMotors(shelfPositions[shelf], true);
      itisup=1;
    }
  } else {
    Serial.println("Invalid shelf position!");
    return;
  }
  if (isValidIndex(box, spiceBoxPositionsUpper, sizeof(spiceBoxPositionsUpper) / sizeof(spiceBoxPositionsUpper[0]))) {
    moveStepper(stepperLeftElevated, spiceBoxPositionsUpper[box]);
  } else {
    Serial.println("Invalid box position!");
    return;
  }
  Serial.println("Spice position reached.");
}



void homeBothMotors() {
  Serial.println("Homing motors...");
  rightMotorTriggered = leftMotorTriggered = false;
  while (!rightMotorTriggered || !leftMotorTriggered  ) {
    if (!rightMotorTriggered) motorRight.step(5); // CCW
    if (!leftMotorTriggered) motorLeft.step(5);  // CCW
    delay(10);
  }

  Serial.println("Motors homed.");
}

void moveBothMotors(int steps, bool up) {
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

void centerLeftElevated(int box) {
  static const int boxSteps[] = {-600, -250, 100, 550};
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

bool isValidIndex(int index, const int arr[], int size) {
    return index >= 0 && index < size;
}

void rightSwitchISR() { rightMotorTriggered = true; }
void leftSwitchISR() { leftMotorTriggered = true; }

void returnBox(int box){ 
  static const int boxSteps[] = {550, 250, -100, -550};                                                                
  moveBothMotors(200,true);
  stepperLeftElevated.step(boxSteps[box]);
  stepperLeftGround.step(1500); // Adjust ground motor
  stepperTelescopic.step(DISTANCE_BETWEEN_SHELVES_AND_GRIPPER);
  gripper.write(GRIPPER_OPEN_ANGLE); // Open to 60 degrees
  delay(ACTION_DELAY); 
  stepperTelescopic.step(-DISTANCE_BETWEEN_SHELVES_AND_GRIPPER);
  stepperLeftElevated.step(spiceBoxPositionsUpper[box]);
}
