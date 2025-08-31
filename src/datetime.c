#include "datetime.h"

#include <stdio.h>

void initialize_tm_from_fields(struct tm* tm, int year, int month, int day,
                               int hour, int min, int sec) {
  tm->tm_year = year;
  tm->tm_mon = month;
  tm->tm_mday = day;
  tm->tm_hour = hour;
  tm->tm_min = min;
  tm->tm_sec = sec;
  tm->tm_isdst = -1;
}

void initialize_rfc3339_string_from_tm(char* s, const struct tm* tm) {
  sprintf(s, "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d",
          tm->tm_year, tm->tm_mon, tm->tm_mday,
          tm->tm_hour, tm->tm_min, tm->tm_sec);
}
