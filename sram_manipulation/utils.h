#ifndef __UTILS_H
#define __UTILS_H

#include <Arduino.h>

#define MAX_ARGS (3)

typedef unsigned int pin_t;

unsigned int parse_args(String command, String* args, int max_args);

#endif