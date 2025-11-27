#ifndef VALIDATION_H
#define VALIDATION_H

#include <time.h>

/**
 * Reports a memory problem with a given context
 *
 * @param context  The context of the memory problem
 */
void report_memory_problem(const char* context);

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

#endif
