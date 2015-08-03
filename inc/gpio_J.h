#ifndef _GPIO_J_h_
#define _GPIO_J_h_


#include "gpio2_ioctl.h"
#define LED_RED_RIGHT   33
#define LED_RED_LEFT    30
#define LED_GREEN_RIGHT 31
#define LED_GREEN_LEFT  32

#define GPIO_DEV "/dev/gpio"
/*
enum gpio_mode {
	GPIO_INPUT = 0,				//!< Pin configured for input
	GPIO_OUTPUT_LOW,			//!< Pin configured for output with low level
	GPIO_OUTPUT_HIGH,			//!< Pin configured for output with high level
};

enum gpio_irq_mode {
	GPIO_IRQ_TYPE_EDGE_RISING = 0,
	GPIO_IRQ_TYPE_EDGE_FALLING = 1,
	GPIO_IRQ_TYPE_EDGE_BOTH = 2,
	GPIO_IRQ_TYPE_LEVEL_HIGH = 3,
	GPIO_IRQ_TYPE_LEVEL_LOW = 4,
};

struct gpio_direction {
	int pin;
	enum gpio_mode mode;
};

struct gpio_data {
	int pin;
	int value;
};

struct gpio_irq {
	int pin;
	enum gpio_irq_mode mode;
};
*/

void GpioOpen();
void GpioDir(int pin,enum gpio_mode mode);
void GpioWrite(int pin, int value);


#endif
