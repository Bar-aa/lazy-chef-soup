#include <Keypad.h>
#include <Stepper.h> 
#include <Wire.h>
#include <RTClib.h>
#include <string.h> // Required for strcpy
#include <SoftwareSerial.h>
#include <Wire.h>
#include "max6675.h"

RTC_DS3231 rtc;

#define STEPS_PER_REV 200
const int stepsPerRevolution = 6400;

#define directionPinKhodar 22
#define stepPinKhodar 23

Stepper stepperAdas(STEPS_PER_REV, 41,40,39,38);
Stepper stepperShera(STEPS_PER_REV, 31,29,27,25);

#define trig1 47
#define echo1 45
#define trig2 17
#define echo2 16
#define trig3 49
#define echo3 51


int ktcSO = 12;
int ktcCS = 11;
int ktcCLK = 10;
const int oven = 13;
MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

int LDR_PIN = A0;
int laserMin = 0.5;
int laserMax = 570;


const long threshold = 10; 
const int WATER_THRESHOLD = 30;     

#define valvePin 53
#define GazePin  52
volatile bool timerExpired = false;
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
char arduino1[] = "0000";
String TimeBuffer = ""; // To hold serial input
String cnc="";
int keyPressCount=0;
int year, month, day, hour, minute, second;
static bool isHandled =true;

// RGB LED Pins (PWM pins on Arduino Uno)
int redPin = 31;
int greenPin = 35;
int bluePin = 33;

// Function to set RGB LED color
void setRGBColor(int redVal, int greenVal, int blueVal) {
  // Invert values because it's a common anode RGB LED
  analogWrite(redPin, redVal);    // Set red intensity (inverted)
  analogWrite(greenPin, greenVal); // Set green intensity (inverted)
  analogWrite(bluePin, blueVal);   // Set blue intensity (inverted)
}

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


String measureAll() {
  int trigPins[] = {trig1, trig2, trig3};
  int echoPins[] = {echo1, echo2, echo3};
  String result = "";
  float conversionFactor = 50.0; 
  for (int i = 0; i < 3; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
    long distance = measureDistance(trigPins[i], echoPins[i]);
    long weight = distance * conversionFactor;
    result += String(weight);
    result += ",";
  }
  long waterDistance=measureDistance(trig2, echo2);
  float waterInLiters = 3.14159 * (24/2.0) * (24/2.0) * (30-waterDistance) * 0.001;
  result += String(waterInLiters,2);
  result += ",";
  result += String(ktc.readCelsius());
  return result;
}

bool potExist() {
  int lightValue = analogRead(LDR_PIN);
  return lightValue >= laserMin && lightValue <= laserMax;
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

// Calculate steps based on people count
int calculateSteps(char key) {
  if (key == '3') return stepsPerRevolution / 5; 
  if (key == '6') return stepsPerRevolution / 3; 
  if (key == '9') return stepsPerRevolution / 2; 
  return 0;
}

bool isDistanceBelowThreshold(int trigPin, int echoPin) {
  long distance = measureDistance(trigPin, echoPin);
  return distance < threshold;
}

bool isWaterLevelSufficient(int trigPin, int echoPin) {
  long distance = measureDistance(trigPin, echoPin);
  return distance < WATER_THRESHOLD;
}

bool executeMotor(char motorKey, int steps) {
  int trigPin = 0, echoPin = 0;
  if (motorKey == 'A') {trigPin = trig1; echoPin = echo1;}
  else if (motorKey == 'B') {trigPin = trig2; echoPin = echo2;}
  else if (motorKey == 'C') {trigPin = trig3; echoPin = echo3;}
  else {trigPin = trig3; echoPin = echo3;}

  if (isDistanceBelowThreshold(trigPin, echoPin)) {
    if (isWaterLevelSufficient(trig2, echo2)) {
      setRGBColor(0, 255, 0);
      if (motorKey == 'A') {stepperAdas.step(steps);} 
      else if (motorKey == 'C') {moveStepper(directionPinKhodar, stepPinKhodar, steps); }
      else if (motorKey == 'B') { stepperShera.step(steps);}
      delay(2000);
      return true; 
    } else {
      Serial1.write("Water level is insufficient\n");
    }
  } else {
    Serial1.write("The type of soup is not sufficient\n");
  }
  setRGBColor(255, 0, 0);
  return false;
}

void storeKeyPress(char key) {
  if (keyPressCount < 4) {  
    message[keyPressCount] = key;
    arduino1[keyPressCount]=key;
    keyPressCount++;
  }
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
  unsigned long timeToWait = 0;
  if (soupType == 'A') {
    timeToWait = 40*60*1000;  
  } else if (soupType == 'B') {
    timeToWait = 60*60*1000;   
  } else if (soupType == 'C') {
    timeToWait = 35*60*1000;   // 3 minutes in milliseconds
  }
  rtc.setAlarm1(rtc.now() + TimeSpan(timeToWait / 1000), DS3231_A1_Hour);
}
void rtcInterruptHandler() {
  timerExpired = true;  
}

void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);
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
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
  setRGBColor(255, 255, 255);  // White

  Wire.begin();
  attachInterrupt(digitalPinToInterrupt(2), rtcInterruptHandler, FALLING);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC. Check your connections.");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the default time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
  }
 
}

