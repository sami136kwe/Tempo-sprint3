#include <tap.h>

#include "datetime.h"

/**
 * Tests the datetime_compare function
 */
void test_datetime_compare(void) {
  diag("Testing datetime_compare");

  // Setup
  struct Datetime dt, dt_y, dt_m, dt_d, dt_h, dt_min, dt_s;
  datetime_initialize(&dt, 2025, 11, 1, 9, 0, 0);
  datetime_initialize(&dt_y, 2026, 11, 1, 9, 0, 0);
  datetime_initialize(&dt_m, 2025, 12, 1, 9, 0, 0);
  datetime_initialize(&dt_d, 2025, 11, 2, 9, 0, 0);
  datetime_initialize(&dt_h, 2025, 11, 1, 10, 0, 0);
  datetime_initialize(&dt_min, 2025, 11, 1, 9, 1, 0);
  datetime_initialize(&dt_s, 2025, 11, 1, 9, 0, 1);

  // Checks
  cmp_ok(datetime_compare(&dt, &dt_y), "<", 0,
         "2025-11-01T09:00:00 < 2026-11-01T09:00:00");
  cmp_ok(datetime_compare(&dt, &dt_m), "<", 0,
         "2025-11-01T09:00:00 < 2025-12-01T09:00:00");
  cmp_ok(datetime_compare(&dt, &dt_d), "<", 0,
         "2025-11-01T09:00:00 < 2025-11-02T09:00:00");
  cmp_ok(datetime_compare(&dt, &dt_h), "<", 0,
         "2025-11-01T09:00:00 < 2025-11-01T10:00:00");
  cmp_ok(datetime_compare(&dt, &dt_min), "<", 0,
         "2025-11-01T09:00:00 < 2025-11-01T09:01:00");
  cmp_ok(datetime_compare(&dt, &dt_s), "<", 0,
         "2025-11-01T09:00:00 < 2025-11-01T09:00:01");

  // Teardown
  datetime_delete(&dt);
  datetime_delete(&dt_y);
  datetime_delete(&dt_m);
  datetime_delete(&dt_d);
  datetime_delete(&dt_h);
  datetime_delete(&dt_min);
  datetime_delete(&dt_s);
}

/**
 * Tests the datetime_copy function
 */
void test_datetime_copy(void) {
  diag("Testing datetime_copy");

  // Setup
  struct Datetime dt1;
  datetime_initialize(&dt1, 2025, 11, 1, 9, 0, 0);
  struct Datetime dt2 = datetime_copy(&dt1);

  // Checks
  cmp_ok(datetime_compare(&dt1, &dt2), "==", 0,
         "The copy of a datetime is equal to the source datetime");

  // Teardown
  datetime_delete(&dt1);
  datetime_delete(&dt2);
}

/**
 * Tests the datetime_to_rfc3339_string function
 */
void test_datetime_to_rfc3339_string(void) {
  diag("Testing datetime_to_rfc3339_string");

  // Setup
  struct Datetime dt1, dt2;
  datetime_initialize(&dt1, 2025, 11, 1, 9, 0, 0);
  datetime_initialize(&dt2, 2000, 1, 1, 0, 0, 0);

  // Checks
  is(datetime_to_rfc3339_string(&dt1), "2025-11-01T09:00:00",
     "The string representation of 2025-11-01T09:00:00 is correct");
  is(datetime_to_rfc3339_string(&dt2), "2000-01-01T00:00:00",
     "The string representation of 2000-01-01T00:00:00 is correct");

  // Teardown
  datetime_delete(&dt1);
  datetime_delete(&dt2);
}

/**
 * Tests the datetime_to_rfc3339_string function
 */
void test_datetime_diff(void) {
  diag("Testing datetime_diff");

  // Setup
  struct Datetime dt, dt_y, dt_m, dt_d, dt_h, dt_min, dt_s;
  datetime_initialize(&dt, 2025, 11, 1, 9, 0, 0);
  datetime_initialize(&dt_y, 2026, 11, 1, 9, 0, 0);
  datetime_initialize(&dt_m, 2025, 12, 1, 9, 0, 0);
  datetime_initialize(&dt_d, 2025, 11, 2, 9, 0, 0);
  datetime_initialize(&dt_h, 2025, 11, 1, 10, 0, 0);
  datetime_initialize(&dt_min, 2025, 11, 1, 9, 1, 0);
  datetime_initialize(&dt_s, 2025, 11, 1, 9, 0, 1);
  int s_in_y = 365 * 24 * 60 * 60,
      s_in_m = 30 * 24 * 60 * 60,
      s_in_d = 24 * 60 * 60,
      s_in_h = 60 * 60;

  // Checks
  cmp_ok(datetime_diff(&dt, &dt_y), "==", s_in_y,
         "The difference between two dates separated by a year is correct");
  cmp_ok(datetime_diff(&dt, &dt_m), "==", s_in_m,
         "The difference between two dates separated by a month is correct");
  cmp_ok(datetime_diff(&dt, &dt_d), "==", s_in_d,
         "The difference between two dates separated by a day is correct");
  cmp_ok(datetime_diff(&dt, &dt_h), "==", s_in_h,
         "The difference between two dates separated by an hour is correct");
  cmp_ok(datetime_diff(&dt, &dt_min), "==", 60,
         "The difference between two dates separated by a minute is correct");
  cmp_ok(datetime_diff(&dt, &dt_s), "==", 1,
         "The difference between two dates separated by a second is correct");

  // Teardown
  datetime_delete(&dt);
  datetime_delete(&dt_y);
  datetime_delete(&dt_m);
  datetime_delete(&dt_d);
  datetime_delete(&dt_h);
  datetime_delete(&dt_min);
  datetime_delete(&dt_s);
}

/**
 * Tests the datetime_add_seconds function
 */
void test_datetime_add_seconds(void) {
  diag("Testing datetime_add_seconds");

  // Setup
  struct Datetime dt, dt_h;
  datetime_initialize(&dt, 2025, 11, 1, 9, 0, 0);
  int s_in_h = 60 * 60;
  datetime_add_seconds(&dt, s_in_h);
  datetime_initialize(&dt_h, 2025, 11, 1, 10, 0, 0);

  // Checks
  cmp_ok(datetime_compare(&dt, &dt_h), "==", 0,
         "Adding an hour to 2025-11-01T09:00:00 works");

  // Teardown
  datetime_delete(&dt);
  datetime_delete(&dt_h);
}

int main(void) {
  test_datetime_compare();
  test_datetime_copy();
  test_datetime_to_rfc3339_string();
  test_datetime_diff();
  test_datetime_add_seconds();
  done_testing();
}
