#ifndef _MY_GPIO_H
#define _MY_GPIO_H

/* Register definition - START*/
#define MY_PINB  (*(volatile uint8_t *)0x23)
#define MY_DDRB  (*(volatile uint8_t *)0x24)
#define MY_PORTB (*(volatile uint8_t *)0x25)

#endif 