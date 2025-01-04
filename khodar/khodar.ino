
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

// Stepper motor pins
#define directionPinShera 22
#define stepPinShera 23

#define directionPinKhodar 24
#define stepPinKhodar 25

#define directionPinAdas 26
#define stepPinAdas 27

const int GazePin = 21;
const int valvePin = 20;

const int stepsPerRevolution = 6400;  // 6400 steps for one full revolution

// Keypad setup
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Ultrasonic sensor pins
int trig1 = 47; // Trigger pin for ultrasonic sensor
int echo1 = 45; // Echo pin for ultrasonic sensor

int trig2 = 49; // Trigger pin for ultrasonic sensor
int echo2 = 51;

int trig3 = 16; // Trigger pin for ultrasonic sensor
int echo3 = 17;

const int threshold = 18; // Threshold distance in cm to detect empty box

// Variables to store the selected motor, mode, and number of steps
char selectedMotor = '\0';
bool dualMode = false; // Flag to indicate if 'D' was pressed
int stepsShera = 0;
int stepsKhodar = 0;

// Global variables to store the key presses in an array
char keyPresses[10];  // Array to store the last 10 key presses
int keyPressCount = 0;  // Counter to track how many keys are pressed

// Function prototypes
void executeMotor(char motorKey, int steps);
void storeKeyPress(char key);
void controlValve(int valvePin, int duration);
void checkAndControlValve(); // Function to check key presses and control valve based on the key
void controlGaze(int GazePin,  int duration );
void setup() {
  Serial.begin(9600);

  // Set motor pins as outputs
  pinMode(directionPinShera, OUTPUT);
  pinMode(stepPinShera, OUTPUT);

  pinMode(directionPinKhodar, OUTPUT);
  pinMode(stepPinKhodar, OUTPUT);

  pinMode(directionPinAdas, OUTPUT);
  pinMode(stepPinAdas, OUTPUT);

  // Set ultrasonic sensor pins
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(GazePin, OUTPUT);
  
  Serial.println("System Initialized.");
}

// Function to measure distance using ultrasonic sensor
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  // Wait for the signal to be LOW for a while
  digitalWrite(trigPin, HIGH);  // Send a pulse to trigger the sensor
  delayMicroseconds(10);  // Pulse duration must be at least 10 microseconds
  digitalWrite(trigPin, LOW);  // Turn off the trigger

  long duration = pulseIn(echoPin, HIGH);  // Measure the time for the echo pulse to return
  long distance = (duration / 2) * 0.0344;  // Convert duration to distance in cm
  return distance;
}

// Move stepper motor function
void moveStepper(int directionPin, int stepPin, int steps) {
  digitalWrite(directionPin, HIGH); // Set direction
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
  }
}

int calculateSteps(char key) {
  if (key == '3') return stepsPerRevolution / 5; // Steps for people 1–2
  if (key == '6') return (stepsPerRevolution / 3); // Steps for people 3–4
  if (key == '9') return (stepsPerRevolution / 2); // Steps for people 5–7
  return 0;
}

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

  moveStepper((motorKey == 'A' ? directionPinAdas : motorKey == 'B' ? directionPinKhodar : directionPinShera),
              (motorKey == 'A' ? stepPinAdas : motorKey == 'B' ? stepPinKhodar : stepPinShera), steps);

  long distanceAfter = measureDistance(trigPin, echoPin);
  Serial.print("Distance after: ");
  Serial.print(distanceAfter);
  Serial.println(" cm");

  if (distanceAfter < threshold) {
    Serial.println("Warning: Container might be empty!");
  }
}

void executeBothMotors(int stepsShera, int stepsKhodar) {
  // Activate both motors simultaneously
  Serial.println("Starting both Shera and Khodar motors...");
  
  // Start Shera motor
  moveStepper(directionPinAdas, stepPinAdas, stepsShera);
  
  // Start Khodar motor
  moveStepper(directionPinKhodar, stepPinKhodar, stepsKhodar);

  // Measure distances for both boxes
  long distanceShera = measureDistance(trig3, echo3);
  Serial.print("Shera box distance: ");
  Serial.print(distanceShera);
  Serial.println(" cm");
  
  if (distanceShera < threshold) {
    Serial.println("Warning: Shera box might be empty!");
  }

  long distanceKhodar = measureDistance(trig2, echo2);
  Serial.print("Khodar box distance: ");
  Serial.print(distanceKhodar);
  Serial.println(" cm");

  if (distanceKhodar < threshold) {
    Serial.println("Warning: Khodar box might be empty!");
  }
}
  
