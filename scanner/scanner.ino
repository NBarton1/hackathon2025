#include <MFRC522.h>
#include <SPI.h>
#include <EEPROM.h>
#include <Keypad.h>

#define SS_PIN 53
#define RST_PIN 5

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

const char LOGOUT_KEY = 'D';

MFRC522 rfid(SS_PIN, RST_PIN);

const size_t UID_LEN = 4;
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

  // int hash = enter_password();
  // for (int i=0; i<2; i++)
  //   EEPROM.write(i, hash << (i << 3));
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
    pass[i] = key - '0';
    hash += pass[i] * pass[i] * primes[i];
  }

  return hash;
}

void attempt_login() {
  if (enter_password() == get_password_hash()) state = USER_LOGGED_IN;
}

void set_password() {
  if (state != ADMIN_LOGGED_IN) return;

  int hash = enter_password();
  for (int i=0; i<2; i++)
    EEPROM.write(i, hash << (i << 3));
}

// void login_mode() {
//   if (key == '#') {
//     set_password();
//   }
// }

void loop() {
  state = LOGGED_OUT;
  char key = '\0';
  bool scanned = false;
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
    Serial.println("Dk won");
  } else {
    Serial.println("You're mid");
  }
  Serial.print("state: ");
  Serial.println(state);

  delay(2000);

}
