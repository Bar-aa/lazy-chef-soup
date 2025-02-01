// #include <SoftwareSerial.h>
// #include <DFRobotDFPlayerMini.h>

// //#define RELAY_PIN 7  // Pin for the relay module (use pin 7 since Uno has no pin 33)

// // Create SoftwareSerial for DFPlayer Mini
// SoftwareSerial mySerial(10, 11);  // RX = 10, TX = 11
// DFRobotDFPlayerMini myDFPlayer;

// void setup() {
   

//     Serial.begin(9600);
//     mySerial.begin(9600);  // DFPlayer Mini baud rate

//     Serial.println("Initializing DFPlayer Mini...");
    
//     if (!myDFPlayer.begin(mySerial)) {
//         Serial.println("DFPlayer Mini not detected or SD card missing.");
//         while (true);  // Halt if initialization fails
//     }

//     Serial.println("DFPlayer Mini initialized successfully!");
//     myDFPlayer.volume(14);  // Set volume (0-30)
// }

// void loop() {
//    Serial.println("Playing track 001...");
//     myDFPlayer.play(1);  // Play track 002.mp3
//     delay(5000);  
//     Serial.println("Playing track 002...");
//     myDFPlayer.play(2);  // Play track 002.mp3
//     delay(5000);  // Wait 5 seconds

//     Serial.println("Playing track 003...");
//     myDFPlayer.play(3);  // Play track 003.mp3
//     delay(36000);  // Wait 36 seconds

//     Serial.println("Restarting loop...");
// }
#include <DFRobotDFPlayerMini.h>

DFRobotDFPlayerMini myDFPlayer;

void setup() {
    Serial.begin(9600);
    Serial3.begin(9600);  // Use Serial3 instead of SoftwareSerial

    Serial.println("Initializing DFPlayer Mini...");

    if (!myDFPlayer.begin(Serial3)) {  // Use Serial3 instead of mySerial
        Serial.println("DFPlayer Mini not detected or SD card missing.");
        while (true);  // Halt if initialization fails
    }

    Serial.println("DFPlayer Mini initialized successfully!");
    myDFPlayer.volume(14);  // Set volume (0-30)
}

void loop() {
    Serial.println("Playing track 001...");
    myDFPlayer.play(1);
    delay(5000);

    Serial.println("Playing track 002...");
    myDFPlayer.play(2);
    delay(5000);

    Serial.println("Playing track 003...");
    myDFPlayer.play(3);
    delay(36000);
  Serial.println("Playing track 004...");
    myDFPlayer.play(4);
    delay(36000);
    Serial.println("Playing track 005...");
    myDFPlayer.play(5);
    delay(36000);
    Serial.println("Playing track 006...");
    myDFPlayer.play(6);
    delay(36000);
    Serial.println("Restarting loop...");
}
