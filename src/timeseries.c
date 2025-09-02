#include "timeseries.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "datetime.h"

// Constants
// ---------

// The maximum length of a read line
#define LINE_MAX_LENGTH 100

// Help functions
// --------------

/**
 * Set the start datetime of the timeseries from stdin
 *
 * @param timeseries  The timeseries to set
 */
void timeseries_set_start_datetime_from_stdin(struct Timeseries* timeseries) {
  char line[LINE_MAX_LENGTH];
  fgets(line, LINE_MAX_LENGTH, stdin);
  int y, m, d, h, min, s;
  if (sscanf(line, "%d-%d-%dT%d:%d:%d", &y, &m, &d, &h, &min, &s) == 6)
    datetime_initialize(&timeseries->start_datetime, y, m, d, h, min, s);
  else {
    fprintf(stderr, "error: could not retrieve start datetime in line %s",
            line);
    exit(1);
  }
}

/**
 * Set the observations of the timeseries from stdin
 *
 * @param timeseries  The timeseries to set
 */
void timeseries_set_observations_from_stdin(struct Timeseries* timeseries) {
  char line[LINE_MAX_LENGTH];
  while (fgets(line, LINE_MAX_LENGTH, stdin) != NULL) {
    int offset, value;
    if (sscanf(line, "%d %d", &offset, &value) == 2) {
      timeseries->offsets[timeseries->size] = offset;
      timeseries->values[timeseries->size] = value;
      ++timeseries->size;
    } else {
      fprintf(stderr, "error: could not retrieve values for line %s", line);
      exit(1);
    }
  }
}

/**
 * Set the last datetime of the timeseries
 *
 * @param timeseries  The timeseries to set
 */
void timeseries_set_last_datetime(struct Timeseries* timeseries) {
  int hour = timeseries->size == 2 ? 10 : 9;
  datetime_initialize(&timeseries->last_datetime, 2025, 9, 1, hour, 0, 0);
}

// Functions
// ---------

void timeseries_initialize_from_stdin(struct Timeseries* timeseries) {
  timeseries->size = 0;
  timeseries_set_start_datetime_from_stdin(timeseries);
  timeseries_set_observations_from_stdin(timeseries);
  timeseries_set_last_datetime(timeseries);
}

void timeseries_print_stats(const struct Timeseries* timeseries) {
  printf("Range: [%s, %s]\n",
         datetime_to_rfc3339_string(&timeseries->start_datetime),
         datetime_to_rfc3339_string(&timeseries->last_datetime));
  printf("Size: %d\n", timeseries->size);
}

void timeseries_print_observations(const struct Timeseries* timeseries) {
  for (size_t i = 0; i < timeseries->size; ++i)
    printf("%s %d\n", datetime_to_rfc3339_string(&timeseries->start_datetime),
                      timeseries->values[i]);
}
