#include "my_gpio.h"
#include <stdint.h>
#include <stddef.h> 

/* Function Definition */
my_gpio_status_e my_gpio_set_direction(my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM, my_gpio_dir_e direction)
{
    //Check if valid values are passed
    if (GPIO_PORT < GPIO_PORT_B || GPIO_PORT > GPIO_PORT_D) {
        return GPIO_INVALID_PORT; 
    }

    if (GPIO_PIN_NUM < GPIO_PIN_0 || GPIO_PIN_NUM > GPIO_PIN_7) {
        return GPIO_INVALID_PIN; 
    }

    if (direction < GPIO_INPUT || direction > GPIO_OUTPUT) {
        return GPIO_INVALID_DIR; 
    }

    switch (GPIO_PORT)
    {
        case GPIO_PORT_B:
        if (direction == GPIO_OUTPUT) {
            MY_DDRB |= (uint8_t)(1 << GPIO_PIN_NUM); 
        }
        else {
            MY_DDRB &= ~(uint8_t)(1 << GPIO_PIN_NUM); 
        }
        break; 

        case GPIO_PORT_C:
        if (direction == GPIO_OUTPUT) {
            MY_DDRC |= (uint8_t)(1 << GPIO_PIN_NUM); 
        }
        else {
            MY_DDRC &= ~(uint8_t)(1 << GPIO_PIN_NUM); 
        }
        
        case GPIO_PORT_D:
        if (direction == GPIO_OUTPUT) {
            MY_DDRD |= (uint8_t)(1 << GPIO_PIN_NUM); 
        }
        else {
            MY_DDRD &= ~(uint8_t)(1 << GPIO_PIN_NUM); 
        }
        
        default: 
        break; 
    }

    return GPIO_OK; 
}

my_gpio_status_e my_gpio_set_output_value (my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM, my_gpio_pin_state_e output_val)
{
    //Check if valid values are passed
    if (GPIO_PORT < GPIO_PORT_B || GPIO_PORT > GPIO_PORT_D) {
        return GPIO_INVALID_PORT; 
    }

    if (GPIO_PIN_NUM < GPIO_PIN_0 || GPIO_PIN_NUM > GPIO_PIN_7) {
        return GPIO_INVALID_PIN; 
    }

    if (output_val < GPIO_PIN_LOW || output_val > GPIO_PIN_HIGH) {
        return GPIO_INVALID_OUTPUT_VAL; 
    }

    switch (GPIO_PORT)
    {
    case GPIO_PORT_B:
        if (output_val == GPIO_PIN_HIGH) {
            MY_PORTB |= (1U << GPIO_PIN_NUM); 
        }
        else {
            MY_PORTB &= ~(1U << GPIO_PIN_NUM); 
        }
    break; 
    
    case GPIO_PORT_C: 
        if (output_val == GPIO_PIN_HIGH) {
            MY_PORTC |= (1U << GPIO_PIN_NUM); 
        }
        else {
            MY_PORTC &= ~(1U << GPIO_PIN_NUM); 
        }
    break; 

    case GPIO_PORT_D:
        if (output_val == GPIO_PIN_HIGH) {
            MY_PORTD |= (1U << GPIO_PIN_NUM); 
        }
        else {
            MY_PORTD &= ~(1U << GPIO_PIN_NUM); 
        }
    break; 

    default:
        break; 
    }
    return GPIO_OK; 
}

my_gpio_status_e my_gpio_read(my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM, my_gpio_pin_state_e *pin_state) {
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

my_gpio_status_e my_gpio_toggle_pin(my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM) {
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

my_gpio_status_e my_gpio_enable_input_pullup(my_gpio_ports_e GPIO_PORT, my_gpio_pin_e GPIO_PIN_NUM) {
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