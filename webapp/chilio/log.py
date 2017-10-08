import logging

# Application root logger
mainlog = logging.getLogger('chilio')
mainlog.setLevel(logging.INFO)

# Connect handlers
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s %(name)s %(levelname)s: %(message)s')
ch.setFormatter(formatter)
mainlog.addHandler(ch)

DEBUG = logging.DEBUG
INFO = logging.INFO
WARN = logging.WARN
ERR = logging.ERROR
CRIT = logging.CRITICAL

def get_logger(name):
    return logging.getLogger('chilio.' + name)
