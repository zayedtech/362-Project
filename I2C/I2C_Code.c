#include <stdio.h>
#include "i2c_utils.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"


#define I2C_PORT    i2c0
#define SDA_PIN     28
#define SCL_PIN     29


void i2c_setup(void) {
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

}


void i2c_scan(void) {
    printf("I2C scan:\n");
    for(uint8_t addr = 0x08; addr <= 0x77; addr++) {
        int ret = i2c_write_blocking(I2C_PORT, addr, NULL, 0, false);
        if(ret >= 0) printf(" found 0x%02X\n", addr);
    }
}