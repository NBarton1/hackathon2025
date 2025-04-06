#include <string.h>
#include <MFRC522.h>
#include <SPI.h>
#include <EEPROM.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

#define LCD_RS 8
#define LCD_ENABLE 9
#define LCD_D4 10
#define LCD_D5 11
#define LCD_D6 12
#define LCD_D7 13

// #define SET_INITIAL_PASS
#define SS_PIN 53
#define RST_PIN 21

#define LOGOUT 'D'
#define SET_PASS '#'

#define PASS_LEN 4
#define UID_LEN 4

#define LOCK_CHAR 0
#define UNLOCK_CHAR 1
#define LEFT_ARROW_CHAR 2

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

const byte leftArrow[8] = {
  B00001,
  B00011,
  B00111,
  B01111,
  B00111,
  B00011,
  B00001,
  B00000
};

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

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
MFRC522 rfid(SS_PIN, RST_PIN);

byte admin_uid[UID_LEN] = {0xD9, 0xAE, 0xD6, 0x00};

enum session_state {
  LOGGED_OUT,
  USER_LOGGED_IN,
  ADMIN_LOGGED_IN,
};

session_state state = LOGGED_OUT;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  SPI.begin();
  rfid.PCD_Init();

  lcd.begin(16, 2);
  lcd.noBlink();
  lcd.createChar(LOCK_CHAR, lock);
  lcd.createChar(UNLOCK_CHAR, unlock);
  lcd.createChar(LEFT_ARROW_CHAR, leftArrow);
  lcd.clear();

#ifdef SET_INITIAL_PASS
    lcd_clear_print("Set Initial");
    lcd.setCursor(0, 1);
    lcd.print("Password");
    int hash = enter_password();
    for (int i=0; i<2; i++)
      EEPROM.write(i, hash >> (i << 3));
#endif
}

void read_card() {
  if (rfid.PICC_ReadCardSerial() && strncmp(rfid.uid.uidByte, admin_uid, UID_LEN) == 0) state = ADMIN_LOGGED_IN;
}

int get_password_hash() {
  int hashed_pass = 0;
  for (int i=0; i<2; i++)
    hashed_pass |= EEPROM.read(i) << (i << 3);
  return hashed_pass;
}

int enter_password() {
  byte pass[PASS_LEN];
  int hash = 0;
  for (int i=0; i<PASS_LEN; i++) {
    byte key = keypad.getKey();
    while (key < '0' || key > '9')
      key = keypad.getKey();
    lcd.print("*");
    pass[i] = key - '0';
    hash += pass[i] * pass[i] * primes[i];
  }

  return hash;
}

void attempt_login() {
  if (enter_password() == get_password_hash()) state = USER_LOGGED_IN;
}

void set_password() {
  Serial.println("Setting password");
  if (state != ADMIN_LOGGED_IN) {
    Serial.println("You're too mid to change the password");
    return;
  }

  int hash = enter_password();
  for (int i=0; i<2; i++)
    EEPROM.write(i, hash >> (i << 3));

  Serial.println("Password changed");
}

void login_mode() {

  if (state == ADMIN_LOGGED_IN) {
    lcd_clear_print("Welcome, Admin");
  } else {
    lcd_clear_print("Welcome, User");
  }

  char key = '\0';

  while (key != LOGOUT) {
    key = keypad.getKey();
    switch (key) {
      case SET_PASS:
        set_password();
        break;

    }
  }
}

void lcd_clear_print(char* printed) {
  lcd.clear();
  lcd.print(printed);
}

void loop() {

  state = LOGGED_OUT;
  char key = '\0';
  bool scanned = false;

  lcd_clear_print("Enter Password: ");
  lcd.setCursor(6, 1);

  while (!scanned && key != '*') {
    key = keypad.getKey();
    scanned = rfid.PICC_IsNewCardPresent();
  }

  if (scanned) {
    Serial.println("Scanned Card");
    read_card();
  } else {
    Serial.println("Loggin in");
    attempt_login();
    Serial.println(get_password_hash());
  }

  if (state != LOGGED_OUT) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Access Granted");
    lcd.setCursor(7, 1);
    lcd.write(byte(UNLOCK_CHAR));
    delay(1500);
    login_mode();
    lcd_clear_print("Goodbye");
  } else {
    lcd_clear_print("Access Denied");
    lcd.setCursor(7, 1);
    lcd.write(byte(LOCK_CHAR));
  }

  delay(2000);
}
