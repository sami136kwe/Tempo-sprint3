#include "timeseries.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"
#include "validation.h"

// Constants
// ---------

// The maximum length of a read line
#define LINE_MAX_LENGTH 100

// Help functions
// --------------

/**
 * Checks if the timeseries ran out of memory
 *
 * @param timeseries  The timeseries to check
 */
void check_out_of_memory(const struct Timeseries* timeseries) {
  if (timeseries->offsets == NULL || timeseries->values == NULL)
    report_out_of_memory();
}

/**
 * Set the start datetime of the timeseries from stdin
 *
 * @param timeseries  The timeseries to set
 */
void timeseries_set_start_datetime_from_stdin(struct Timeseries* timeseries) {
  char line[LINE_MAX_LENGTH];
  fgets(line, LINE_MAX_LENGTH, stdin);
  int y, m, d, h, min, s;
  if (sscanf(line, "%4d-%2d-%2dT%2d:%2d:%2d", &y, &m, &d, &h, &min, &s) == 6)
    datetime_initialize(&timeseries->start_datetime, y, m, d, h, min, s);
  else
    report_invalid_datetime_format();
}

/**
 * Returns the index of an offset in a given timeseries
 *
 * If the offset does not belong to the timerseries, returns -1.
 *
 * @param timeseries  The timeseries to update
 * @param offset      The searched offset
 * @return            The index of the offset or -1
 */
int timeseries_offset_index(const struct Timeseries* timeseries,
                            int offset) {
  size_t i = 0;
  while (i < timeseries->size) {
    if (offset == timeseries->offsets[i])
      return (int)i;
    else if (offset < timeseries->offsets[i])
      return -1;
    ++i;
  }
  return -1;
}

/**
 * Set the observations of the timeseries from stdin
 *
 * @param timeseries  The timeseries to set
 */
void timeseries_set_observations_from_stdin(struct Timeseries* timeseries) {
  char line[LINE_MAX_LENGTH];
  while (fgets(line, LINE_MAX_LENGTH, stdin) != NULL) {
    line[strcspn(line, "\n")] = '\0';
    int offset, value;
    char trailing;
    int num_fields = sscanf(line, "%d %d%c", &offset, &value, &trailing);
    if (num_fields != 2)
      report_invalid_observation_format(line);
    else if (offset < 0)
      report_invalid_offset(line);
    else
      timeseries_add_observation(timeseries, offset, value);
  }
}

/**
 * Set the last datetime of the timeseries
 *
 * @param timeseries  The timeseries to set
 */
void timeseries_set_last_datetime(struct Timeseries* timeseries) {
  int max_offset = 0;
  for (size_t i = 0; i < timeseries->size; ++i)
    max_offset = timeseries->offsets[i] > max_offset ?
      timeseries->offsets[i] : max_offset;
  timeseries->last_datetime = datetime_copy(&timeseries->start_datetime);
  datetime_add_seconds(&timeseries->last_datetime, max_offset);
}

// Functions
// ---------

void timeseries_initialize(struct Timeseries* timeseries) {
  timeseries->size = 0;
  timeseries->capacity = 1;
  timeseries->offsets = malloc(sizeof(int));
  timeseries->values = malloc(sizeof(int));
  check_out_of_memory(timeseries);
}

void timeseries_initialize_from_stdin(struct Timeseries* timeseries) {
  timeseries_initialize(timeseries);
  timeseries_set_start_datetime_from_stdin(timeseries);
  timeseries_set_observations_from_stdin(timeseries);
  timeseries_set_last_datetime(timeseries);
}

void timeseries_add_observation(struct Timeseries* timeseries,
                                int offset,
                                int value) {
  int idx = timeseries_offset_index(timeseries, offset);
  size_t i;
  if (idx == -1) {
    if (timeseries->size == timeseries->capacity) {
      timeseries->capacity *= 2;
      timeseries->offsets = realloc(timeseries->offsets,
                                    timeseries->capacity * sizeof(int));
      timeseries->values = realloc(timeseries->values,
                                   timeseries->capacity * sizeof(int));
      check_out_of_memory(timeseries);
    }
    i = timeseries->size;
    while (i > 0 && offset < timeseries->offsets[i - 1]) {
      timeseries->offsets[i] = timeseries->offsets[i - 1];
      timeseries->values[i] = timeseries->values[i - 1];
      --i;
    }
    ++timeseries->size;
  } else {
    i = (size_t)idx;
  }
  timeseries->offsets[i] = offset;
  timeseries->values[i] = value;
}

void timeseries_delete(struct Timeseries* timeseries) {
  datetime_delete(&timeseries->start_datetime);
  datetime_delete(&timeseries->last_datetime);
  free(timeseries->offsets);
  free(timeseries->values);
}

