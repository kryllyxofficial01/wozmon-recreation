#ifndef __LCD_H
#define __LCD_H

#include <LiquidCrystal.h>

#include "utils.h"

#define LCD_ENABLE_PIN ((pin_t) (11))
#define LCD_REGISTER_SELECT_PIN ((pin_t) (12))

#define LCD_DATA_BUS_LENGTH (4)

static struct _LCD {
    const pin_t data_pins[LCD_DATA_BUS_LENGTH] = {
        5, 4, 3, 2
    };

    LiquidCrystal display = LiquidCrystal(
        LCD_REGISTER_SELECT_PIN,
        LCD_ENABLE_PIN,
        this->data_pins[0],
        this->data_pins[1],
        this->data_pins[2],
        this->data_pins[3]
    );
} lcd;

void lcd_init();

#endif