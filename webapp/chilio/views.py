import datetime
import sqlite3
import time
import json
from chilio.app import app
from flask import render_template, jsonify, request
from flask import g as flask_global

def get_db():
    """ DB Connection singleton """
    db = getattr(flask_global, '_database', None)
    if db is None:
        db = flask_global._database = sqlite3.connect(
        app.config['DATABASE_URI'])
        db.row_factory = dict_factory
    return db

def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d

def query_db(query, args=(), one=False):
    """ Execute single query 
        
        :param query: SQL statement string
        :param args:  Variables
        :param one:   Set to True to receive only one row
    """
    cur = get_db().execute(query, args)
    rv = cur.fetchall()
    cur.close()
    return (rv[0] if rv else None) if one else rv

@app.teardown_appcontext
def close_connection(exception):
    db = getattr(flask_global, '_database', None)
    if db is not None:
        db.close()

@app.route('/')
@app.route('/index')
def index():
    results = query_db('SELECT time,value FROM sensorvals WHERE sensorid=0')

    return render_template('index.html', samples=results)

# API routes
@app.route('/_api/sensorvals/')
def get_sensorvals():
    # TODO: Get from the last 24 hours or something?

    sensorid = request.args.get('sensorid')

    results = query_db('SELECT * FROM sensorvals '
                        + 'WHERE sensorid=? '
                        + ' ORDER BY time', args=(sensorid,))
    return jsonify(results)

@app.route('/_api/sensors/')
def get_sensors():

    return jsonify(query_db('SELECT * FROM sensors'))
    