int timeseries_min_value(const struct Timeseries* timeseries) {
  int m = INT_MAX;
  for (size_t i = 0; i < timeseries->size; ++i)
    m = timeseries->values[i] < m ? timeseries->values[i] : m;
  return m;
}

int timeseries_max_value(const struct Timeseries* timeseries) {
  int m = INT_MIN;
  for (size_t i = 0; i < timeseries->size; ++i)
    m = timeseries->values[i] > m ? timeseries->values[i] : m;
  return m;
}

unsigned int timeseries_duration(const struct Timeseries* timeseries) {
  return (unsigned int)datetime_diff(&timeseries->start_datetime,
                                     &timeseries->last_datetime);
}

unsigned int timeseries_amplitude(const struct Timeseries* timeseries) {
  return timeseries->size == 0 ? 0
    : (unsigned int)(timeseries_max_value(timeseries) -
                     timeseries_min_value(timeseries));
}

int timeseries_interpolation(const struct Timeseries* timeseries,
                             const struct Datetime* datetime) {
  if (datetime_compare(datetime, &timeseries->start_datetime) < 0 ||
      datetime_compare(&timeseries->last_datetime, datetime) < 0)
    report_cannot_interpolate_outside_of_range();
  int offset = datetime_diff(&timeseries->start_datetime, datetime);
  size_t i = 0;
  while (timeseries->offsets[i] < offset)
    ++i;
  if (timeseries->offsets[i] == offset)
    return timeseries->values[i];
  double p = (double)(offset - timeseries->offsets[i - 1])
             /
             (double)(timeseries->offsets[i] - timeseries->offsets[i - 1]);
  return (int)((1.0 - p) * timeseries->values[i - 1] +
               p * timeseries->values[i]);
}

void timeseries_print_stats(const struct Timeseries* timeseries) {
  printf("Domain: [%s, %s]\n",
         datetime_to_rfc3339_string(&timeseries->start_datetime),
         datetime_to_rfc3339_string(&timeseries->last_datetime));
  if (timeseries->size == 0)
    printf("Codomain: none\n");
  else
    printf("Codomain: [%d, %d]\n", timeseries_min_value(timeseries),
                                   timeseries_max_value(timeseries));
  printf("Size: %d\n", timeseries->size);
  printf("Duration: %d\n", timeseries_duration(timeseries));
  printf("Amplitude: %d\n", timeseries_amplitude(timeseries));
}

void timeseries_print_interpolations(const struct Timeseries* timeseries,
                                     unsigned int step) {
  if (timeseries->size == 0)
    report_cannot_interpolate_empty_timeseries();
  struct Datetime datetime = datetime_copy(&timeseries->start_datetime);
  do {
    printf("%s %d\n", datetime_to_rfc3339_string(&datetime),
                      timeseries_interpolation(timeseries, &datetime));
    datetime_add_seconds(&datetime, (int)step);
  } while (datetime_compare(&datetime, &timeseries->last_datetime) <= 0);
}

void timeseries_print_observations(const struct Timeseries* timeseries) {
  for (size_t i = 0; i < timeseries->size; ++i) {
    struct Datetime datetime = datetime_copy(&timeseries->start_datetime);
    datetime_add_seconds(&datetime, timeseries->offsets[i]);
    printf("%s %d\n", datetime_to_rfc3339_string(&datetime),
                      timeseries->values[i]);
    datetime_delete(&datetime);
  }
}

void timeseries_print_gnuplot_script(const struct Timeseries* timeseries) {
  printf("#!/usr/bin/gnuplot\n");
  printf("# Data\n");
  printf("$data << EOD\n");
  
  for (size_t i = 0; i < timeseries->size; ++i) {
    struct Datetime datetime = datetime_copy(&timeseries->start_datetime);
    datetime_add_seconds(&datetime, timeseries->offsets[i]);
    printf("%s %d\n", datetime_to_rfc3339_string(&datetime),
           timeseries->values[i]);
    datetime_delete(&datetime);
  }
  
  printf("EOD\n");
  printf("# General settings\n");
  printf("set terminal png\n");
  printf("set xlabel \"Time\" offset 0,-1\n");
  printf("set xdata time\n");
  printf("set timefmt \"%%Y-%%m-%%dT%%H:%%M:%%S\"\n");
  printf("set ylabel \"Value\"\n");
  printf("set nokey\n");
  printf("set output \"timeseries.png\"\n");
  printf("# Plot times\n");
  printf("plot $data using 1:2 ps 2.0 pt 7 lc \"orange\" title \"0.9\" with points\n");
}
