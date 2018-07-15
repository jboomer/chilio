#ifndef CHILIO_DEFS_H
#define CHILIO_DEFS_H

#define MAX_CHANNELS 8

typedef enum cerr {
    CHILIO_ERR_OK,
    CHILIO_ERR_OUTOFRANGE,
    CHILIO_ERR_EGAIN
} CErr;

#endif // #ifndef CHILIO_DEFS_H
