#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"



// RGB LED Pins (PWM pins on Arduino Uno)
int redPin = 3;
int greenPin = 5;
int bluePin = 6;

// Function to set RGB LED color
void setRGBColor(int redVal, int greenVal, int blueVal) {
  // Invert values because it's a common anode RGB LED
  analogWrite(redPin, 255 - redVal);    // Set red intensity (inverted)
  analogWrite(greenPin, 255 - greenVal); // Set green intensity (inverted)
  analogWrite(bluePin, 255 - blueVal);   // Set blue intensity (inverted)
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
  setRGBColor(255, 0, 0);  // Red
  delay(1000);

  setRGBColor(0, 255, 0);  // Green
  delay(1000);

  setRGBColor(0, 0, 255);  // Blue
  delay(1000);

  setRGBColor(255, 255, 255);  // White
  delay(1000);
}
