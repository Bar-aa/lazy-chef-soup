#include <Stepper.h>

// Set the steps per revolution for your motors
const int STEPS_PER_REV = 200; // Replace with your motor's specification

// Initialize the stepper library for two motors
Stepper stepper_left_elevated(STEPS_PER_REV, 44, 45, 46, 47);
Stepper stepper_left_ground(STEPS_PER_REV, 50, 51, 52, 53);

void setup() {
  // Set speed for the stepper motor
  stepper_left_elevated.setSpeed(30); // Set Motor 5 speed in RPM
  stepper_left_ground.setSpeed(30); // Set Motor 5 speed in RPM
  
  // Initialize serial communication for debugging
  Serial.begin(9600);

}

void loop() {
  
 /* delay(3000);
  // Move stepper motor5
  stepper_left_elevated.step(2000);
  delay(3000);
  // Move stepper motor5
  stepper_left_elevated.step(-2000);
  delay(3000);
  // Move stepper motor6
  stepper_left_elevated.step(2000);
  delay(3000);*/
  
 delay(3000);
  // Move stepper motor5
  stepper_left_ground.step(1500);
  delay(3000);
  // Move stepper motor5
  stepper_left_ground.step(-1500);
  delay(3000);


}
