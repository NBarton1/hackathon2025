#include <SPI.h>
#include <MFRC522.h>

#include <LiquidCrystal.h>

#define RS 8
#define ENABLE 9
#define D4 10
#define D5 11
#define D6 12
#define D7 13

LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);

const byte lock[8] = {
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};

const byte unlock[8] = {
  B01110,
  B10001,
  B10000,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};

const byte arrowLeft[8] = {
  B00001,
  B00011,
  B00111,
  B01111,
  B00111,
  B00011,
  B00001,
  B00000
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  lcd.begin(16, 2);
  lcd.noBlink();
  lcd.createChar(0, lock);
  lcd.createChar(1, unlock);
  lcd.clear();
}

void loop() {

  lcd.print("Enter Password:");
  delay(100);
  lcd.clear();
  // lcd.print("Access Denied");

  // lcd.setCursor(7, 1);
  // lcd.print(byte(0));

  // delay(1000);
  // lcd.setCursor(0, 0);
  // lcd.print("Access Granted");

  // lcd.setCursor(7, 1);
  // lcd.print(byte(1));

}
