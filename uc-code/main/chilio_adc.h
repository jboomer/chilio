#ifndef CHILIO_ADC_H_
#define CHILIO_ADC_H_
#include <stdint.h>
#include "chilio_defs.h"

void chilio_adc_init(void);
CErr chilio_get_average(uint8_t channel, float* val); 
void chilio_adc_read_task(void* pvParameter);
#endif // #ifndef CHILIO_ADC_H_
