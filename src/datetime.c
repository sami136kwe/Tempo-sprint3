#include "datetime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/**
 * Reports a memory problem with a given context
 *
 * @param context  The context of the memory problem
 */
void report_memory_problem(const char* context) {
  fprintf(stderr, "error: memory allocation failed %s\n", context);
  exit(1);
}

void datetime_initialize(struct Datetime* datetime,
                         int year, int month, int day,
                         int hour, int min, int sec) {
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

struct Datetime datetime_copy(const struct Datetime* datetime) {
  struct Datetime copy;
  copy.t = datetime->t;
  copy.rfc3339_string = malloc((DATETIME_LENGTH + 1) * sizeof(char));
  if (copy.rfc3339_string == NULL)
    report_memory_problem("when trying to initialize copy->rfc3339_string");
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
      fprintf(stderr, "error: could not convert timestamp %ld to struct tm\n",
              datetime->t);
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
