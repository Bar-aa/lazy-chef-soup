#include <Stepper.h>
#include <Servo.h>

// Define steps per revolution for the motors
#define STEPS_PER_REV 200


// Servo angles
#define GRIPPER_OPEN_ANGLE 70
#define GRIPPER_CLOSE_ANGLE 53
#define LeftGround 1390

// Delays
#define ACTION_DELAY 1000
#define DEBOUNCE_DELAY 300

// Distance between shelves and gripper in steps
#define DISTANCE_BETWEEN_SHELVES_AND_GRIPPER 180000

// Motor speeds (RPM)
#define MAX_SPEED 40
#define MIN_SPEED 30

// Define motor pins for Motor 1 (Up-Down Motor 1)
#define MOTOR1_STEP_PIN  9 // Step pin for Motor 1
#define MOTOR1_DIR_PIN  8  // Direction pin for Motor 1

// Define motor pins for Telescopic Motor 
const int Telescopic_STEP_PIN = 11; // Step pin for Motor 1
const int Telescopic_DIR_PIN = 10;  // Direction pin for Motor 1

Stepper stepperRotational(STEPS_PER_REV, 32, 34, 36, 38);
Stepper stepperLeftElevated(STEPS_PER_REV, 42, 44, 46, 48);
Stepper stepperLeftGround(STEPS_PER_REV, 43, 45, 47, 49);

// Limit switch pins
const int limitSwitchRight = 2;
const int limitSwitchLeft = 3;
const int limitSwitchElevated = 21;


// Servo motor for the gripper
Servo gripper;

// Flags for limit switch triggers
volatile bool rightMotorTriggered = false;
volatile bool leftMotorTriggered = false;
volatile bool ElevatedMotorTriggered = false;

// Positions for shelves and spice boxes
const int long shelfPositions[] = {30000, 130000}; // Vertical positions

// Define constants for spices
const int spiceTypes[] = {0, 1, 2, 3}; // Salt (0), Pepper (1), Onion (2), Chicken (3)
const int spiceBoxPositionsUpper[] = {0, 300, 660, 1070}; // Horizontal positions
const int centerBoxPositionsUpper[] = {-600, -250, 100, 550}; // center Horizontal positions


// Define constants for mixing
const int mixingTypes[] = {0, 1}; // sponn (0), mixer (1)
const int spiceBoxPositionslower[] = {300,1050}; // Horizontal positions
const int centerBoxPositionslower[] = {300,1050}; // Horizontal positions


// Define a structure to represent each soup
struct Soup {
  const int* requiredSpices;  // Array of spice types needed
  int spiceCount;             // Number of spices
  int mixingTool;             // Mixing tool required (0: Spoon, 1: Mixer)
};

// Spice arrays for specific soups
const int ADASSpices[] = {2};  // Salt, Chicken
const int KHODARSpices[] = {0, 1, 2};  // Salt, Pepper, Onion
const int SHERASpices[] = {1};  // Pepper

// Define soups
const Soup soups[] = {
  {ADASSpices, 1, 0},  // Soup 1 (Spoon)
  {KHODARSpices, 3, 1},  // Soup 2 (Mixer)
  {SHERASpices, 1, 0}  // Soup 3 (Spoon)
};

void setup() {
  // Set motor pins as output
  pinMode(MOTOR1_STEP_PIN, OUTPUT);
  pinMode(MOTOR1_DIR_PIN, OUTPUT);

  // Set motor pins as output
  pinMode(Telescopic_DIR_PIN, OUTPUT);
  pinMode(Telescopic_STEP_PIN, OUTPUT);

  // Initialize motor speeds
  stepperRotational.setSpeed(25);
  stepperLeftElevated.setSpeed(MAX_SPEED);
  stepperLeftGround.setSpeed(MAX_SPEED);

  // Attach servo motor
  gripper.attach(7);

  // Initialize limit switches and button
  pinMode(limitSwitchRight, INPUT_PULLUP);
  pinMode(limitSwitchLeft, INPUT_PULLUP);
  pinMode(limitSwitchElevated,INPUT_PULLUP);

 
  // // Attach interrupts to limit switches
  attachInterrupt(digitalPinToInterrupt(limitSwitchRight), rightSwitchISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitSwitchElevated), elevatedSwitchISR, FALLING);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  delay (5000);
  char serial_input[5]="029#";
  handleCharacters(serial_input);
  
  
}

// Function to handle multiple character inputs
void handleCharacters(char characters[]) {
  homeBothMotors();
  homeElevated();
  if (strlen(characters) < 4 || characters[3] != '#') {
    Serial.println("Invalid input format.");
    return;
  }
  if (characters[3]=='#'){
    int mod=characters[1] -'0';
    int exclude = characters[2] -'0'; 
    int selectedSoup =characters[0] -'0';
    performOperation(selectedSoup,mod,exclude);
  }        
}
void performOperation(int selectedSoup, int mode, int exclude){
  const Soup& soup = soups[selectedSoup]; 
  putSpicesIntoSoup(soup, mode, exclude);
}

void putSpicesIntoSoup(const Soup& selectedSoup,int mode, int exclude) {
  // Loop through each spice required for the soup
  for (int i = 0; i < selectedSoup.spiceCount; i++) {
    int spiceType = selectedSoup.requiredSpices[i];
    long int  shelf = shelfPositions[1]; 
    int box = spiceBoxPositionsUpper[spiceType];
    moveToSpicePosition(shelf, box);
    startgripperingSequence(true);
    centerLeftElevated(box);
    gripperAction(mode);
    returnBox(box);
    homeElevated();
  }
  homeBothMotors();
}
void moveToSpicePosition(long int shelf, int box) {
  Serial.println(shelf);
  Serial.println(box);
  moveBothMotors(shelf,true);
  stepperLeftElevated.step(-box);
  Serial.println("Spice position reached.");
  delay (1000);
}

