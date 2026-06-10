#include "my_gpio.h"
#include <stdint.h>
#include <stddef.h> 

/* Function Definition */
my_gpio_status_e my_gpio_set_direction(my_gpio_port_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM, my_gpio_dir_e direction)
{
    //Check if valid values are passed
    // if (GPIO_PORT < GPIO_PORT_B || GPIO_PORT > GPIO_PORT_D) {
    //     return GPIO_INVALID_PORT; 
    // }

    if (GPIO_PIN_NUM < GPIO_PIN_0 || GPIO_PIN_NUM > GPIO_PIN_7) {
        return GPIO_INVALID_PIN; 
    }

    if (direction < GPIO_INPUT || direction > GPIO_OUTPUT) {
        return GPIO_INVALID_DIR; 
    }

    volatile uint8_t *ddr_reg = NULL; 
    switch (GPIO_PORT)
    {
        case GPIO_PORT_B: ddr_reg = &MY_DDRB;
        break; 

        case GPIO_PORT_C: ddr_reg = &MY_DDRC;
        break; 

        case GPIO_PORT_D: ddr_reg = &MY_DDRC;
        break; 

        default: return GPIO_INVALID_PORT; 
        break; 
    }

    //Step 2: Apply direction logic
    if (GPIO_OUTPUT == direction) {
        *ddr_reg |= (1U << GPIO_PIN_NUM); 
    }
    else {
        *ddr_reg &= ~(1U << GPIO_PIN_NUM); 
    }
    return GPIO_OK; 
}

my_gpio_status_e my_gpio_set_output_value (my_gpio_port_e GPIO_PORT,
                                           my_gpio_pin_e GPIO_PIN_NUM,
                                           my_gpio_pin_state_e output_val)
{
    if (GPIO_PIN_NUM < GPIO_PIN_0 || GPIO_PIN_NUM > GPIO_PIN_7) {
        return GPIO_INVALID_PIN; 
    }

    if (output_val < GPIO_PIN_LOW || output_val > GPIO_PIN_HIGH) {
        return GPIO_INVALID_OUTPUT_VAL; 
    }

    // Step 1: Select the correct PORT register
    volatile uint8_t *port_reg = NULL;
    switch (GPIO_PORT)
    {
    case GPIO_PORT_B: port_reg = &MY_PORTB;
        break; 

    case GPIO_PORT_C:  port_reg = &MY_PORTC;
        break; 

    case GPIO_PORT_D:  port_reg = &MY_PORTD;
        break; 

    default: return GPIO_INVALID_PORT; 
        break; 
    }
    
    // Step 2: Apply output logic once
    if (output_val == GPIO_PIN_HIGH) {
        *port_reg |= (1U << GPIO_PIN_NUM);
    } else {
        *port_reg &= ~(1U << GPIO_PIN_NUM);
    }
    
    return GPIO_OK; 
}

my_gpio_status_e my_gpio_read(my_gpio_port_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM, my_gpio_pin_state_e *pin_state) {
    //Check if correct values are passed
    if (GPIO_PORT < GPIO_PORT_B || GPIO_PORT > GPIO_PORT_D) {
        return GPIO_INVALID_PORT; 
    }
    if (GPIO_PIN_NUM < GPIO_PIN_0 || GPIO_PIN_NUM > GPIO_PIN_7) {
        return GPIO_INVALID_PIN; 
    }
    if (NULL == pin_state) {
        return GPIO_INVALID_PTR_ERROR; 
    }

    switch (GPIO_PORT) {
        case GPIO_PORT_B:
            //Check if PIN register of the passed pin is 0 or 1
            if (MY_PINB & (1U << GPIO_PIN_NUM)) {
                *pin_state = GPIO_PIN_HIGH; 
            }
            else {
                *pin_state = GPIO_PIN_LOW;
            }
        break; 

        case GPIO_PORT_C:
            if (MY_PINC & (1U << GPIO_PIN_NUM)) {
                *pin_state = GPIO_PIN_HIGH; 
            }
            else {
                *pin_state = GPIO_PIN_LOW;
            }
        break; 

        case GPIO_PORT_D:
            if (MY_PIND & (1U << GPIO_PIN_NUM)) {
                *pin_state = GPIO_PIN_HIGH; 
            }
            else {
                *pin_state = GPIO_PIN_LOW;
            }        
        break;

        default:
        break; 
    }

    return GPIO_OK; 
}

my_gpio_status_e my_gpio_toggle_pin(my_gpio_port_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM) {
    //Check if correct values are passed
    if (GPIO_PORT < GPIO_PORT_B || GPIO_PORT > GPIO_PORT_D) {
        return GPIO_INVALID_PORT; 
    }
    if (GPIO_PIN_NUM < GPIO_PIN_0 || GPIO_PIN_NUM > GPIO_PIN_7) {
        return GPIO_INVALID_PIN; 
    }

    switch (GPIO_PORT) {
        case GPIO_PORT_B:
            //Check if PIN register of the passed pin is 0 or 1
            MY_PORTB ^= (1U << GPIO_PIN_NUM); 
        break; 

        case GPIO_PORT_C:
            MY_PORTC ^= (1U << GPIO_PIN_NUM); 
        break; 

        case GPIO_PORT_D:
            MY_PORTD ^= (1U << GPIO_PIN_NUM); 
        break;

        default:
        break; 
    }

    return GPIO_OK; 
}

my_gpio_status_e my_gpio_enable_input_pullup(my_gpio_port_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM) {
    my_gpio_status_e status; 
    // As per the 13.2.3 Switching Between Input and Output section of the data sheet for AVR ATmega328P
    // STEP 1: Set the PORT value to HIGH *before* changing direction.
    // If the pin was previously an Output, this safely drives it High (5V).
    // If the pin was already an Input, this pre-loads the Pull-Up resistor.
    status = my_gpio_set_output_value(GPIO_PORT, GPIO_PIN_NUM, GPIO_PIN_HIGH);
    
    // Safety Check: If the user passed a bad port/pin, bail out immediately!
    if (status != GPIO_OK) {
        return status;
    }

    // STEP 2: Set the Direction to INPUT.
    // If the pin was an Output, it now safely becomes an Input with Pull-Up.
    // The outside world never sees a sudden drop to 0V (Ground).
    status = my_gpio_set_direction(GPIO_PORT, GPIO_PIN_NUM, GPIO_INPUT);
    
    return status;
}