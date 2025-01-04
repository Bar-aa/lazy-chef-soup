#include <Stepper.h>

// Set the steps per revolution for your motor (verify with datasheet)
const int stepsPerRevolution = 200;

// Initialize the stepper motor (pins: 32, 33, 34, 35)
Stepper stepper_rotational(stepsPerRevolution, 32, 33, 34, 35);

void setup() {
  // Set motor speed in RPM
  stepper_rotational.setSpeed(30);

  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Test gripper action with different scenarios
  gripperAction(1); // One time with delay 85
  delay(3000);
  gripperAction(2); // One time with delay 130
  delay(3000);
  gripperAction(3); // Two times with delay 130
}

void loop() {
  // Continuously call the function as needed
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
