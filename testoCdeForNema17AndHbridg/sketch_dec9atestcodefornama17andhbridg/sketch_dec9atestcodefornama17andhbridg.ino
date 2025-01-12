#include <Stepper.h>

// Define the number of steps per revolution for your motor
#define STEPS_PER_REV 200


// Initialize the Stepper library on pins 8, 9, 10, and 11
Stepper stepper(STEPS_PER_REV, 8,9,10,11);

void setup() {
  // Set the speed of the stepper motor (in RPM)
  stepper.setSpeed(30); // 60 RPM
}

void loop() {
  delay (3000);
  // Step forward
  stepper.step(-500);
  // Move one revolution forward
  delay(1000); 
  stepper.step(500);
  // Move one revolution forward
  delay(1000);                 // Wait for a second

  // Step backward
  //stepper.step(-2000); // Move one revolution backward
  //delay(1000);                 // Wait for a second
}
