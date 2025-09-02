#include "datetime.h"

#include <stdio.h>

void datetime_initialize(struct Datetime* datetime,
                         int year, int month, int day,
                         int hour, int min, int sec) {
  datetime->tm.tm_year = year;
  datetime->tm.tm_mon = month;
  datetime->tm.tm_mday = day;
  datetime->tm.tm_hour = hour;
  datetime->tm.tm_min = min;
  datetime->tm.tm_sec = sec;
  datetime->tm.tm_isdst = -1;
  sprintf(datetime->string, "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d",
          year, month, day, hour, min, sec);
}

const char* datetime_to_rfc3339_string(const struct Datetime* datetime) {
  return datetime->string;
}
