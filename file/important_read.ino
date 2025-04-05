#include <EEPROM.h>

void setup() {
    Serial.begin(9600);
    while (!Serial);
    char *dk = "DK won";
    for (int i = 0; *dk != '\0'; dk++, i++) {
        Serial.println(i);
        EEPROM.read(i, *dk);
    }
}

void loop() {

}
