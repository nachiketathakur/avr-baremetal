#include "my_gpio.h"
#include <stdint.h>

int main(void) {
    my_gpio_set_direction(GPIO_PORT_B, GPIO_PIN_5, GPIO_OUTPUT);
    volatile uint32_t counter; 
    while (1) {
        my_gpio_toggle_pin(GPIO_PORT_B, GPIO_PIN_5); 
        for (counter = 0; counter < 200000; counter++); 
    } 
    return 0; 
}
