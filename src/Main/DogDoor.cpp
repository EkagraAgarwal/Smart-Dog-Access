#include "DogDoor.h"
#include <SPI.h>
#include <MFRC522.h>

// RFID reader module pins
#define RFID_SS_PIN 10
#define RFID_RST_PIN 9

// Door control pins
#define DOOR_OPEN_PIN 5
#define DOOR_CLOSE_PIN 6

// RFID tag database (replace with your own dog collar IDs)
byte authorizedTags[][6] = {
  {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB}, // Dog 1
  {0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67}  // Dog 2
};
int numAuthorizedTags = 2;

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN); // Create MFRC522 instance

void DogDoorSetup() {
  pinMode(DOOR_OPEN_PIN, OUTPUT);
  pinMode(DOOR_CLOSE_PIN, OUTPUT);

  Serial.begin(9600);
  SPI.begin();        // Initialize SPI communication
  rfid.PCD_Init();    // Initialize RFID reader module
  delay(100);         // Allow time for initialization
  rfid.PCD_DumpVersionToSerial();  // Print MFRC522 library version to serial monitor
  Serial.println("Dog door system ready!");
}

void DogDoorLoop() {
  // Check for a new RFID tag
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // Verify if the tag is authorized
    if (IsTagAuthorized(rfid.uid.uidByte, rfid.uid.size)) {
      Serial.println("Access granted!");
      OpenDoor();
    } else {
      Serial.println("Access denied!");
    }
    rfid.PICC_HaltA();  // Halt the tag
    rfid.PCD_StopCrypto1(); // Stop encryption on the tag
  }
}

bool IsTagAuthorized(byte* tagData, byte dataSize) {
  // Compare the received tag with authorized tags
  for (int i = 0; i < numAuthorizedTags; i++) {
    if (memcmp(tagData, authorizedTags[i], dataSize) == 0) {
      return true;
    }
  }
  return false;
}

void OpenDoor() {
  digitalWrite(DOOR_OPEN_PIN, HIGH);
  delay(1000);  // Adjust the delay to control the door opening time
  digitalWrite(DOOR_OPEN_PIN, LOW);
}
