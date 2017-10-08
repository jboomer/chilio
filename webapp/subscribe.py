#! /usr/bin/env python 

import chilio.log
from chilio.config import Config
import paho.mqtt.client as mqtt
import json
import sqlite3
import time

MYIP = '192.168.2.4'

log = chilio.log.get_logger('subscribe')
log.setLevel(chilio.log.DEBUG)

DB_URI = Config.DATABASE_URI
TABLENAME = 'sensorvals'

conn = sqlite3.connect(DB_URI)

def on_connect(client, userdata, flags, rc):
    log.debug('Connect with RC: %d', rc)

    client.subscribe('chilio/#')

def on_message(client, userdata, msg):
    log.info('Got [{topic}] : {msg}'.format(
        topic=msg.topic, msg=str(msg.payload)))

    tpclist = msg.topic.split('/')

    # Topics are of the form chilio/<SUBJECT>/[EXTRA]/[PARAMS]
    # Extra parameters are optional
    if len(tpclist) >= 2 and tpclist[0] == 'chilio':
        subject = tpclist[1]
        extra_params = tpclist[2:]
        handle_chilio_message(subject, extra_params, msg.payload)


def handle_chilio_message(subject, extra_params, msgraw):
    log.debug('Got subject %s', subject)

    if subject == 'sensorvals':

        try:
            msg = json.loads(msgraw)

            sensorid = msg['id']
            value    = msg['value']

            log_sensor_value(sensorid, value)

        except (ValueError, TypeError):
            log.error('Could not deserialize data')
        except KeyError:
            log.error('Missing fields in message')
    else:
        log.error('Unknown subject %s', subject)


def log_sensor_value(sensorid, value):
    timestamp = int(time.time())
    log.debug('Logging sensor id %d value %d timestamp %d'
            , sensorid, value, timestamp)
    conn.execute('INSERT INTO ' + TABLENAME + ' VALUES (?,?,?)'
                 , (sensorid , timestamp , value))
    conn.commit()


def main():
    log.debug('Starting subscribe')

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    
    client.connect(MYIP, 1883, 60)

    client.loop_forever()

if __name__ == '__main__':
    main()
