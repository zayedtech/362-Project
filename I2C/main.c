#include <stddef.h>
#include <stdint.h>
#include "i2c_utils.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>


// This is the actual main function that will be used later to combine all the other main functions in the peripherals folders

int main() {
    
    stdio_init_all();
    sleep_ms(300);
    i2c_setup();
    i2c_scan();
    while(1) tight_loop_contents();
}