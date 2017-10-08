from pathlib import Path
import os

class Config:
    DEBUG        = True
    DATABASE_URI = os.path.join(str(Path.home()), '.chilio', 'db.sqlite')
