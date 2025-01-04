#include <Stepper.h>

// Set the steps per revolution for your motors
const int STEPS_PER_REV = 200; // Replace with your motor's specification

// Initialize the stepper library for two motors
Stepper stepper_telescopic(STEPS_PER_REV, 33, 35, 37, 39); 

// Servo motor for the gripper


void setup() {
  // Set speed for the stepper motor
  stepper_telescopic.setSpeed(55); // Set Motor 4 (telescopic) speed in RPM
  
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
}

void loop() {
 


  // Move stepper motor
  stepper_telescopic.step(-1200);
   delay(5000);

  // Reverse stepper motor
  stepper_telescopic.step(-1200);
}
