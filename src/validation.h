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
 * Reports that a conversion from time_t to struct tm failed
 *
 * @param t  The provided timestamp
 */
void report_time_conversion_error(time_t t);

// CLI
// ---

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
