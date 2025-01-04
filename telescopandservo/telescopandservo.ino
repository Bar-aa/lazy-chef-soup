#include <Stepper.h>
#include <Servo.h>

// Set the steps per revolution for your motors
const int STEPS_PER_REV = 200; // Replace with your motor's specification

// Initialize the stepper library for the motor
Stepper stepper_telescopic(STEPS_PER_REV, 33, 35, 37, 39);

//Stepper stepperTelescopic(STEPS_PER_REV, 33, 35, 37, 39);

// Servo motor for the gripper
//Servo gripper;

// Button pin
//const int buttonPin = 12; // Connect your button to digital pin 10
//int buttonState = 0; // Variable to store the button state

void setup() {
  // Set speed for the stepper motor
  stepper_telescopic.setSpeed(55); // Set Motor speed in RPM
  
  // Attach servo motor
  //gripper.attach(5); // Servo connected to pin 5
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set button pin as input
  //pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor

  // Initialize servo to closed position
  //gripper.write(50); // Closed position
}

void loop() {
  // Read the button state
  //buttonState = digitalRead(buttonPin);

  //if (buttonState == LOW) { // Button pressed (LOW if using INPUT_PULLUP)
  //delay(7000);
    Serial.println("Button pressed! Executing actions...");

    // Open servo to grip position
    //gripper.write(70); // Open to 60 degrees
    //delay(1000);       // Wait for servo to fully move
    delay(5000);
    // Move stepper motor forward
    stepper_telescopic.step(-5000); // Move forward 3000 steps
    delay(5000); // Ensure movement completion

    // Close servo to release position
    //gripper.write(55); // Return to 50 degrees
    //delay(1000);       // Wait for servo to fully move

    // Move stepper motor backward
    stepper_telescopic.step(-5000); // Move backward 3000 steps
    delay(1000); // Ensure movement completion

    // Small debounce delay to prevent re-triggering
    //delay(300);
  //}
}