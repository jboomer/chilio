#! /usr/bin/env python 
import chilio.log
import paho.mqtt.client as mqtt

MYIP = '192.168.2.4'

log = chilio.log.get_logger('client')
log.setLevel(chilio.log.DEBUG)

def on_connect(client, userdata, flags, rc):
    log.debug('Connect with RC: %d', rc)

    client.subscribe('chilio/#')

def on_message(client, userdata, msg):
    log.info('Got [{topic}] : {msg}'.format(
        topic=msg.topic, msg=str(msg.payload)))

def main():
    log.debug('Starting subscribe')

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    
    client.connect(MYIP, 1883, 60)

    client.loop_forever()


if __name__ == '__main__':
    main()
