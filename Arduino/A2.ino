#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> // Include the Servo library

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 6
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myservo;  // Create Servo instance

String acceptableUID = "4C 57 52 49"; // Set the initial acceptable UID

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myservo.attach(SERVO_PIN); // Attach the servo to the specified pin
}

void loop()
{
  // Check if there is data available on the Serial
  if (Serial.available() > 0) {
    String incomingUID = Serial.readStringUntil('\n');
    incomingUID.trim();
    if (incomingUID.length() > 0) {
      acceptableUID = incomingUID;
      Serial.println("Updated UID: " + acceptableUID);
    }
  }

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // Show UID on serial monitor
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  if (content.substring(1) == acceptableUID) // Check if the UID matches the acceptable UID
  {
    Serial.println("Welcome Home");
    myservo.write(90); // Rotate the servo 90 degrees
    delay(5000);       // Wait for 5 seconds
    myservo.write(0);  // Rotate the servo back to its original position
    delay(3000);       // Wait for 3 seconds
  }
  else
  {
    Serial.println("Access denied");
    delay(3000);
  }
}
