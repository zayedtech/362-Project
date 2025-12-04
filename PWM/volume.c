
#include "volume.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"


#define VOL_PIN 45      
#define VOL_CHAN 5       


volatile uint32_t volume_raw = 0; 
int dma_chan;

void init_volume_system(void) {
    // Setup ADC
    adc_init();
    adc_gpio_init(VOL_PIN);
    adc_select_input(VOL_CHAN);
    
    // ADC FIFO
    adc_fifo_setup(
        true,    
        true,    
        1,       
        false,  
        false 
    );
    
    // Set the speed
    adc_set_clkdiv(0); 

    // Setup dma
    dma_chan = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    
    
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    
    channel_config_set_read_increment(&c, false);
    
    channel_config_set_write_increment(&c, false);
   
    channel_config_set_dreq(&c, DREQ_ADC);

    
    dma_channel_configure(
        dma_chan,       
        &c,            
        &volume_raw,    
        &adc_hw->fifo,  
        0,              
        false           
    );

   
    dma_channel_set_trans_count(dma_chan, 0xFFFFFFFF, true);
    
    adc_run(true);
    
    printf("[VOLUME] DMA System initialized on Pin %d\n", VOL_PIN);
}

float get_volume(void) {
    // Convert 0 to 4095 to 0-1
    float v = (float)volume_raw / 4095.0f;
    
    v = v * v; 

    if (v < 0.02f) v = 0.0f;//dead zone
    return v;
}