void sendKeyPresses() {
  //if ((keyPressCount==3&&message[2]=='#')||(keyPressCount==4&&message[3]=='#')){
    for (int i = 0; i < 4; i++){
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
  //}
  message[4]= '\0';
}

// Function to handle multiple character inputs
void handleCharacters(char characters[]) {
  if (strlen(characters) < 4 || characters[3] != '#') {
    //Serial.println("Invalid input format.");
    return;
  }
  if (characters[3]=='#'){
    char soupType = characters[0];
    char peopleCount = characters[1];
    int steps = calculateSteps(peopleCount);
    int stepsMotor32 = Calcnema23(peopleCount);
    
    bool motorExecuted;
    bool pot=potExist();
    if (soupType == 'C') {motorExecuted = executeMotor(soupType, steps);} 
    else { motorExecuted = executeMotor(soupType, stepsMotor32);}

    if (motorExecuted&&pot) {
      Serial.write(message); // Send success message
      controlWaterPump(peopleCount); // Manage water pump based on people count
      controlHeater(soupType); // Control heater based on soup type
      while (!timerExpired) {
        delay(100);
      }
      digitalWrite(GazePin, HIGH);
      delay(6000);  
      timerExpired = false;   
    }
  }        
}
void processInput(String input) {
  // Parse the input string
  if (sscanf(input.c_str(), "%d:%d", &hour, &minute) == 2) {
    //Serial.println("Time input parsed successfully.");
  } else {
    Serial.println("Invalid format. Use: HH:MM");
  }
}
 void handelInput(String input){
  if (input=="R"){
    String distances = measureAll();
    Serial1.print(distances);
  }else {
    cnc=input.substring(0, 4);
    TimeBuffer = input.substring(4);
    cnc.toCharArray(message, sizeof(message));
    strcpy(arduino1, message);
    processInput(TimeBuffer); // Process the time input
    sendKeyPresses(); 
    isHandled = false; // To avoid multiple calls
  }
}
void loop() {
  setRGBColor(0, 0, 255);  // blue
  char key = keypad.getKey();
  if (key) {
    storeKeyPress(key); 
    if (key=='#'){
      setRGBColor(255, 165, 0);
      sendKeyPresses();
      keyPressCount=0;
      handleCharacters(arduino1);
    }
  }
 
  if (Serial1.available() > 0) {
    setRGBColor(255, 165, 0);
    String input = Serial1.readStringUntil('\n'); // Read input as String
    input.trim(); // Remove leading/trailing whitespace
    handelInput(input);
  }  
  DateTime now = rtc.now();
  if (!isHandled && TimeBuffer != "") {
      if (now.hour() == hour && now.minute() == minute) {
        handleCharacters(arduino1);
        isHandled = true; 
      }
  } 
}


