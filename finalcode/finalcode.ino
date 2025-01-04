#include <Stepper.h>
#include <Servo.h>

// Define steps per revolution for the motors
#define STEPS_PER_REV 200

// Stepper motor objects (6 motors with different pin assignments)
Stepper stepper_up_right(STEPS_PER_REV, 22, 23, 24, 25);
Stepper stepper_up_left(STEPS_PER_REV, 28, 29, 30, 31);
Stepper stepper_telescopic(STEPS_PER_REV, 38, 39, 40, 41);
Stepper stepper_rotational(STEPS_PER_REV, 32, 33, 34, 35);
Stepper stepper_left_elevated(STEPS_PER_REV, 44, 45, 46, 47);
Stepper stepper_left_ground(STEPS_PER_REV, 50, 51, 52, 53);

// Servo motor for the gripper
Servo gripper;

// Limit switch pins
const int limitSwitchRight = 2;
const int limitSwitchLeft = 3;
const int limitSwitchElevated = 4;3.

// Shelf heights and spice box positions
const int lower_shelf_height = 700; // Example height for lower shelf
const int upper_shelf_height = 4500; // Example height for upper shelf
const int spice_box_positions_lower[] = {100, 300, 500, 700}; // Positions on lower shelf
const int spice_box_positions_upper[] = {150, 350, 550, 750}; // Positions on upper shelf

// Flags to indicate when a limit switch is triggered
volatile bool rightMotorTriggered = false;
volatile bool leftMotorTriggered = false;
volatile bool leftElevatedMotorTriggered = false;
/*
// Function prototypes (declarations)
void rightSwitchISR();
void leftSwitchISR();
void homeBothMotors();
void moveToShelf(int height);
void moveHorizontalToPosition(int position);
void moveTelescopicForward(int steps);
void moveTelescopicBackward(int steps);
void rotate(int degree);
void grip(bool close);
*/
void setup() {
  // Set motor speeds
  stepper_up_left.setSpeed(100);
  stepper_up_right.setSpeed(100);
  stepper_left_ground.setSpeed(30);
  stepper_left_elevated.setSpeed(30);
  stepper_rotational.setSpeed(30);
  stepper_telescopic.setSpeed(30);
  

  // Attach servo motor
  gripper.attach(5); // Servo connected to pin 5

  // Set limit switches as input with pull-up resistors
  pinMode(limitSwitchRight, INPUT_PULLUP);
  pinMode(limitSwitchLeft, INPUT_PULLUP);
  pinMode(limitSwitchElevated, INPUT_PULLUP);

  // Attach interrupts to limit switches
  attachInterrupt(digitalPinToInterrupt(limitSwitchRight), rightSwitchISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitSwitchLeft), leftSwitchISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitSwitchElevated), elevatedSwitchISR, FALLING);



  Serial.begin(9600);
}

void loop() {
  
  homeBothMotors();
  homeMotor();

  
  /*moveToShelf(upper_shelf_height);
  gripper.write(60); // Set to 60° (open position)
  moveTelescopicForward(-2000);
  delay(2000); // Wait 1 second
  gripper.write(55); // Set to 55° (gripping position)
  delay(2000); // Wait 1 second
  moveTelescopicBackward(2000);
  delay(2000); // Wait 1 second*/
  //rotate(25);
  //rotate(25);
  
  //moveTelescopicForward(-2000);  
}


void homeBothMotors() {
  Serial.println("Homing both motors...");

  // Reset flags to ensure correct operation
  rightMotorTriggered = false;
  leftMotorTriggered = false;
  leftElevatedMotorTriggered= false;

  // Move motors until limit switches are triggered
  while (!rightMotorTriggered || !leftMotorTriggered ||!leftElevatedMotorTriggered) {
    if (!rightMotorTriggered) {
      stepper_up_right.step(5); // Move Right motor CCW (toward home position)
    }
    if (!leftMotorTriggered) {
      stepper_up_left.step(5); // Move Left motor CCW (toward home position)
    }
    if (!leftElevatedMotorTriggered){
      stepper_left_elevated.step(5);
    }
    delay(10); // Small delay for smooth operation
  }

  Serial.println("Both motors homed.");
}

void homeMotor() {
  Serial.println("Homing motor...");

  // Reset flags to ensure correct operation
  leftElevatedMotorTriggered= false;

  // Move motors until limit switches are triggered
  while (!leftElevatedMotorTriggered) {
    if (!leftElevatedMotorTriggered){
      stepper_left_elevated.step(5);
    }
    delay(10); // Small delay for smooth operation
  }

  Serial.println("motor homed.");
}

// Interrupt Service Routine for the right limit switch
void rightSwitchISR() {
  rightMotorTriggered = true;
}

// Interrupt Service Routine for the left limit switch
void leftSwitchISR() {
  leftMotorTriggered = true;
}

void elevatedSwitchISR(){
  leftElevatedMotorTriggered = true;
}

void moveToShelf(int height) {
  Serial.print("Moving to shelf height: ");
  Serial.println(height);
  int increment = 10; // Small step size for smoother movement
  int totalSteps = abs(height);

  for (int i = 0; i < totalSteps; i += increment) {
    stepper_up_left.step(-1 * increment); // Move right motor
    stepper_up_right.step(-1 * increment);  // Move left motor
    delay(5); // Short delay to synchronize movement
  }
}
/*
void moveHorizontalToPosition(int position) {
  Serial.print("Moving horizontally to position: ");
  Serial.println(position);

  // Example horizontal movement logic
  // while (current position < position) {
  //   stepper_left_ground.step(10);
  // }
}
*/
void moveTelescopicForward(int steps) {
  Serial.println("Extending telescopic motor.");
  stepper_telescopic.step(steps);
}

void moveTelescopicBackward(int steps) {
  Serial.println("Retracting telescopic motor.");
  stepper_telescopic.step(-steps);
}

void rotate(int degree) {
  Serial.print("Rotating by degrees: ");
  Serial.println(degree);

  int steps = (degree / 360.0) * STEPS_PER_REV;
  stepper_rotational.step(steps);
}
/*
void grip(bool close) {
  if (close) {
    Serial.println("Closing gripper.");
    gripper.write(0); // Adjust angle as needed
  } else {
    Serial.println("Opening gripper.");
    gripper.write(90); // Adjust angle as needed
  }
}


*/