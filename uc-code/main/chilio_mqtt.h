

/**
 * Start the MQTT client
 */
void chilio_mqtt_start(void);

/**
 * Stop the MQTT client
 */
void chilio_mqtt_stop(void);

/**
 * Publish a sensor value if MQTT is connected
 *
 * @param sensorid
 * @param val
 */
void chilio_mqtt_publish_sensorval(int sensorid, int val);
