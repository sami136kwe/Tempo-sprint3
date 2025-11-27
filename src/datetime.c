#include "datetime.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "validation.h"

/**
 * Indicates if a given datetime is valid
 *
 * @param year   The year of the datetime
 * @param month  The month of the datetime
 * @param day    The day of the datetime
 * @param hour   The hour of the datetime
 * @param min    The minutes of the datetime
 * @param sec    The seconds of the datetime
 * @return       true if and only if the datetime is valid
 */
bool datetime_is_valid(int year, int month, int day,
                       int hour, int min, int sec) {
  if (year < 1900 || year > 3000 ||
      month < 1 || month > 12 ||
      day < 1)
    return false;
  switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      if (day > 31) return false;
      break;
    case 4: case 6: case 9: case 11:
      if (day > 30) return false;
      break;
    case 2:
      if (year % 4 != 0 || year % 400 != 0 || day > 28)
        return false;
      if (day > 29) return false;
      break;
  }
  if (hour < 0 || hour > 23 ||
      min < 0 || min > 59 ||
      sec < 0 || sec > 59)
    return false;
  return true;
}

/**
 * Checks if the given datetime fields form a valid datetime
 *
 * @param year   The year of the datetime
 * @param month  The month of the datetime
 * @param day    The day of the datetime
 * @param hour   The hour of the datetime
 * @param min    The minutes of the datetime
 * @param sec    The seconds of the datetime
 */
void datetime_validate(int year, int month, int day,
                       int hour, int min, int sec) {
  if (!datetime_is_valid(year, month, day, hour, min, sec))
    report_invalid_datetime(year, month, day, hour, min, sec);
}

void datetime_initialize(struct Datetime* datetime,
                         int year, int month, int day,
                         int hour, int min, int sec) {
  datetime_validate(year, month, day, hour, min, sec);
  struct tm tm = {0};
  tm.tm_year = year - 1900;
  tm.tm_mon = month - 1;
  tm.tm_mday = day;
  tm.tm_hour = hour;
  tm.tm_min = min;
  tm.tm_sec = sec;
  tm.tm_isdst = -1;
  datetime->t = utils_timegm(&tm);
  datetime->rfc3339_string = malloc((DATETIME_LENGTH + 1) * sizeof(char));
  if (datetime->rfc3339_string == NULL)
    report_memory_problem("when trying to initialize datetime->rfc3339_string");
  strcpy(datetime->rfc3339_string, "");
}

int datetime_compare(const struct Datetime* datetime1,
                     const struct Datetime* datetime2) {
  if (datetime1->t < datetime2->t)
    return -1;
  else if (datetime1->t > datetime2->t)
    return 1;
  else
    return 0;
}

struct Datetime datetime_copy(const struct Datetime* datetime) {
  struct Datetime copy;
  copy.t = datetime->t;
  copy.rfc3339_string = malloc((DATETIME_LENGTH + 1) * sizeof(char));
  if (copy.rfc3339_string == NULL)
    report_memory_problem("when trying to initialize copy->rfc3339_string");
  else
    strcpy(copy.rfc3339_string, "");
  return copy;
}

void datetime_delete(struct Datetime* datetime) {
  free(datetime->rfc3339_string);
}

const char* datetime_to_rfc3339_string(const struct Datetime* datetime) {
  if (strcmp(datetime->rfc3339_string, "") == 0) {
    struct tm* tm = gmtime(&datetime->t);
    if (tm == NULL)
      report_time_conversion_error(datetime->t);
    strftime(datetime->rfc3339_string, DATETIME_LENGTH + 1,
             "%Y-%m-%dT%H:%M:%S", tm);
  }
  return datetime->rfc3339_string;
}

int datetime_diff(const struct Datetime* datetime1,
                  const struct Datetime* datetime2) {
  return (int)difftime(datetime2->t, datetime1->t);
}

void datetime_add_seconds(struct Datetime* datetime, int seconds) {
  datetime->t += seconds;
  strcpy(datetime->rfc3339_string, "");
}
