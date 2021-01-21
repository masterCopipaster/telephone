/*
 * iomacro.h
 *
 * Created: 17.07.2019 0:49:11
 *  Author: andro
 */ 


#ifndef IOMACRO_H_
#define IOMACRO_H_
#include <avr/io.h>

#define SET_BIT(port, bit) port |= (1 << bit);
#define CLR_BIT(port, bit) port &= ~(1 << bit);

#define PIN_SET_1(pin) \
{SET_BIT(DDRB, pin); \
SET_BIT(PORTB, pin);}

#define PIN_SET_0(pin) \
{SET_BIT(DDRB, pin); \
CLR_BIT(PORTB, pin);}

#define PIN_SET_IN(pin) \
{CLR_BIT(DDRB, pin); \
CLR_BIT(PORTB, pin);}

#define PIN_SET_IN_PU(pin) \
{CLR_BIT(DDRB, pin); \
SET_BIT(PORTB, pin);}

#define PIN_IS_1(pin) ((PINB & (1 << pin)) != 0)
#define PIN_IS_0(pin) ((PINB & (1 << pin)) == 0)

#define RELAY_PIN 2
#define DYN_PIN 1
#define OUT_PIN 0
#define BTN1_PIN 3
#define BTN2_PIN 4

#define DYN_MUTE PIN_SET_IN(DYN_PIN)
#define DYN_ON PIN_SET_0(DYN_PIN)

#define OUT_MUTE PIN_SET_IN(OUT_PIN)
#define OUT_ON PIN_SET_0(OUT_PIN)

#endif /* IOMACRO_H_ */