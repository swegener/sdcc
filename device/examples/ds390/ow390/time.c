#include "time.h"

// let's just pretend it's 01 jan 1970 00.00:00 for now

time_t time(time_t *t) {
  *t=0;
  return *t;
}

struct tm lastTime;

struct tm *localtime(const time_t *timep) {
  timep; // hush the compiler
  lastTime.tm_sec=0;
  lastTime.tm_min=0;
  lastTime.tm_hour=0;
  lastTime.tm_mday=1;
  lastTime.tm_mon=0;
  lastTime.tm_year=101;
  lastTime.tm_wday=0;
  lastTime.tm_yday=0;
  lastTime.tm_isdst=0;
  return &lastTime;
}
