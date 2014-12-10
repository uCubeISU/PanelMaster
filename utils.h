/*
 * utils.h
 *
 *  Created on: Dec 9, 2014
 *      Author: vens
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

typedef enum
{
	EXIT_SUCCESS = 0,
	EXIT_FAILURE = -1,
	EXIT_WARNING = 1
}return_status;

typedef struct PIN_REGISTER_STRUCT
{
	volatile unsigned char *in;
	volatile unsigned char *out;
	volatile unsigned char *dir;
	volatile unsigned char *ren;
	unsigned char bit;
}pin_register_t;

#define PIN_SET_HIGH(pin)  			*pin.out |= pin.bit
#define PIN_SET_LOW(pin)	 		*pin.out &= ~pin.bit
#define PIN_SET_OUTPUT(pin)			*pin.dir |= pin.bit
#define PIN_SET_INPUT(pin)			*pin.dir &= ~pin.bit
#define PIN_GET_VALUE(pin)			(*pin.in) & pin.bit ? 1 : 0
#define PIN_ENABLE_PULLUP(pin)		*pin.ren |= pin.bit, PIN_SET_HIGH(pin)
#define PIN_ENABLE_PULLDOWN(pin)	*pin.ren |= pin.bit, PIN_SET_LOW(pin)
#define PIN_DISABLE_RESISTOR(pin)	*pin.ren &= ~pin.bit


#define PIN_CREATE_PIN(port, pin) 	{.in = P##port##IN, .out = P##port##OUT, .dir = P##port##DIR, .ren = P##port##REN, .bit = pin}
#define PIN_SET_PIN(out_pin, port, pin) {out_pin.in = &P##port##IN; out_pin.out = &P##port##OUT; out_pin.dir = &P##port##DIR; out_pin.ren = &P##port##REN; out_pin.bit = pin;}
#endif /* UTILS_H_ */
