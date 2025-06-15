#include "sram.h"
#include "lcd.h"

void setup() {
    Serial.begin(9600);

    randomSeed(analogRead(0));

    sram_init();
    lcd_init();
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');

        command.trim();

        String args[MAX_ARGS];

        int argc = parse_args(command, args, MAX_ARGS);

        if (args[0] == "w") {
            uint16_t address = strtol(args[1].c_str(), NULL, 0);
            uint8_t data = strtol(args[2].c_str(), NULL, 0);

            sram_write(address, data);
        }
        else if (args[0] == "r") {
            uint16_t address = strtol(args[1].c_str(), NULL, 0);

            uint8_t data = sram_read(address);
        }
    }
    else {
        sram_write(random(pow(2, 15) - 1), random(UINT8_MAX));

        delay(1000);
    }
}

void sram_init() {
    for (int i = 0; i < SRAM_ADDRESS_BUS_LENGTH; i++) {
        pinMode(sram_address_bus_pins[i], OUTPUT);
    }

    for (int i = 0; i < SRAM_DATA_BUS_LENGTH; i++) {
        pinMode(sram_data_bus_pins[i], OUTPUT);
    }

    pinMode(SRAM_WRITE_ENABLE_PIN, OUTPUT);
    pinMode(SRAM_OUTPUT_ENABLE_PIN, OUTPUT);
    pinMode(SRAM_CHIP_SELECT_PIN, OUTPUT);

    digitalWrite(SRAM_WRITE_ENABLE_PIN, HIGH);
    digitalWrite(SRAM_OUTPUT_ENABLE_PIN, HIGH);
    digitalWrite(SRAM_CHIP_SELECT_PIN, HIGH);
}

uint8_t sram_read(uint16_t address) {
    sram_update_address_bus(address);

    for (int i = 0; i < SRAM_DATA_BUS_LENGTH; i++) {
        pinMode(sram_data_bus_pins[i], INPUT);
    }

    digitalWrite(SRAM_WRITE_ENABLE_PIN, HIGH);
    digitalWrite(SRAM_OUTPUT_ENABLE_PIN, LOW);
    digitalWrite(SRAM_CHIP_SELECT_PIN, LOW);

    delayMicroseconds(1); // access time

    uint8_t data = sram_get_data();

    digitalWrite(SRAM_OUTPUT_ENABLE_PIN, HIGH);
    digitalWrite(SRAM_CHIP_SELECT_PIN, HIGH);

    lcd.display.clear();

    lcd.display.print("READ(");
    lcd.display.print("A:0x");
    lcd.display.print(address, HEX);
    lcd.display.print(")");

    lcd.display.setCursor(0, 1);

    lcd.display.print("-> D:0x");
    lcd.display.print(data, HEX);

    return data;
}

void sram_write(uint16_t address, uint8_t data) {
    sram_update_address_bus(address);

    for (int i = 0; i < SRAM_DATA_BUS_LENGTH; i++) {
        pinMode(sram_data_bus_pins[i], OUTPUT);
    }

    sram_update_data_bus(data);

    digitalWrite(SRAM_WRITE_ENABLE_PIN, LOW);
    digitalWrite(SRAM_OUTPUT_ENABLE_PIN, HIGH);
    digitalWrite(SRAM_CHIP_SELECT_PIN, LOW);

    delayMicroseconds(1); // hold time

    digitalWrite(SRAM_WRITE_ENABLE_PIN, HIGH);
    digitalWrite(SRAM_CHIP_SELECT_PIN, HIGH);

    lcd.display.clear();

    lcd.display.print("WRITE(");
    lcd.display.print("D:0x");
    lcd.display.print(data, HEX);
    lcd.display.print(")");

    lcd.display.setCursor(0, 1);

    lcd.display.print("->A:0x");
    lcd.display.print(address, HEX);
}

uint8_t sram_get_data() {
    uint8_t data = 0;

    for (int i = 0; i < SRAM_DATA_BUS_LENGTH; i++) {
        data |= (digitalRead(sram_data_bus_pins[i]) << i);
    }

    return data;
}

void sram_update_address_bus(uint16_t address) {
    for (int i = 0; i < SRAM_ADDRESS_BUS_LENGTH; i++) {
        digitalWrite(sram_address_bus_pins[i], (address >> i) & HIGH);
    }
}

void sram_update_data_bus(uint8_t data) {
    for (int i = 0; i < SRAM_DATA_BUS_LENGTH; i++) {
        digitalWrite(sram_data_bus_pins[i], (data >> i) & HIGH);
    }
}

void lcd_init() {
    lcd.display.begin(16, 2);

    lcd.display.print("Initialized");
}

unsigned int parse_args(String command, String* args, int max_args) {
    int argc = 0;

    int arg_index = 0;
    int space_index = command.indexOf(' ');

    while (space_index != -1 && argc < max_args - 1) {
        args[argc++] = command.substring(arg_index, space_index);

        arg_index = space_index + 1;

        space_index = command.indexOf(' ', arg_index);
    }

    if (argc < max_args) {
        args[argc++] = command.substring(arg_index);
    }

    return argc;
}