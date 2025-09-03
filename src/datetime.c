#include "datetime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  datetime->tm = malloc(sizeof(struct tm));
  if (datetime->tm == NULL)
    report_memory_problem("when trying to initialize datetime->tm");
  datetime->tm->tm_year = year - 1900;
  datetime->tm->tm_mon = month - 1;
  datetime->tm->tm_mday = day;
  datetime->tm->tm_hour = hour;
  datetime->tm->tm_min = min;
  datetime->tm->tm_sec = sec;
  datetime->tm->tm_isdst = -1;
  datetime->t = mktime(datetime->tm);
  datetime->string = malloc(DATETIME_LENGTH * sizeof(char));
  if (datetime->string == NULL)
    report_memory_problem("when trying to initialize datetime->string");
  strcpy(datetime->string, "");
}

struct Datetime datetime_copy(const struct Datetime* datetime) {
  struct Datetime copy;
  copy.t = datetime->t;
  copy.tm = malloc(sizeof(struct tm));
  if (copy.tm == NULL)
    report_memory_problem("when trying to initialize datetime->tm");
  *copy.tm = *localtime(&copy.t);
  copy.string = malloc(DATETIME_LENGTH * sizeof(char));
  if (copy.string == NULL)
    report_memory_problem("when trying to initialize datetime->string");
  strcpy(copy.string, "");
  return copy;
}

void datetime_delete(struct Datetime* datetime) {
  free(datetime->tm);
  free(datetime->string);
}

const char* datetime_to_rfc3339_string(const struct Datetime* datetime) {
  if (strcmp(datetime->string, "") == 0)
    strftime(datetime->string, DATETIME_LENGTH + 1, "%Y-%m-%dT%H:%M:%S",
             datetime->tm);
  return datetime->string;
}

void datetime_add_seconds(struct Datetime* datetime, int seconds) {
  datetime->t += seconds;
  *datetime->tm = *localtime(&datetime->t);
  strcpy(datetime->string, "");
}
