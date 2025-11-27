#ifndef TIMESERIES_H
#define TIMESERIES_H

#include "datetime.h"

// Constants
// ---------

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
  // The capacity of the timeseries
  unsigned int capacity;
  // The offsets of the observations
  int* offsets;
  // The observed values
  int* values;
};

// Functions
// ---------

/**
 * Initializes an empty timeseries
 *
 * @param timeseries  The timeseries to initialize
 */
void timeseries_initialize(struct Timeseries* timeseries);

/**
 * Initializes an empty timeseries
 *
 * @param timeseries  The timeseries to initialize
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
 * @return            The maximum value
 */
int timeseries_max_value(const struct Timeseries* timeseries);

/**
 * Returns the duration in seconds of the timeseries
 *
 * Note: If the timeseries is empty, returns 0.
 *
 * @param timeseries  The queried timeseries
 * @return            The duration
 */
unsigned int timeseries_duration(const struct Timeseries* timeseries);

/**
 * Returns the amplitude of the timeseries
 *
 * Note: If the timeseries is empty, returns 0.
 *
 * @param timeseries  The queried timeseries
 * @return            The amplitude
 */
unsigned int timeseries_amplitude(const struct Timeseries* timeseries);

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
