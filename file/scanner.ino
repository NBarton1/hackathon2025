#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 5
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
    Serial.begin(9600);
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522
}

void loop() {
    if (rfid.PICC_IsNewCardPresent()) Serial.println("DK won!");
    delay(10);
}
