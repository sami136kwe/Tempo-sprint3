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
  // The 'time_t' representation of the datetime
  time_t t;
  // The RFC3339 string representation of the datetime
  char* rfc3339_string;
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
void datetime_initialize(struct Datetime* datetime,
                         int year, int month, int day,
                         int hour, int min, int sec);

/**
 * Compares two datetimes chronologically
 *
 * The function returns
 *
 * - -1 if the first datetime occurs before the second datetime
 * - 0 if the datetimes are the same
 * - 1 if the first datetime occurs after the second datetime
 */
int datetime_compare(const struct Datetime* datetime1,
                     const struct Datetime* datetime2);

/**
 * Copies a datetime
 *
 * Note: the returned datetime must be deleted with datetime_delete when it is
 * no longer needed.
 *
 * @param datetime   The datetime to copy
 * @return           The copy
 */
struct Datetime datetime_copy(const struct Datetime* datetime);

/**
 * Deletes a datetime
 *
 * @param datetime  The datetime to delete
 */
void datetime_delete(struct Datetime* datetime);

/**
 * Returns a RFC3339 string representation of the datetime
 *
 * @param s   The string in which the datetime will be stored
 * @param tm  The tm insance
 */
const char* datetime_to_rfc3339_string(const struct Datetime* datetime);

/**
 * Returns the time difference between two datetimes
 *
 * Note: TODO what if the second datetime occurs before the first?
 */
int datetime_diff(const struct Datetime* datetime1,
                  const struct Datetime* datetime2);

/**
 * Adds a given number of seconds to a datetime
 *
 * @param datetime  The datetime to modify
 * @param seconds   The number of seconds to add
 */
void datetime_add_seconds(struct Datetime* datetime, int seconds);

#endif
