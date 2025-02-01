
//#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <Wire.h>


// RGB LED Pins (PWM pins on Arduino Uno)
int redPin = A10;
int greenPin = A9;
int bluePin = A8;

// Function to set RGB LED color
void setRGBColor(int redVal, int greenVal, int blueVal) {
  // Invert values because it's a common anode RGB LED
  analogWrite(redPin, redVal);    // Set red intensity (inverted)
  analogWrite(greenPin, greenVal); // Set green intensity (inverted)
  analogWrite(bluePin, blueVal);   // Set blue intensity (inverted)
}

void setup() {
  // Initialize RGB LED pins as OUTPUT
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Set initial color to white (255, 255, 255 means all colors on)
  setRGBColor(255, 255, 255);  // White
}

void loop() {
  // Cycle through Red, Green, and Blue colors with 1-second delay
  setRGBColor(0, 0, 255);  // Red
  delay(5000);
  setRGBColor(0, 255, 0);  // Red
  delay(5000);
  setRGBColor(255, 0, 0);  // Red
  delay(5000);

 
}
