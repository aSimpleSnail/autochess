#ifndef GPIO_H_
#define GPIO_H_
#define debug

typedef enum { IN = 0, OUT = 1 } gpio_mode_t;

short initPin(short);
short setPinMode(short, gpio_mode_t);
short uninitPin(short);
short readPin(short);
short writePin(short, short);
#endif