#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "cJSON.h"
#include "esp_log.h"
#include "mqtt.h"

static const char* TAG = "chilio.mqtt";

/* MQTT callbacks */
static void mqtt_connect_cb(mqtt_client* client
                            , mqtt_event_data_t* event_data);
static void mqtt_disconnect_cb(mqtt_client* client
                               , mqtt_event_data_t* event_data);
static void mqtt_subscribe_cb(mqtt_client* client
                              , mqtt_event_data_t* event_data);
static void mqtt_publish_cb(mqtt_client* client
                            , mqtt_event_data_t* event_data);
static void mqtt_data_cb(mqtt_client* client
                         , mqtt_event_data_t* event_data);


/* Settings object, is not copied in mqtt component! */
static mqtt_settings m_settings = {
    .host = "192.168.2.19",
#if CONFIG_MQTT_SECURITY_ON
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
    .subscribe_cb = mqtt_subscribe_cb,
    .publish_cb = mqtt_publish_cb,
    .data_cb = mqtt_data_cb
};

// TODO: Should there be a mutex here?
static bool m_connected = false;
static mqtt_client* m_client;

void chilio_mqtt_start()
{
   m_client = mqtt_start(&m_settings); 
}

void chilio_mqtt_stop()
{
    mqtt_stop();
}

void chilio_mqtt_publish_sensorval(int sensorid, float val)
{

    if (m_connected) {
        cJSON* pubObj;

        pubObj = cJSON_CreateObject();
        cJSON_AddNumberToObject(pubObj, "id", sensorid);
        cJSON_AddNumberToObject(pubObj, "value", val);

        char* mesg = cJSON_Print(pubObj);

        ESP_LOGD(TAG, "Publish : %s\n", mesg);

        mqtt_publish(m_client, "chilio/sensorvals", mesg, strlen(mesg), 0, 0);

        free(mesg);
        cJSON_Delete(pubObj);
    }

}

static void mqtt_connect_cb(mqtt_client* client
                          , mqtt_event_data_t* event_data)
{
    ESP_LOGI(TAG, "MQTT Connect callback"); 
    
    m_connected = true;
}

static void mqtt_disconnect_cb(mqtt_client* client
                               , mqtt_event_data_t* event_data)
{
    ESP_LOGD(TAG, "MQTT Disconnect callback");

    m_connected = false;
}

static void mqtt_subscribe_cb(mqtt_client* client
                              , mqtt_event_data_t* event_data)
{
    ESP_LOGD(TAG, "Subscribed client %p", client);
}

static void mqtt_publish_cb(mqtt_client* client
                            , mqtt_event_data_t* event_data)
{
    ESP_LOGD(TAG, "MQTT Publish callback");
}

static void mqtt_data_cb(mqtt_client* client
                            , mqtt_event_data_t* event_data)
{
    ESP_LOGD(TAG, "MQTT Data callback");
}
