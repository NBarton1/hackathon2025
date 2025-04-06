#include <Keypad.h>
#include <EEPROM.h>

const byte PASS_LEN = 4;
byte primes[] = { 2, 3, 5, 7 };

const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 22, 24, 26, 28 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 23, 25, 27, 29 };  //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int enter_password() {
  byte pass[PASS_LEN];
  int hash = 0;
  for (int i=0; i<PASS_LEN; i++) {
    byte key = keypad.getKey();
    while (key < '0' || key > '9')
      key = keypad.getKey();
    pass[i] = key - '0';
    hash += pass[i] * pass[i] * primes[i];
  }

  return hash;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Enter your passcode, then # to stop");

  int hash = enter_password();
  for (int i=0; i<2; i++)
    EEPROM.write(i, hash << (i << 3));
}

void loop() {
  Serial.println("Enter your password guess");

  int hashed_pass = 0;
  for (int i=0; i<2; i++)
    hashed_pass += EEPROM.read(i) << (i << 3);

  int hash = enter_password();
  if (hash == hashed_pass)
    Serial.println("DK won");
  else Serial.println("You're mid");

}