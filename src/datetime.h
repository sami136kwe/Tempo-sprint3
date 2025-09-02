#ifndef DATETIME_H
#define DATETIME_H

#include <time.h>

// Constants
// ---------

// The length of a datetime
#define DATETIME_LENGTH 19

// Types
// -----

struct Datetime {
  // The 'struct tm' representation of the datetime
  struct tm tm;
  // The string representation of the datetime
  char string[DATETIME_LENGTH + 1];
};

// Functions
// ---------

/**
 * Initializes a datetime
 *
 * @param datetime  The datetime to initialize
 * @param year      The year
 * @param month     The month
 * @param day       The day
 * @param hour      The hours
 * @param min       The minutes
 * @param sec       The seconds
 */
void datetime_initialize(struct Datetime* tm, int year, int month, int day,
                         int hour, int min, int sec);

/**
 * Returns a RFC3339 string representation of the datetime
 *
 * @param s   The string in which the datetime will be stored
 * @param tm  The tm insance
 */
const char* datetime_to_rfc3339_string(const struct Datetime* datetime);

#endif
