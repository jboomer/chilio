#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "driver/gpio.h"
#include "driver/adc.h"

#include "chilio_mqtt.h"


/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

static const char* TAG = "chilio.main";

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event);
static void initialise_wifi(void);
static void initialize_adc(void);
static void pub_adc_task(void* pvParameter);

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;




static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGD(TAG, "Got an IP: " IPSTR, IP2STR(&event->event_info.got_ip.ip_info.ip));
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        chilio_mqtt_start();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        chilio_mqtt_stop();
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void pub_adc_task(void* pvParameter)
{

    const TickType_t xTicksSecond = pdMS_TO_TICKS( 1000 );

    while(1) {
        int val = adc1_get_raw(ADC1_CHANNEL_6);

        ESP_LOGD(TAG, "Read value %d", val);

        chilio_mqtt_publish_sensorval(0, val);

        vTaskDelay(xTicksSecond);
    }
}

static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(wifi_event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASSWORD,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static void initialize_adc(void)
{
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_11db);
}

void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    initialize_adc();
    initialise_wifi();

    xTaskCreate(&pub_adc_task, "pub_adc_task", 4096, NULL, 3, NULL);
}
