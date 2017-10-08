import datetime
import sqlite3
import time
from chilio.app import app
from flask import render_template
from flask import g as flask_global

def get_db():
    """ DB Connection singleton """
    db = getattr(flask_global, '_database', None)
    if db is None:
        db = flask_global._database = sqlite3.connect(
        app.config['DATABASE_URI'])
        db.row_factory = sqlite3.Row
    return db

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
    results = query_db('SELECT * FROM sensorvals;')
    samples = [{'tmilliseconds' : row['time'] * 1000
              , 'value' : row['value']
              , 'tstring' : time.strftime('%Y-%m-%d %H:%M:%S'
                                          , time.localtime(row['time']))}
                    for row in results]

    return render_template('index.html', samples=samples)
