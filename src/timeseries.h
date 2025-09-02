#ifndef TIMESERIES_H
#define TIMESERIES_H

#include "datetime.h"

// Constants
// ---------

// The maximum size of a timeseries
#define MAX_SIZE 1000

// Types
// -----

// A timeseries
struct Timeseries {
  // The start datetime of the timeseries
  struct Datetime start_datetime;
  // The last datetime of the timeseries
  struct Datetime last_datetime;
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
void timeseries_initialize_from_stdin(struct Timeseries* timeseries);

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
