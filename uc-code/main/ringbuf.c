#include <string.h>
#include <stdbool.h>

#define NDATAPOINTS 64

struct ringbuf {
    float values[NDATAPOINTS];
    int index;
    bool isfull; 
};

void ringbuf_init(struct ringbuf* rb) 
{
    memset(rb->values, 0x00, sizeof(float) * NDATAPOINTS);
    rb->index = -1;
    rb->isfull = false;
}

int ringbuf_insert(struct ringbuf* rb, float val) 
{
    int ret = 0;
    rb->index++;

    if (rb->index >= NDATAPOINTS) {
         rb->index = 0;
         rb->isfull = true;
    }

    if (rb->index >= 0 && rb->index < NDATAPOINTS) {
        rb->values[rb->index] = val;
    } else {
        ret = -1;
    }

   return ret;
}
