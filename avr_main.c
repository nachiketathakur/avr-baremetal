#include <avr/io.h>
#include <util/delay.h>

int add_numbers (int a, int b); 
int main(void)
{
    add_numbers(5, 6); 
    while (1);  //this is a comment
    return 0; //this is a comment too 
}

int add_numbers (int a, int b)
{
    int output = a + b; 
    return output; 
}