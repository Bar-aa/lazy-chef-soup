#include <Keypad.h>
#include <Stepper.h>  // Add this line for the Stepper library

// Constants for stepper motors
#define directionPinShera 22
#define stepPinShera 23
#define directionPinKhodar 24
#define stepPinKhodar 25
//#define directionPinAdas 26
//#define stepPinAdas 27
#define STEPS_PER_REV 200
const int stepsPerRevolution = 6400;

// Initialize the Stepper library on pins 8, 9, 10, and 11

// Constants for ultrasonic sensors
#define trig1 30
#define echo1 31
#define trig2 32
#define echo2 33
#define trig3 34
#define echo3 35

const long threshold = 10; // Distance threshold in cm

// Constants for valve, gaze (heater), and temperature sensor
const int valvePin = 20;
const int GazePin = 21;
const int temperatureSensor = 19;

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Variables for keypress handling
char keyPresses[4] = {'\0', '\0', '\0', '\0'};
int keyPressCount = 0;

// Timer variables
unsigned long lastTime = 0;
unsigned long heaterStartTime = 0;
bool isHeaterOn = false;
bool isPumpOn = false;

// Function Prototypes
void storeKeyPress(char key);
int calculateSteps(char key);
long measureDistance(int trigPin, int echoPin);
void moveStepper(int directionPin, int stepPin, int steps);
void executeMotor(char motorKey, int steps);
void controlWaterPump(int peopleCount);
void controlHeater(char soupType);
void checkTemperature();
int Calcnema23(char key);
// Initialize stepper motor for Adas
Stepper stepperAdas(STEPS_PER_REV, 41,40,39,38);
Stepper stepperkhodar(STEPS_PER_REV, 31,29,27,25);
// Measure distance using ultrasonic sensor
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

// Move stepper motor for a specific number of steps
void moveStepper(int directionPin, int stepPin, int steps) {
  digitalWrite(directionPin, HIGH);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
  }
}
int Calcnema23(char key) {
  if (key == '3') return STEPS_PER_REV / 5; // Steps for people 1–2
  if (key == '6') return STEPS_PER_REV / 3; // Steps for people 3–4
  if (key == '9') return STEPS_PER_REV / 2; // Steps for people 5–7
  return 0; // Return 0 if no matching condition
}


 
// Execute motor operation with distance measurement
void executeMotor(char motorKey, int steps) {
  int trigPin = 0, echoPin = 0;

  if (motorKey == 'A') {
    Serial.println("Adas motor selected.");
    trigPin = trig1; echoPin = echo1;
  } else if (motorKey == 'B') {
    Serial.println("Khodar motor selected.");
    trigPin = trig2; echoPin = echo2;
  } else if (motorKey == 'C') {
    Serial.println("Shera motor selected.");
    trigPin = trig3; echoPin = echo3;
  } else {
    Serial.println("Invalid motor key.");
    return;
  }

  long distanceBefore = measureDistance(trigPin, echoPin);
  Serial.print("Distance before: ");
  Serial.print(distanceBefore);
  Serial.println(" cm");

  if (motorKey == 'A') {
    stepperAdas.step(steps); // Call the step function for motor A
  } else if (motorKey == 'C') {
    moveStepper(directionPinKhodar, stepPinKhodar, steps); // Call the step function for motor B
  } else if (motorKey == 'B') {
    stepperkhodar.step(steps); // Call the step function for motor C
  }

  long distanceAfter = measureDistance(trigPin, echoPin);
  Serial.print("Distance after: ");
  Serial.print(distanceAfter);
  Serial.println(" cm");

  if (distanceAfter < threshold) {
    Serial.println("Warning: Container might be empty!");
  }

  // Control the water pump based on the number of people
  delay(6000);
}

// Store key press in a circular buffer
void storeKeyPress(char key) {
  if (keyPressCount < 4) {  // Store up to 4 key presses
    keyPresses[keyPressCount] = key;
    keyPressCount++;
  } else {
    for (int i = 1; i < 4; i++) {
      keyPresses[i - 1] = keyPresses[i];
    }
    keyPresses[3] = key;
  }
}

// Calculate steps based on people count
int calculateSteps(char key) {
  if (key == '3') return stepsPerRevolution / 5; // Steps for people 1–2
  if (key == '6') return stepsPerRevolution / 3; // Steps for people 3–4
  if (key == '9') return stepsPerRevolution / 2; // Steps for people 5–7
  return 0;
}

// Control the water pump (valve) based on the number of people
void controlWaterPump(char peopleCount) {
  int pumpTime = 0;
  Serial.println("Water pump is ON.");
  digitalWrite(valvePin, LOW);  // Turn on the water pump
  if (peopleCount == '3') {
    pumpTime = 3 * 1000; // 9 seconds for 9 people
    delay(4000); //3000
  } else if (peopleCount == '6') {
    pumpTime = 6 * 1000; // 6 seconds for 6 people
    delay(7000);//6000
  } else if (peopleCount == '9') {
    pumpTime = 9 * 1000; // 9 seconds for 9 people
    delay(10000);//9000
  }
  digitalWrite(valvePin, HIGH); // Turn off the water pump
  Serial.println("Water pump is OFF.");
  Serial.println(" controlWaterPump Done");
  delay(6000);
}

