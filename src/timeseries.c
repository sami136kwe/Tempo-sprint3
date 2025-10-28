#include "timeseries.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  if (sscanf(line, "%4d-%2d-%2dT%2d:%2d:%2d", &y, &m, &d, &h, &min, &s) == 6)
    datetime_initialize(&timeseries->start_datetime, y, m, d, h, min, s);
  else {
    fprintf(stderr,
            "error: invalid datetime format (should be YYYY-mm-DDTHH:MM:SS)");
    exit(2);
  }
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
 * Adds an observation to a timeseries
 *
 * If the timeseries already has an observation with the same offset, the
 * associated values is overwritten.
 *
 * @param timeseries  The timeseries to update
 * @param offset      The offset of the observation
 * @param value       The value of the observation
 */
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
    if (num_fields != 2) {
      fprintf(stderr, "error: invalid observation format (%s)", line);
      exit(2);
    } else if (offset < 0) {
      fprintf(stderr, "error: invalid offset (%s)", line);
      exit(2);
    } else {
      timeseries_add_observation(timeseries, offset, value);
    }
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
}

void timeseries_initialize_from_stdin(struct Timeseries* timeseries) {
  timeseries_initialize(timeseries);
  timeseries_set_start_datetime_from_stdin(timeseries);
  timeseries_set_observations_from_stdin(timeseries);
  timeseries_set_last_datetime(timeseries);
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

void timeseries_print_observations(const struct Timeseries* timeseries) {
  for (size_t i = 0; i < timeseries->size; ++i) {
    struct Datetime datetime = datetime_copy(&timeseries->start_datetime);
    datetime_add_seconds(&datetime, timeseries->offsets[i]);
    printf("%s %d\n", datetime_to_rfc3339_string(&datetime),
                      timeseries->values[i]);
    datetime_delete(&datetime);
  }
}
