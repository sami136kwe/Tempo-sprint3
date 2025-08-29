#include "timeseries.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Constants
// ---------

// The maximum length of a read line
#define LINE_MAX_LENGTH 100

// Functions
// ---------

void timeseries_initialize(struct Timeseries* timeseries) {
  strcpy(timeseries->start_datetime, "");
  timeseries->size = 0;
}

void timeseries_set_start_datetime_from_stdin(struct Timeseries* timeseries) {
  char line[LINE_MAX_LENGTH];
  if (fgets(line, LINE_MAX_LENGTH, stdin) != NULL) {
    strncpy(timeseries->start_datetime, line, DATETIME_LENGTH);
    timeseries->start_datetime[DATETIME_LENGTH] = '\0';
  }
}

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

void timeseries_print_stats(const struct Timeseries* timeseries) {
  printf("Range: [%s, %s]\n",
         timeseries->start_datetime, timeseries->start_datetime);
  printf("Size: %d\n", timeseries->size);
}

void timeseries_print_observations(const struct Timeseries* timeseries) {
  for (size_t i = 0; i < timeseries->size; ++i)
    printf("%s %d\n", timeseries->start_datetime, timeseries->values[i]);
}
