#ifndef MY_GPIO_H
#define MY_GPIO_H

/* Register definition - START*/
#define MY_PINB  (*(volatile uint8_t *)0x23)
#define MY_DDRB  (*(volatile uint8_t *)0x24)
#define MY_PORTB (*(volatile uint8_t *)0x25)

#define MY_PINC  (*(volatile uint8_t *)0x26)
#define MY_DDRC  (*(volatile uint8_t *)0x27)
#define MY_PORTC (*(volatile uint8_t *)0x28)

#define MY_PIND  (*(volatile uint8_t *)0x29)
#define MY_DDRD  (*(volatile uint8_t *)0x2A)
#define MY_PORTD (*(volatile uint8_t *)0x2B)
/* Register definition - END*/

typedef enum {
    GPIO_INPUT = 0, 
    GPIO_OUTPUT = 1
} my_gpio_dir_e; 

typedef enum {
    GPIO_PIN_LOW = 0, 
    GPIO_PIN_HIGH,
} my_gpio_pin_state_e; 

typedef enum {
    GPIO_PIN_0, 
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4, 
    GPIO_PIN_5,
    GPIO_PIN_6, 
    GPIO_PIN_7,
} my_gpio_pin_e; 

typedef enum {
    GPIO_PORT_B, 
    GPIO_PORT_C, 
    GPIO_PORT_D, 
} my_gpio_ports_e; 

typedef enum {
    GPIO_OK = 0, 
    GPIO_INVALID_PORT, 
    GPIO_INVALID_PIN, 
    GPIO_INVALID_OUTPUT_VAL,
    GPIO_INVALID_DIR,
    GPIO_INVALID_PTR_ERROR, 
} my_gpio_status_e; 

/* Function prototypes */
my_gpio_status_e my_gpio_set_direction(my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM, my_gpio_dir_e direction); 
my_gpio_status_e my_gpio_set_output_value(my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM, my_gpio_pin_state_e output_val); 
my_gpio_status_e my_gpio_read(my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM, my_gpio_pin_state_e *pin_state); 
my_gpio_status_e my_gpio_toggle_pin(my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM); 
my_gpio_status_e my_gpio_enable_input_pullup(my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM); 
#endif 