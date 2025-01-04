#include <Stepper.h>
#include <Servo.h>

// Define steps per revolution for the motors
#define STEPS_PER_REV 200

// Define the distance between shelves and gripper in steps
#define DISTANCE_BETWEEN_SHELVES_AND_GRIPPER 12000

Stepper stepperTelescopic(STEPS_PER_REV, 38, 39, 40, 41);
Stepper stepperLeftElevated(STEPS_PER_REV, 44, 45, 46, 47);
Stepper stepperLeftGround(STEPS_PER_REV, 50, 51, 52, 53);



// Button pin
const int buttonPin = 12;



int itisup=0;

void setup() {
  // Initialize motor speeds
  stepperTelescopic.setSpeed(40);
  stepperLeftElevated.setSpeed(40);
  stepperLeftGround.setSpeed(40);



  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(buttonPin) == LOW){
    /*stepperLeftGround.step(-1450);
    delay (5000);
    stepperLeftElevated.step(-900);
    delay (5000);
    stepperLeftElevated.step(900);
    stepperLeftGround.step(1450);*/
    //mixSoup();
  }
}

void mixSoup(){
  moveBothMotors(shelfPositions[1]);
  stepperLeftElevated.step(spiceBoxPositionslower[1]);
  startgripperingSequence();
  stepperLeftElevated.step(-spiceBoxPositionslower[1]);
  stepperLeftGround.step(1200);
  stepperLeftElevated.step(spiceBoxPositionslower[1]);


  //stepperTelescopic.step(4000);
  // stepperLeftElevated.step(-300);
  // stepperLeftElevated.step(600);
  // stepperLeftElevated.step(-600);
  // stepperLeftElevated.step(-300);
  // stepperTelescopic.step(3000);
  // stepperTelescopic.step(-6000);
  /*for (int i =0;i<4;i++){
    stepperLeftElevated.step(-300);
    stepperLeftElevated.step(600);
    stepperLeftElevated.step(-300);
    stepperTelescopic.step(6000);
    stepperTelescopic.step(+6000);
  }*/

}