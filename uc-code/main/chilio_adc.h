#ifndef CHILIO_ADC_H_
#define CHILIO_ADC_H_
#include <stdint.h>

void chilio_adc_init(void);
int chilio_adc_get_raw(uint8_t channel);
#endif // #ifndef CHILIO_ADC_H_
