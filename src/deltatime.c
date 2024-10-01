#include "deltatime.h"
#include <time.h>

unsigned long get_microtime() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000000 + t.tv_nsec / 1000;
}

void microsleep(unsigned long d) {
    struct timespec t = {
        .tv_sec = d / 1000000,
        .tv_nsec = (d % 1000000) * 1000
    };
    nanosleep(&t, NULL);
}
