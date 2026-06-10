#include "my_gpio.h"

int main(void) {
    //TODO: implement the code to test the GPIO APIs
    my_gpio_set_direction(GPIO_PORT_B, GPIO_PIN_5, GPIO_OUTPUT);
    my_gpio_set_output_value(GPIO_PORT_B, GPIO_PIN_5, GPIO_PIN_HIGH); 
    while (1);  
    return 0; 
}
