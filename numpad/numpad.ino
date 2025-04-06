#include <Keypad.h>
#include <EEPROM.h>

const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
char keys[ROWS][COLS] = {
  { '1', '2', '3', ' ' },
  { '4', '5', '6', ' ' },
  { '7', '8', '9', ' ' },
  { ' ', '0', ' ', ' ' }
};

byte rowPins[ROWS] = { 22, 24, 26, 28 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 23, 25, 27, 29 };  //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  int pass = 0;
  for (int i = 0; i < 4; i++) {
    byte key = keypad.getKey();
    while (!key || key == ' ')
      key = keypad.getKey();
    pass = pass * 10 + key - '0';
  }
  
  int hash = pass & 1023;
  for (int i = 0; hash; hash >>= 8)
    EEPROM.write(i++, hash & 0xFF);
}

void loop() {
  char key = keypad.getKey();
  if (key)
    Serial.println(key);
}