void storeKeyPress(char key) {
  if (keyPressCount < 10) {  // Store up to 10 key presses
    keyPresses[keyPressCount] = key;  // Store key at the current index
             ++;  // Increment the counter
    Serial.print("Key stored: ");
    Serial.println(key);
  } else {
    // Shift all elements to the left to make room for the new key
    for (int i = 1; i < 10; i++) {
      keyPresses[i - 1] = keyPresses[i];
    }
    keyPresses[9] = key;  // Store the new key at the end
    Serial.println("Key buffer full. Oldest key removed.");
  }
}

void controlValve(int valvePin, int duration) {
  digitalWrite(valvePin, LOW);  // Turn on the valve (active low)
  Serial.println("Valve is ON.");
  
  delay(duration);  // Wait for the specified duration in milliseconds
  
  digitalWrite(valvePin, HIGH);  // Turn off the valve (active low)
  Serial.println("Valve is OFF.");
}

void controlGaze(int GazePin, int duration) {
  digitalWrite(GazePin, LOW);  // Turn on the valve (active low)
  Serial.println("Gaze is ON.");
  
  delay(duration);  // Wait for the specified duration in milliseconds
  
  digitalWrite(GazePin, HIGH);  // Turn off the valve (active low)
  Serial.println("Gaze is OFF.");
}
// Function to check the stored key presses and control the valve accordingly
void checkAndControlValve() {
  for (int i = 0; i < keyPressCount; i++) {
    if (keyPresses[i] == '3') {
      controlValve(valvePin, 3000); // Turn valve on for 3 seconds
    } else if (keyPresses[i] == '6') {
      controlValve(valvePin, 6000); // Turn valve on for 6 seconds
    } else if (keyPresses[i] == '9') {
      controlValve(valvePin, 9000); // Turn valve on for 9 seconds
    }
  }
}

void checkAndControlGaze() {
  for (int i = 0; i < keyPressCount; i++) {
    if (keyPresses[i] == 'A') {
      controlGaze(GazePin,30*60 *1000);  // 30 min Adas 
    } else if (keyPresses[i] == 'B') {
      controlGaze(GazePin,40*60 *1000); //  Khodare  40 min 
    } else if (keyPresses[i] == 'C') {
      controlGaze(GazePin,20*60*1000);  //shera  20 min 
    }
    else if (keyPresses[i] == 'D') {
      controlGaze(GazePin,20*60*1000); 
    }
  }
}

void initialCheckAndControl() {
  // First, check and control the valve
  checkAndControlValve();
  // Delay for 2 minutes (120000 milliseconds)
 delay(120000);
  // Then, check and control the gaze
  checkAndControlGaze();
}
char receiveCharFromOtherArduino() {
  if (Serial.available() > 0) {  // Check if data is available to read
    char receivedChar = Serial.read();  // Read the incoming character
    return receivedChar;  // Return the received character
  }
  return '\0';  // Return null character if no data is available
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Key pressed: ");
    Serial.println(key);

    // Store the key in the array
    storeKeyPress(key);
  /*char receivedChar = receiveCharFromOtherArduino();

  if (receivedChar != '\0'&& receivedChar =='v') { 
    Serial.print("Received: ");
    Serial.println(receivedChar);
    initialCheckAndControl();
  }*/
    // Check and control the valve based on stored key presses
    //checkAndControlValve();
     
    initialCheckAndControl();

    // Handle the keys for motor selection, steps, and actions
    if (key == 'A' || key == 'B' || key == 'C') {
      selectedMotor = key;
      Serial.print("Motor selected: ");
      Serial.println(selectedMotor);
    } else if (key == '3' || key == '6' || key == '9') {
      // Calculate steps for the motor and store for both motors if in dual mode
      int steps = calculateSteps(key);
      if (dualMode) {
        stepsShera = steps;
        stepsKhodar = steps;
        Serial.print("Steps set for both motors: ");
        Serial.println(steps);
      } else {
        stepsShera = steps;
        stepsKhodar = 0;
        Serial.print("Steps set for Shera motor: ");
        Serial.println(steps);
      }
    } else if (key == '#') {
      if (dualMode) {
        // Execute both motors if in dual mode
        Serial.println("Executing both Shera and Khodar motors simultaneously...");
        executeBothMotors(stepsShera, stepsKhodar);
        dualMode = false; // Reset dualMode flag after executing
        stepsShera = 0;
        stepsKhodar = 0;
      } else if (selectedMotor != '\0' && stepsShera > 0) {
        // Execute selected motor if not in dual mode
        Serial.println("Executing selected motor movement...");
        executeMotor(selectedMotor, stepsShera);
        selectedMotor = '\0';
        stepsShera = 0;
      } else {
        Serial.println("Please select a motor and set steps before pressing #.");
      }
    } else if (key == 'D') {
      // Set dualMode flag to true when D is pressed
      dualMode = true;
      Serial.println("Dual mode enabled. Select steps (3, 6, 9) and press #.");
    } else {
      Serial.println("Invalid key pressed. Use A, B, C, 3, 6, 9, or #.");
    }
  }
}