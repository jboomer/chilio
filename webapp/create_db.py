#! /usr/bin/env python

from pathlib import Path
import os
import sqlite3

def main():

    directory = os.path.join(str(Path.home()), '.chilio')

    if not os.path.isdir(directory):
        print('Creating directory {dir}'.format(dir=directory))
        os.mkdir(directory)
    else:
        print('Directory already exists')

    db_file = os.path.join(directory, 'db.sqlite')

    if not os.path.isfile(db_file):
        print('Creating database {db}'.format(db=db_file))
        conn = sqlite3.connect(db_file)
        conn.execute('''CREATE TABLE sensorvals
                (SENSORID INT NOT NULL, 
                TIME INT NOT NULL, 
                VALUE INT NOT NULL)''')
    else:
        print('Database file already exists')


if __name__ == '__main__':
    main()
