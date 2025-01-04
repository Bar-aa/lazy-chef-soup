// Steps per revolution (adjust based on your motor specs)
const int stepsPerRevolution = 200;

void setup() {
  // Set pins as output
  pinMode(22, OUTPUT); // STEP_PINMU
  pinMode(23, OUTPUT); // DIR_PINMU
  pinMode(24, OUTPUT); // EN_PINMU

  pinMode(26, OUTPUT); // STEP_PINMUP
  pinMode(27, OUTPUT); // DIR_PINMUP
  pinMode(25, OUTPUT); // EN_PINMUP

  // Enable the motor driver
  digitalWrite(24, LOW); // Enable motor 1
  digitalWrite(25, LOW); // Enable motor 2
}

void loop() {
  // Set direction to clockwise
  digitalWrite(23, HIGH); // DIR_PINMU
  digitalWrite(27, HIGH); // DIR_PINMUP
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(22, HIGH); // STEP_PINMU
    digitalWrite(26, HIGH); // STEP_PINMUP
    delayMicroseconds(1000); // Adjust speed by changing the delay
    digitalWrite(22, LOW);  // STEP_PINMU
    digitalWrite(26, LOW);  // STEP_PINMUP
    delayMicroseconds(1000);
  }
  delay(1000); // Wait 1 second

  // Set direction to counterclockwise
  digitalWrite(23, LOW); // DIR_PINMU
  digitalWrite(27, LOW); // DIR_PINMUP
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(22, HIGH); // STEP_PINMU
    digitalWrite(26, HIGH); // STEP_PINMUP
    delayMicroseconds(1000); // Adjust speed by changing the delay
    digitalWrite(22, LOW);  // STEP_PINMU
    digitalWrite(26, LOW);  // STEP_PINMUP
    delayMicroseconds(1000);
  }
  delay(1000); // Wait 1 second
}
