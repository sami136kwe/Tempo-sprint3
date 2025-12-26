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
                                int value);

/**
 * Deletes a timeseries
 *
 * @param timeseries  The timeseries to delete
 */
void timeseries_delete(struct Timeseries* timeseries);

/**
 * Adds an observation to a timeseries
 *
 * If the timeseries already has an observation with the same offset, the
 * associated value is overwritten.
 *
 * @param timeseries  The timeseries to update
 * @param offset      The offset of the observation
 * @param value       The value of the observation
 */
void timeseries_add_observation(struct Timeseries* timeseries,
                                int offset,
                                int value);

/**
 * Set the last datetime of the timeseries
 *
 * @param timeseries  The timeseries to set
 */
void timeseries_set_last_datetime(struct Timeseries* timeseries);

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
 * Returns the interpolation of the timeseries at the given datetime
 *
 * @param timeseries  The queried timeseries
 * @param datetime    The datetime
 * @return            The interpolation
 */
int timeseries_interpolation(const struct Timeseries* timeseries,
                             const struct Datetime* datetime);

/**
 * Prints the statistics of a timeseries to stdout
 *
 * @param timeseries  The timeseries to print
 */
void timeseries_print_stats(const struct Timeseries* timeseries);

/**
 * Prints the interpolations of a timeseries to stdout
 *
 * @param timeseries  The timeseries to print
 * @param step        The interpolation step (in seconds)
 */
void timeseries_print_interpolations(const struct Timeseries* timeseries,
                                     unsigned int step);

/**
 * Prints the observations of a timeseries to stdout
 *
 * @param timeseries  The timeseries to print
 */
void timeseries_print_observations(const struct Timeseries* timeseries);

#endif
