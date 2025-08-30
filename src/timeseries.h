#ifndef TIMESERIES_H
#define TIMESERIES_H

// Constants
// ---------

// The length of a datetime
#define DATETIME_LENGTH 19
// The maximum size of a timeseries
#define MAX_SIZE 1000

// Types
// -----

// A timeseries
struct Timeseries {
  // The start datetime
  char start_datetime[DATETIME_LENGTH + 1];
  // The size of the timeseries
  unsigned int size;
  // The offsets of the observations
  int offsets[MAX_SIZE];
  // The observed values
  int values[MAX_SIZE];
};

// Functions
// ---------

/**
 * Initializes an empty timeseries
 */
void timeseries_initialize(struct Timeseries* timeseries);

/**
 * Set the start datetime of the timeseries from stdin
 *
 * @param timeseries  The timeseries to set
 */
void timeseries_set_start_datetime_from_stdin(struct Timeseries* timeseries);

/**
 * Set the observations of the timeseries from stdin
 *
 * @param timeseries  The timeseries to set
 */
void timeseries_set_observations_from_stdin(struct Timeseries* timeseries);

/**
 * Prints the statistics of a timeseries to stdout
 *
 * @param timeseries  The timeseries to print
 */
void timeseries_print_stats(const struct Timeseries* timeseries);

/**
 * Prints the observations of a timeseries to stdout
 *
 * @param timeseries  The timeseries to print
 */
void timeseries_print_observations(const struct Timeseries* timeseries);

#endif
