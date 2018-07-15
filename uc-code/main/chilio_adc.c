#include "chilio_adc.h"
#include "chilio_mqtt.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "mcp3008.h"

#define N_DATAPOINTS 64

struct SensorReading {

    uint16_t          datapoints[N_DATAPOINTS];
    size_t            index;
    float             average;
    SemaphoreHandle_t avgMutex;
    bool              initialized;
    bool              update;

};
 

static const char* TAG = "chilio.adc";
static struct SensorReading m_sensorVals[MAX_CHANNELS];

static void add_datapoint(uint8_t channel, uint16_t val);
static uint16_t get_raw(uint8_t channel);
static void update_average(uint8_t channel);

void chilio_adc_init()
{
    adc1_config_width(ADC_WIDTH_10Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_11db);

    for (int i = 0; i < MAX_CHANNELS; i++) {
        m_sensorVals[i].avgMutex = xSemaphoreCreateMutex();

        if (! m_sensorVals[i].avgMutex) {
            ESP_LOGE(TAG, "Could not create mutex for channel %d\n", i);
        }
    }

}

CErr chilio_get_average(uint8_t channel, float* val)
{
   CErr ret = CHILIO_ERR_OK; 

   if (channel < MAX_CHANNELS)
   {
       if (m_sensorVals[channel].initialized) {
           xSemaphoreTake(m_sensorVals[channel].avgMutex, portMAX_DELAY);

           *val = m_sensorVals[channel].average;

           xSemaphoreGive(m_sensorVals[channel].avgMutex);
       } else {
           ret = CHILIO_ERR_EGAIN;
       }
        
   } else {
        ret = CHILIO_ERR_OUTOFRANGE;
   }

   return ret;
}

void chilio_adc_read_task(void* pvParameter)
{

    const TickType_t xTicksSecond = pdMS_TO_TICKS( 1000 );

    while(1) {
        uint8_t channel = 0;
        uint16_t val = get_raw(channel);

        add_datapoint(channel, val);

        ESP_LOGD(TAG, "Read value %d", val);

        if (m_sensorVals[channel].update) {

            update_average(channel);

            chilio_mqtt_publish_sensorval(channel
                                        , m_sensorVals[channel].average);

            m_sensorVals[channel].update = false;

        }

        vTaskDelay(xTicksSecond);
    }
}

static uint16_t get_raw(uint8_t channel)
{

    uint16_t ret = 0xFFFF;

    switch(channel) {
        case 0:
            ret = mcp3008_get_raw(channel);
        break;
        default:
            ESP_LOGE(TAG, "Invalid channel number %u\n", channel);
        break;            

    };

    return ret;
}


static void add_datapoint(uint8_t channel, uint16_t val)
{
    if (channel < MAX_CHANNELS) {
        struct SensorReading* sv = &m_sensorVals[channel];

        sv->datapoints[sv->index] = val;
        sv->index++;

        if (sv->index >= N_DATAPOINTS) {
            sv->update = true;
            sv->initialized = true;
            sv->index = 0;
        }
    }
}

static void update_average(uint8_t channel)
{
    //TODO: Mutex
    float average = 0;

    if (channel < MAX_CHANNELS) {
        for (int i = 0; i < N_DATAPOINTS; i++) {
            average += ((float)(m_sensorVals[channel].datapoints[i])) 
                        / N_DATAPOINTS;
        }
    }


    xSemaphoreTake(m_sensorVals[channel].avgMutex, portMAX_DELAY);

    m_sensorVals[channel].average = average;

    xSemaphoreGive(m_sensorVals[channel].avgMutex);
}
