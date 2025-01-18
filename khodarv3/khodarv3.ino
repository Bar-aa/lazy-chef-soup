#include <Keypad.h>
#include <Stepper.h> 

#define STEPS_PER_REV 200
const int stepsPerRevolution = 6400;

#define directionPinKhodar 22
#define stepPinKhodar 23

Stepper stepperAdas(STEPS_PER_REV, 41,40,39,38);
Stepper stepperShera(STEPS_PER_REV, 31,29,27,25);

#define trig1 30
#define echo1 31
#define trig2 32
#define echo2 33
#define trig3 34
#define echo3 35

const long threshold = 10; 
const int valvePin = 20;
const int GazePin = 21;
const int temperatureSensor = 19;

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

char message[] = "0000";
int keyPressCount=0;

long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; 
  return distance;
}

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
  if (key == '3') return STEPS_PER_REV / 5; 
  if (key == '6') return STEPS_PER_REV / 3; 
  if (key == '9') return STEPS_PER_REV / 2; 
  return 0;
}

void executeMotor(char motorKey, int steps) {
  int trigPin = 0, echoPin = 0;

  if (motorKey == 'A') {
    trigPin = trig1; echoPin = echo1;
  } else if (motorKey == 'B') {
    trigPin = trig2; echoPin = echo2;
  } else if (motorKey == 'C') {
    trigPin = trig3; echoPin = echo3;
  } else {
    return;
  }

  long distanceBefore = measureDistance(trigPin, echoPin);
  if (motorKey == 'A') {
    stepperAdas.step(steps); 
  } else if (motorKey == 'C') {
    moveStepper(directionPinKhodar, stepPinKhodar, steps); 
  } else if (motorKey == 'B') {
    stepperShera.step(steps); 
  }

  long distanceAfter = measureDistance(trigPin, echoPin);
  if (distanceAfter < threshold) {
  }

  delay(6000);
}

void storeKeyPress(char key) {
  if (keyPressCount < 4) {  
    message[keyPressCount] = key;
    keyPressCount++;
  }
}

// Calculate steps based on people count
int calculateSteps(char key) {
  if (key == '3') return stepsPerRevolution / 5; 
  if (key == '6') return stepsPerRevolution / 3; 
  if (key == '9') return stepsPerRevolution / 2; 
  return 0;
}

void controlWaterPump(char peopleCount) {
  int pumpTime = 0;
  digitalWrite(valvePin, LOW);  
  if (peopleCount == '3') {
    pumpTime = 3 * 1000; 
    delay(4000); 
  } else if (peopleCount == '6') {
    pumpTime = 6 * 1000; 
    delay(7000);
  } else if (peopleCount == '9') {
    pumpTime = 9 * 1000;
    delay(10000);
  }
  digitalWrite(valvePin, HIGH); 
  delay(6000);
}

void controlHeater(char soupType) {
  digitalWrite(GazePin, LOW);
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

void setup() {
  Serial.begin(9600);
  stepperAdas.setSpeed(20); 
  stepperShera.setSpeed(20);
  pinMode(directionPinKhodar, OUTPUT);
  pinMode(stepPinKhodar, OUTPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(GazePin, OUTPUT);
  pinMode(temperatureSensor, INPUT);
  digitalWrite(GazePin, HIGH);
  digitalWrite(valvePin, HIGH); 
  digitalWrite(temperatureSensor, HIGH); 
}

void sendKeyPresses() {
  if ((keyPressCount==3&&message[2]=='#')||(keyPressCount==4&&message[3]=='#')){
    for (int i = 0; i < keyPressCount; i++){
      char key = message[i];
      // Map specific keys to values
      if (i==0){
        if (key == 'A') message[i] = '0';  // Append '0' for A
        else if (key == 'B') message[i]= '1';  // Append '1' for B
        else if (key == 'C') message[i] = '2';  // Append '2' for C 
        else message[i] = '0';  // Append '0' for A
      }
      else if (i==1){
        if (key == '3') message[i]= '1';  
        else if (key == '6') message[i]= '2';  
        else if (key == '9') message[i]= '3';  
      }
      else if (i==2&&key!='#'){
        if (key == '2') message[i]= '0';
        else if (key == '5') message[i]= '1';  
        else if (key == '8') message[i]= '2';  
        else if (key == '0') message[i]= '3';   
      }
      else if (i==2&&key=='#'){
        message[i]= '9';
        message[i+1]= '#';
      }
    }
  }
  message[4]= '\0';
}

// Function to handle multiple character inputs
void handleCharacters(char characters[]) {
  if (strlen(characters) < 4 || characters[3] != '#') {
    Serial.println("Invalid input format.");
    return;
  }
  if (characters[3]=='#'){
    char soupType = message[0];
    char peopleCount = message[1];
    int steps = calculateSteps(peopleCount);
    int stepsmotor32 = Calcnema23(peopleCount);
    if (soupType=='C') executeMotor(soupType, steps); 
    else executeMotor(soupType, stepsmotor32);
    Serial.write(message);
    controlWaterPump(message[1]);
    controlHeater(soupType);
  }        
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    storeKeyPress(key); 
    if (key=='#'){
      sendKeyPresses();
      keyPressCount=0;
      handleCharacters(message);
      
    }
  }
  
}


