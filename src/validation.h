#ifndef VALIDATION_H
#define VALIDATION_H

#include <time.h>

// Memory
// ------

/**
 * Reports a memory problem with a given context
 *
 * @param context  The context of the memory problem
 */
void report_memory_problem(const char* context);

/**
 * Reports that the timeseries module ran out of memory
 */
void report_out_of_memory(void);

// Datetimes
// ---------

/**
 * Reports that the given datetime fields form an invalid datetime
 *
 * @param year   The provided year
 * @param month  The provided month
 * @param day    The provided day
 * @param hour   The provided hour
 * @param min    The provided minutes
 * @param sec    The provided seconds
 */
void report_invalid_datetime(int year, int month, int day,
                             int hour, int min, int sec);

/**
 * Reports that the provided datetime string has invalid format
 */
void report_invalid_datetime_format(void);

/**
 * Reports that a conversion from time_t to struct tm failed
 *
 * @param t  The provided timestamp
 */
void report_time_conversion_error(time_t t);

// Timeseries
// ----------

/**
 * Reports that the provided string has an invalid observation format
 *
 * @param s  The provided string
 */
void report_invalid_observation_format(const char* s);

/**
 * Reports that the provided string contains an invalid offset
 *
 * @param s  The provided string
 */
void report_invalid_offset(const char* s);

/**
 * Reports that an empty timeseries cannot be interpolated
 */
void report_cannot_interpolate_empty_timeseries(void);

/**
 * Reports that the timeseries cannot be interpolated at a datetime outside of
 * range
 */
void report_cannot_interpolate_outside_of_range(void);

// CLI
// ---

/**
 * Reports that a subcommand is mandatory
 */
void report_subcommand_is_mandatory(void);

/**
 * Reports that an unrecognized subcommand has been provided
 *
 * @param subcommand  The provided subcommand
 */
void report_unrecognized_subcommand(const char* subcommand);

/**
 * Reports that too many arguments have been provided
 *
 * @param subcommand  The provided subcommand
 */
void report_too_many_arguments(const char* subcommand);

/**
 * Checks if too many arguments have been provided
 *
 * @param argc        The number of arguments
 * @param subcommand  The provided subcommand
 */
void check_if_too_many_arguments(int argc, const char* subcommand);

/**
 * Reports that some option was expected
 *
 * @param option  The expected option
 */
void report_wrong_option(const char* option);

/**
 * Reports that an invalid format duration was provided
 *
 * @param s  The string that was provided
 */
void report_invalid_format_duration(const char* s);

#endif
