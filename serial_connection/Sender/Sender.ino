#include <Keypad.h>
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

char message[] = "0000";
int keyPressCount=0;
void setup() {
  Serial.begin(9600);
  // Write a message as bytes
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    //Serial.println(key);
    storeKeyPress(key);
    if (key=='#'){
      sendKeyPresses();
      Serial.write(message);
      keyPressCount=0;
    }
  }
  delay(500);
}
void storeKeyPress(char key) {
  if (keyPressCount < 4) {  
    message[keyPressCount] = key;
    keyPressCount++;
  }
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

