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
 * Deletes a timeseries
 *
 * @param timeseries  The timeseries to delete
 */
void timeseries_delete(struct Timeseries* timeseries);

/**
 * Returns the minimum value of the timeseries
 *
 * Note: If the timeseries is empty, returns INT_MAX.
 *
 * @param timeseries  The queried timeseries
 * @return            The minimum value
 */
int timeseries_min_value(const struct Timeseries* timeseries);

/**
 * Returns the maximum value of the timeseries
 *
 * Note: If the timeseries is empty, returns INT_MIN.
 *
 * @param timeseries  The queried timeseries
 * @return            The minimum value
 */
int timeseries_max_value(const struct Timeseries* timeseries);

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
