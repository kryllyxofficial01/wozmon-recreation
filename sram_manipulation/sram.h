#ifndef __SRAM_H
#define __SRAM_H

#include "utils.h"

#define SRAM_WRITE_ENABLE_PIN ((pin_t) (14)) /* enable low */
#define SRAM_OUTPUT_ENABLE_PIN ((pin_t) (15)) /* enable low */
#define SRAM_CHIP_SELECT_PIN ((pin_t) (16)) /* enable low */

#define SRAM_ADDRESS_BUS_LENGTH (15)
#define SRAM_DATA_BUS_LENGTH (8)

const pin_t sram_address_bus_pins[SRAM_ADDRESS_BUS_LENGTH] = {
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
};

const pin_t sram_data_bus_pins[SRAM_DATA_BUS_LENGTH] = {
    42, 43, 44, 45, 46, 47, 48, 49
};

void sram_init();

uint8_t sram_read(uint16_t address);
void sram_write(uint16_t address, uint8_t data);

uint8_t sram_get_data();

void sram_update_address_bus(uint16_t address);
void sram_update_data_bus(uint8_t data);

unsigned int parse_args(String command, String* args, int max_args);

#endif