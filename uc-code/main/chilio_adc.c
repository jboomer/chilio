#include "chilio_adc.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/adc.h"

static const char* TAG = "chilio.adc";

void chilio_adc_init()
{
    adc1_config_width(ADC_WIDTH_10Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_11db);

}

int chilio_adc_get_raw(uint8_t channel)
{
    int val = -1;

    switch(channel) {
        case 0:
            val = adc1_get_raw(ADC1_CHANNEL_6);
        break;
        default:
            ESP_LOGE(TAG, "Invalid channel number %u\n", channel);
        break;            

    };

    return val;
}
