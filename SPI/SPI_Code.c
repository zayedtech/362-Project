#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "include/lcd.h"


#include <stdio.h>
#include <string.h>
#include <math.h>   

/****************************************** */
// LCD Pins
#define PIN_SDI    7
#define PIN_CS     5
#define PIN_SCK    6
#define PIN_DC     3
#define PIN_nRESET 4

// SD Card Pins
#define SD_MISO    8
#define SD_CS      9
#define SD_SCK     10
#define SD_MOSI    11

/* LCD Setup and functions */

void init_spi_lcd() {
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_function(PIN_DC, GPIO_FUNC_SIO);
    gpio_set_function(PIN_nRESET, GPIO_FUNC_SIO);

    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_set_dir(PIN_nRESET, GPIO_OUT);

    gpio_put(PIN_CS, 1); // CS high
    gpio_put(PIN_DC, 0); // DC low
    gpio_put(PIN_nRESET, 1); // nRESET high

    // initialize SPI1 with 48 MHz clock
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SDI, GPIO_FUNC_SPI);
    spi_init(spi0, 48 * 1000 * 1000);
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

// call next two in main

void LCD_start() {
    stdio_init_all();

    init_spi_lcd();

    LCD_Setup();
    LCD_Clear(BLACK); // Clear the screen to black
}

void LCD_note(int n)
{
    switch (n){
        case 0:
            LCD_Clear(RED); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, RED, "Note: C4", 16, 0);
            LCD_DrawString(0, 20, BLACK, RED, "Frequency: 262", 16, 0);
        case 1:
            LCD_Clear(GREEN); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, GREEN, "Note: D4", 16, 0);
            LCD_DrawString(0, 20, BLACK, GREEN, "Frequency: 294", 16, 0);
        case 2:
            LCD_Clear(BLUE); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, BLUE, "Note: E4", 16, 0);
            LCD_DrawString(0, 20, BLACK, BLUE, "Frequency: 330", 16, 0);
        case 3:
            LCD_Clear(YELLOW); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, YELLOW, "Note: F4", 16, 0);
            LCD_DrawString(0, 20, BLACK, YELLOW, "Frequency: 349", 16, 0);
        case 4:
            LCD_Clear(MAGENTA); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, MAGENTA, "Note: G4", 16, 0);
            LCD_DrawString(0, 20, BLACK, MAGENTA, "Frequency: 392", 16, 0);
        case 5:
            LCD_Clear(CYAN); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, CYAN, "Note: A4", 16, 0);
            LCD_DrawString(0, 20, BLACK, CYAN, "Frequency: 440", 16, 0);
        case 6:
            LCD_Clear(LBBLUE); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, LBBLUE, "Note: B4", 16, 0);
            LCD_DrawString(0, 20, BLACK, LBBLUE, "Frequency: 494", 16, 0);
        case 7:
            LCD_Clear(BROWN); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, BROWN, "Note: C5", 16, 0);
            LCD_DrawString(0, 20, BLACK, BROWN, "Frequency: 523", 16, 0);
        case 8:
            LCD_Clear(GREEN); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, GREEN, "Note: D5", 16, 0);
            LCD_DrawString(0, 20, BLACK, GREEN, "Frequency: 587", 16, 0);
        case 9:
            LCD_Clear(GBLUE); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, GBLUE, "Note: E5", 16, 0);
            LCD_DrawString(0, 20, BLACK, GBLUE, "Frequency: 659", 16, 0);
        case 10:
            LCD_Clear(BRRED); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, BRRED, "Note: F5", 16, 0);
            LCD_DrawString(0, 20, BLACK, BRRED, "Frequency: 698", 16, 0);
        case 11:
            LCD_Clear(MAGENTA); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, MAGENTA, "Note: G5", 16, 0);
            LCD_DrawString(0, 20, BLACK, MAGENTA, "Frequency: 784", 16, 0);
        case 12:
            LCD_Clear(YELLOW); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, YELLOW, "Note: A5", 16, 0);
            LCD_DrawString(0, 20, BLACK, YELLOW, "Frequency: 880", 16, 0);
        case 13:
            LCD_Clear(BRRED); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, BRRED, "Note: B5", 16, 0);
            LCD_DrawString(0, 20, BLACK, BRRED, "Frequency: 988", 16, 0);
        case 14:
            LCD_Clear(LBBLUE); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, LBBLUE, "Note: C6", 16, 0);
            LCD_DrawString(0, 20, BLACK, LBBLUE, "Frequency: 1047", 16, 0);
        case 15:
            LCD_Clear(MAGENTA); // Clear the screen to black
            LCD_DrawString(0, 0, BLACK, MAGENTA, "Note: D6", 16, 0);
            LCD_DrawString(0, 20, BLACK, MAGENTA, "Frequency: 1175", 16, 0);
        default:

    }
}


/* SD Card Setup and functions */

void init_spi_sdcard() {
    gpio_set_function(SD_CS, GPIO_FUNC_SIO);

    gpio_set_dir(SD_CS, GPIO_OUT);

    gpio_put(SD_CS, 1); // CS high

    // initialize SPI1 with 400 KHz clock

    gpio_set_function(SD_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SD_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SD_MISO, GPIO_FUNC_SPI);
    spi_init(spi1, 400000);
    spi_set_format(spi1, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

void disable_sdcard() {
    gpio_put(SD_CS, 1); // CS high

    gpio_set_function(SD_MOSI, GPIO_FUNC_SIO); // MOSI GPIO
    gpio_set_dir(SD_MOSI, GPIO_OUT);
    gpio_put(SD_MOSI, 1); // MOSI high

}

void enable_sdcard() {
    gpio_put(SD_CS, 0);
    gpio_set_function(SD_MOSI, GPIO_FUNC_SPI);
}

void sdcard_io_high_speed() {
    spi_set_baudrate(spi1, 12000000);
}

void init_sdcard_io() {
    init_spi_sdcard();
    disable_sdcard();
}

void init_uart();
void init_uart_irq();
void date(int argc, char *argv[]);
void command_shell();