// Move stepper motor for a specific number of steps
void moveBothMotors( long int steps, bool isUp) {
  int directionPin=MOTOR1_DIR_PIN;
   int stepPin= MOTOR1_STEP_PIN;
  // Set the direction based on isUp parameter
  if (isUp) {
    digitalWrite(directionPin, LOW); // Up (CW)
    for (long int i = 0; i < steps; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(70); // Adjust delay for speed
      digitalWrite(stepPin, LOW);
      delayMicroseconds(70);
    }
  } else {
    digitalWrite(directionPin, HIGH); // Down (CCW)
    for (long int i = 0; i < steps; i++) {
      if (digitalRead(limitSwitchLeft) == LOW) { // Check if limit switch is triggered
        Serial.println("Limit switch triggered, stopping motor.");
        return; // Stop motor movement
      }
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(100); // Adjust delay for speed
      digitalWrite(stepPin, LOW);
      delayMicroseconds(100);
    }
  }  
}

// Function to start the full operation sequence
void startgripperingSequence(bool isTakingBox) {
  int startAngle, endAngle;
  if (isTakingBox) { 
    startAngle = GRIPPER_OPEN_ANGLE;
    endAngle = GRIPPER_CLOSE_ANGLE;
  } else { 
    startAngle = GRIPPER_CLOSE_ANGLE;
    endAngle = GRIPPER_OPEN_ANGLE;
  }
  Serial.println("Starting operation sequence...");
  // Open servo to grip position
  gripper.write(startAngle); // Open to 60 degrees
  delay(ACTION_DELAY);       // Wait for servo to fully move
  // Move stepper motor forward
  Serial.println("Starting operation sequence with start Angle");
  moveTelescopic(DISTANCE_BETWEEN_SHELVES_AND_GRIPPER, true);
  delay(ACTION_DELAY); // Ensure movement completion
  // Close servo to release position
  gripper.write(endAngle); // Return to 55 degrees
  delay(ACTION_DELAY);       // Wait for servo to fully move
  // Move stepper motor backward
  Serial.println("Starting operation sequence with end Angle");
  moveTelescopic(DISTANCE_BETWEEN_SHELVES_AND_GRIPPER, false);
  delay(ACTION_DELAY);
  Serial.println("Operation sequence complete.");
}

void centerLeftElevated(int box) {
  stepperLeftElevated.step(centerBoxPositionsUpper[box]);
  stepperLeftGround.step(-LeftGround);
}
void gripperAction(int mode) {
  Serial.println("const int steps = 230;");
  const int steps = 230;
  const int delayTimes[] = {85, 130, 130};
  const int repetitions = (mode == 3) ? 2 : 1;
  delay(3000);
  for (int i = 0; i < repetitions; ++i) {
    Serial.println("inside the loop");
    stepperRotational.step(steps);    // Move forward
    Serial.println("after the inside the loop");
    delay(delayTimes[mode - 1]);
    delay(3000);
    stepperRotational.step(-steps);   // Move backward
    delay(delayTimes[mode - 1]);
  }
  Serial.println("Gripper action complete.");
  
}

void returnBox(int box){ 
  stepperLeftElevated.step(-centerBoxPositionsUpper[box]);
  stepperLeftGround.step(LeftGround); // Adjust ground motor
  moveBothMotors(5000, true);
  startgripperingSequence(false);
  homeElevated();
}

void homeBothMotors(){
  int directionPin=MOTOR1_DIR_PIN;
  int stepPin= MOTOR1_STEP_PIN;
  int flag =1;
  digitalWrite(directionPin, HIGH); // Down (CCW)
    while(flag) {
      if (digitalRead(limitSwitchLeft) == LOW) { // Check if limit switch is triggered
        Serial.println("Limit switch triggered, stopping motor.");
        flag=0;
        return; // Stop motor movement
      }
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100); // Adjust delay for speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
  }
}
void homeElevated() {
  Serial.println("Homing Elevated...");
  ElevatedMotorTriggered = false;
  while (!ElevatedMotorTriggered ) {
    if (!ElevatedMotorTriggered) stepperLeftElevated.step(5); // CCW
    delay(10);
  }
  Serial.println("Motor homed.");
}

// Move stepper motor for a specific number of steps
void moveTelescopic( long int steps, bool isForward) {
  int directionPin =Telescopic_DIR_PIN;
  int stepPin = Telescopic_STEP_PIN;
  // Set the direction based on isUp parameter
  if (isForward) {
    digitalWrite(directionPin, LOW); 
    for (long int i = 0; i < steps; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(10); // Adjust delay for speed
      digitalWrite(stepPin, LOW);
      delayMicroseconds(10);
    }
  } else {
      digitalWrite(directionPin, HIGH);
      for (long int i = 0; i < steps; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(10); // Adjust delay for speed
      digitalWrite(stepPin, LOW);
      delayMicroseconds(10);
    }
  }  
}

void rightSwitchISR() { rightMotorTriggered = true;Serial.println("DOOONNEER"); }
void elevatedSwitchISR(){ElevatedMotorTriggered=true; Serial.println("DOOONNEEEEE");}


