#! /usr/bin/env python

from pathlib import Path
import os
import sqlite3
import argparse
import time

_directory = os.path.join(str(Path.home()), '.chilio')
_db_file = os.path.join(_directory, 'db.sqlite')

def create():

    if not os.path.isdir(_directory):
        print('Creating directory {dir}'.format(dir=_directory))
        os.mkdir(_directory)
    else:
        print('Directory already exists')

    if not os.path.isfile(_db_file):
        print('Creating database {db}'.format(db=_db_file))
        conn = sqlite3.connect(_db_file)
        conn.execute('''CREATE TABLE sensorvals
                (SENSORID INT NOT NULL, 
                TIME INT NOT NULL, 
                VALUE INT NOT NULL)''')

        conn.execute('''CREATE TABLE sensors
                (SENSORID INT NOT NULL, 
                NAME STRING NOT NULL)''')
        
        conn.commit()

        conn.close()
    else:
        print('Database file already exists')

def clear_db():
    if not os.path.isdir(_directory) or not os.path.isfile(_db_file):
        print('DB does not exist')
        return
    
    conn = sqlite3.connect(_db_file)
    conn.execute('''DELETE FROM sensorvals''')
    conn.execute('''DELETE FROM sensors''')
    conn.commit()
    conn.close()


def test_vals():
    if not os.path.isdir(_directory) or not os.path.isfile(_db_file):
        create()
    else:
        clear_db()

    values = [(0, int(time.time() - 10 * 60), 800)
            , (0, int(time.time() -  9 * 60), 700)
            , (0, int(time.time() -  7 * 60), 650)
            , (0, int(time.time() -  6 * 60), 600)
            , (0, int(time.time() -  5 * 60), 400)
            , (0, int(time.time() -  4 * 60), 900)
            , (0, int(time.time() -  2 * 60), 800)
            , (0, int(time.time() -  1 * 60), 700)

            , (1, int(time.time() -  4 * 60), 880)
            , (1, int(time.time() -  3 * 60), 800)
            , (1, int(time.time() -  2 * 60), 700)
            , (1, int(time.time() -  1 * 60), 650)
            , (1, int(time.time() -  0 * 60), 620)]

    sensors = [(0, 'chili'), (1, 'tomato')]

    conn = sqlite3.connect(_db_file)

    for entry in values:
        print('Inserting {}'.format(entry))
        conn.execute('''INSERT INTO sensorvals VALUES (?,?,?)''', entry)

    for entry in sensors:
        print('Inserting {}'.format(entry))
        conn.execute('''INSERT INTO sensors VALUES (?,?)''', entry)

    conn.commit()
    conn.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('command', action='store', choices=['create', 'clear', 'testvals'])

    args = parser.parse_args()

    if args.command == 'create':
        create()
    elif args.command == 'clear':
        clear_db()
    elif args.command == 'testvals':
        test_vals()
    else:
        print('Unknown command : {}'.format(args.command))
