#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "neotrellis.h"
#include "seesaw.h"
#include "tusb_config.h"
#include "lcd.h"


#define BUZZER_PIN 15  
#define VOL_PIN 45      
#define VOL_CHAN 5      
#define SYS_CLK_FREQ 150000000 

// GLOBALS 
static uint slice_num;
static uint32_t current_top = 0;
volatile uint32_t volume_raw = 0;

void init_volume_system(void) {
    adc_init();
    adc_gpio_init(VOL_PIN);
    adc_select_input(VOL_CHAN);
    adc_fifo_setup(true, true, 1, false, false);
    adc_set_clkdiv(48000.f);

    int dma_chan = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, DREQ_ADC);

    dma_channel_configure(dma_chan, &c, &volume_raw, &adc_hw->fifo, 0, false);
    dma_channel_set_trans_count(dma_chan, 0xFFFFFFFF, true); 
    adc_run(true);
}

float get_volume_scalar(void) {
   
    float current_input = (float)volume_raw / 4095.0f;

    static float smoothed_vol = 0.0f;
    smoothed_vol = (0.9f * smoothed_vol) + (0.1f * current_input);
    float v = smoothed_vol;

   // if (v < 0.02f) v = 0.0f;

    return v; 
}


void pwm_audio_init(void) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice_num, false);
    
    // Reset divider to default!!!
    pwm_set_clkdiv(slice_num, 1.0f);
}

void pwm_play_tone(uint16_t freq) {
    if (freq == 0) {
        pwm_set_enabled(slice_num, false);
        current_top = 0;
        return;
    }

    uint32_t system_clock = SYS_CLK_FREQ;
    uint32_t top = 0;
    float divider = 1.0f;


    uint32_t count = system_clock / freq;

    // Fit into 16-bit (Max 65535)
    if (count < 65535) {
        top = count - 1;
        divider = 1.0f;
    } else {
        divider = 16.0f;
        top = (system_clock / (freq * 16)) - 1;
        
        // IF too big, divide more
        if (top > 65535) {
            divider = 128.0f;
            top = (system_clock / (freq * 128)) - 1;
        }
    }

    current_top = top;

    // 50% Duty Cycle
    uint16_t level = top / 2;
    
    pwm_set_clkdiv(slice_num, divider); 
    pwm_set_wrap(slice_num, (uint16_t)top);
    pwm_set_gpio_level(BUZZER_PIN, level); //Channel B)
    pwm_set_enabled(slice_num, true);

    printf("PLAYING: %d Hz | Div: %.1f | Top: %d | Level: %d\n", freq, divider, top, level);
}


void pwm_update_volume(void) {
    if (current_top > 0) {
        
        float vol = get_volume_scalar();
        
        uint16_t level = (uint16_t)((current_top / 2) * vol);
        
       
        pwm_set_gpio_level(BUZZER_PIN, level);
    }
}


static const uint16_t notes[16] = {
    262, 294, 330, 349, 392, 440, 494, 523, 
    587, 659, 698, 784, 880, 988, 1047, 1175 
};


void play_note(int idx) {
    if (idx >= 0 && idx < 16) pwm_play_tone(notes[idx]);
}

void stop_note(void) {
    pwm_play_tone(0);
}

static void scan_i2c(void) {
    printf("I2C scan:\n");
    for (uint8_t a = 0x08; a <= 0x77; a++) {
        uint8_t dummy = 0;
        int r = i2c_write_blocking(NEOTRELLIS_I2C, a, &dummy, 1, false);
        if (r >= 0) printf("  Found 0x%02X\n", a);
    }
}

int main() {
    stdio_init_all();
    setvbuf(stdout, NULL, _IONBF, 0);   
    sleep_ms(500);    

    seesaw_bus_init(400000);
    pwm_audio_init();  
    init_volume_system();
    scan_i2c();
    
    if (!neotrellis_reset()) while (1); 
    sleep_ms(1000);
    if (!neotrellis_wait_ready(500)) return 0;
    if (!neopixel_begin(3)) {
        while(1){
            tight_loop_contents();
        }
    }


    uint8_t speed = 0x01;  
    seesaw_write(NEOTRELLIS_ADDR, SEESAW_NEOPIXEL_BASE, NEOPIXEL_SPEED, &speed, 1);
    sleep_ms(300);
    
    LCD_start();
    neotrellis_rainbow_startup();
    
    sleep_ms(200);
    fflush(stdout);
    neotrellis_keypad_init();
    
    sleep_ms(200);
    neotrellis_clear_fifo(); 

    printf("DIAGNOSTIC MODE: PRESS A BUTTON\n");
    
    int idx = -1;
    uint32_t last_print = 0;

    while (1) {
        neotrellis_poll_buttons(&idx);
        pwm_update_volume();

        // uint32_t now = to_ms_since_boot(get_absolute_time());
        // if (now - last_print > 200) {
        //     printf("Knob Raw: %4d | Vol: %.2f\r", volume_raw, get_volume_scalar());
        //     last_print = now;
        // }

      
    }
    return 0;
}