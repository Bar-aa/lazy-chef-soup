#include <Servo.h>

Servo gripperServo; // Create a Servo object
int servoPin = 7;   // Pin connected to the servo

void setup() {
  Serial.begin(9600);         // Initialize serial communication
  gripperServo.attach(servoPin); // Attach the servo to the specified pin
  Serial.println("Gripper Test Started");
}

void loop() {
  // Open the gripper (60 degrees)
  Serial.println("Opening gripper...");
  gripperServo.write(60); // Set to 60° (open position)
  delay(2000); // Wait 1 second

  // Close the gripper (55 degrees) to hold the salt box
  Serial.println("Gripping the salt box...");
  gripperServo.write(50); // Set to 55° (gripping position)
  delay(5000); // Hold for 5 seconds

  // Release the gripper (back to 60 degrees)
  Serial.println("Releasing the salt box...");
  gripperServo.write(60); // Return to 60° (release position)
  delay(2000); // Wait 1 second before repeating
}
