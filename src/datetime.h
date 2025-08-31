#ifndef DATETIME_H
#define DATETIME_H

#include <time.h>

/**
 * Initializes a 'struct tm' instance from given fields
 *
 * @param tm     The struct to initialize
 * @param year   The year field
 * @param month  The month field
 * @param day    The day of month field
 * @param hour   The hour field
 * @param min    The minute field
 * @param sec    The second field
 */
void initialize_tm_from_fields(struct tm* tm, int year, int month, int day,
                               int hour, int min, int sec);

/**
 * Returns a RFC3339 datetime string from a 'struct tm' instance
 *
 * @param s   The string in which the datetime will be stored
 * @param tm  The tm insance
 */
void initialize_rfc3339_string_from_tm(char* s, const struct tm* tm);

#endif
