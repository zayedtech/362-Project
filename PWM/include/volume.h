#ifndef VOLUME_H
#define VOLUME_H

// Initializes the ADC hardware and DMA engine.
// Call this once inside main() before the loop.
void init_volume_system(void);

// Returns the current knob position.
// Range: 0.0 (Silent) to 1.0 (Max Volume)
float get_volume(void);

#endif