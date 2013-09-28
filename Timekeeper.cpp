#include <sys/time.h>
#include "Timekeeper.h"

Timekeeper::Timekeeper() {
    Timekeeper::start_time = getTime();
}

long long Timekeeper::getTime() {
    struct timeval t;
    gettimeofday(&t, 0);
    long long ans = t.tv_sec * 1000000 + t.tv_usec;
    return ans;
}
