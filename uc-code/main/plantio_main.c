#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "driver/gpio.h"
#include "driver/adc.h"
#include "mqtt.h"


/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

static const char* TAG = "plantio.main";

/* MQTT callbacks */
static bool mqtt_connect_cb(mqtt_client* client
                          , mqtt_event_data_t* event_data);
static bool mqtt_disconnect_cb(mqtt_client* client
                               , mqtt_event_data_t* event_data);
static bool mqtt_subscribe_cb(mqtt_client* client
                              , mqtt_event_data_t* event_data);
static bool mqtt_publish_cb(mqtt_client* client
                            , mqtt_event_data_t* event_data);
static bool mqtt_data_cb(mqtt_client* client
                            , mqtt_event_data_t* event_data);

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event);
static void initialise_wifi(void);
static void initialize_adc(void);

static mqtt_settings MQTTSettings = {
    .host = "192.168.2.4",
#if defined(CONFIG_MQTT_SECURITY_ON)
    .port = 8883, // encrypted
#else
    .port = 1883, // unencrypted
#endif
    .client_id = "mqtt_client_id",
    .username = "user",
    .password = "pass",
    .clean_session = 0,
    .keepalive = 120,
    .lwt_topic = "/lwt",
    .lwt_msg = "offline",
    .lwt_qos = 0,
    .lwt_retain = 0,
    .connected_cb = mqtt_connect_cb,
    .disconnected_cb = mqtt_disconnect_cb,
    .subscribe_cb = mqtt_,
    .publish_cb = publish_cb,
    .data_cb = data_cb
};

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;

/* Constants that aren't configurable in menuconfig */
#define ADC1_TEST_CHANNEL (ADC1_CHANNEL_6)


static bool mqtt_connect_cb(mqtt_client* client
                          , mqtt_event_data_t* event_data)
{
}

static bool mqtt_disconnect_cb(mqtt_client* client
                               , mqtt_event_data_t* event_data)
{
}

static bool mqtt_subscribe_cb(mqtt_client* client
                              , mqtt_event_data_t* event_data)
{
}

static bool mqtt_publish_cb(mqtt_client* client
                            , mqtt_event_data_t* event_data)
{
}

static bool mqtt_data_cb(mqtt_client* client
                            , mqtt_event_data_t* event_data)
{
}




static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
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
    adc1_config_channel_atten(ADC1_TEST_CHANNEL, ADC_ATTEN_11db);
}


void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    initialize_adc();
    initialise_wifi();
}
