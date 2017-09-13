import datetime
from chilio.app import app
from flask import render_template

@app.route('/')
@app.route('/index')
def index():
    # Mock values
    samples = [
        { 'time' : '13:06:34'
          , 'value' : 1023}
      , { 'time' : '13:07:27'
          , 'value' : 1019}
      , { 'time' : '13:09:19'
          , 'value' : 1012}
    ]

    return render_template('index.html', samples=samples)