// Control the heater based on the soup type
void controlHeater(char soupType) {
  digitalWrite(GazePin, LOW); // Turn on the heater
  if (soupType == 'A') {
    delay(2100000);
  } else if (soupType == 'B') {
    delay(120000);
  } else if (soupType == 'C') {
    delay(180000);
  }
  digitalWrite(GazePin, HIGH);
  delay(6000);
}

// Check temperature sensor and interrupt if oven is ready
void checkTemperature() {
  // Assuming a digital temperature sensor where HIGH means ready
  int temperatureValue = digitalRead(temperatureSensor);
  
  if (temperatureValue == HIGH) {
    Serial.println("Oven is ready!");
    // No action for heater here; it remains on
  }
}

// Setup function
void setup() {
  Serial.begin(9600);
  stepperAdas.setSpeed(20); 
  stepperkhodar.setSpeed(20);
  // Set motor pins as outputs
  pinMode(directionPinShera, OUTPUT);
  pinMode(stepPinShera, OUTPUT);
  pinMode(directionPinKhodar, OUTPUT);
  pinMode(stepPinKhodar, OUTPUT);
  // pinMode(directionPinAdas, OUTPUT);
  // pinMode(stepPinAdas, OUTPUT);

  // Set ultrasonic sensor pins
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);

  // Set valve and gaze (heater) pins
  pinMode(valvePin, OUTPUT);
  pinMode(GazePin, OUTPUT);

  // Set temperature sensor pin
  pinMode(temperatureSensor, INPUT);

  digitalWrite(GazePin, HIGH); // Initially turn off the heater
  digitalWrite(valvePin, HIGH); // Initially turn off the water pump
  digitalWrite(temperatureSensor, HIGH); // Initially turn on the temperatureSensor


  Serial.println("System Initialized.");
}


String key = "";
String time ="";
String numberpeople = "";
String done = "";
String Spice = "";
int inputState = 0; // 0: Waiting for soupType, 1: Waiting for numberpeople, 2: Waiting for Done
//int keyPressCount = 0; // To count key presses
String timeinput="";
String cnc="";
   
void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read input as String
    input.trim(); // Remove leading/trailing whitespace
    Serial.println(input);
    cnc=input.substring(0, 4);
    timeinput = input.substring(4);
    Serial.println(cnc);
    Serial.println(timeinput);


for (int i=0;i<4;i++){  
    switch (inputState) {
      case 0: // Waiting for soupType (A)
        if (input[inputState]=='A' || input[inputState]=='B' || input[inputState]=='C') {
          key = input[inputState];
          inputState = 1; // Move to the next state
          Serial.println("Soup type received: " + key);
        } else {
          Serial.println("Invalid soup type. Please enter A, B, or C.");
        }
        break;

      case 1: // Waiting for numberpeople (3, 6, 9)
        if (input[inputState]=='3' ||input[inputState]=='6' || input[inputState]=='9') {
          numberpeople = input[inputState];
          inputState = 2; // Move to the next state
          Serial.println("Number of people received: " + numberpeople);
        } else {
          Serial.println("Invalid number of people. Please enter 3, 6, or 9.");
        }
        break;

      case 2: // Waiting for Spice input (2, 5, 8, 0, 9)
        if (input[inputState]=='2' || input[inputState]=='5' || input[inputState]=='8' || input[inputState]=='0' || input[inputState]=='9') {
          Spice = input[inputState];
          inputState = 3; // Move to the next state
          Serial.println("Spice level received: " + Spice);
        } else {
          Serial.println("Invalid spice level. Please enter 2, 5, 8, 0, or 9.");
        }
        break;
       
      case 3: // Waiting for Done signal (#)
        if (input[inputState]=='#') { // Check if # is pressed after 3 or 4 key presses
          done = input;
          Serial.println("Received Done signal: " + done);
          // All inputs are valid; execute the motor operation
          char soupType = key.charAt(0);
          int stepsmotor32 = Calcnema23(numberpeople.charAt(0));
          executeMotor(soupType, stepsmotor32);
          // controlWaterPump(numberpeople.charAt(0));
          // controlHeater(soupType);
          inputState = 0;
          key = "";
          numberpeople = "";
          done = "";
          keyPressCount = 0; // Reset the key press count
          Serial.println("Sequence completed. Ready for the next input.");
        } else {
          Serial.println("Invalid Done signal. Please enter # after 3 or 4 key presses.");
        }
        break;
    }
}
  }


    //  }
    
  // Handle serial input
  

  // Handle keypad input
  char key = keypad.getKey();

  if (key) {
    Serial.print("Key pressed: ");
    Serial.println(key);

    storeKeyPress(key); // Store the key in the buffer
    if ((keyPressCount == 4 || keyPressCount == 3) && (keyPresses[3] == '#' || keyPresses[2] == '#')) {
      char soupType = keyPresses[0];
      char peopleCount = keyPresses[1];
      int steps = calculateSteps(peopleCount);
      int stepsmotor32 = Calcnema23(peopleCount);

      Serial.print("Steps to move: ");
      Serial.println(steps);

      if (soupType == 'C') {
        executeMotor(soupType, steps); // Execute motor operation
      } else {
        executeMotor(soupType, stepsmotor32);
      }

     // controlWaterPump(keyPresses[1]);
     // controlHeater(soupType); // Control the heater based on soup type
      keyPressCount = 0; // Reset the buffer
    }
  }
}

