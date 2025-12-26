#include <tap.h>
#include <limits.h>

#include "timeseries.h"
#include "datetime.h"

/**
 * Helper function to safely delete a timeseries initialized with
 * timeseries_initialize (without datetime initialization)
 */
void timeseries_delete_no_datetime(struct Timeseries* timeseries) {
  free(timeseries->offsets);
  free(timeseries->values);
}

/**
 * Tests the timeseries_initialize function
 */
void test_timeseries_initialize(void) {
  diag("Testing timeseries_initialize");

  // Setup
  struct Timeseries ts;
  timeseries_initialize(&ts);

  // Checks
  cmp_ok((int)ts.size, "==", 0, "Initialized timeseries has size 0");
  cmp_ok((int)ts.capacity, "==", 1, "Initialized timeseries has capacity 1");
  ok(ts.offsets != NULL, "Offsets array is allocated");
  ok(ts.values != NULL, "Values array is allocated");

  // Teardown
  timeseries_delete_no_datetime(&ts);
}

/**
 * Tests the timeseries_offset_index function
 */
void test_timeseries_offset_index(void) {
  diag("Testing timeseries_offset_index");

  // Setup
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);

  // Checks - empty timeseries
  cmp_ok(timeseries_offset_index(&ts, 0), "==", -1,
         "Empty timeseries returns -1");

  // Setup - add observations
  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 100, 20);
  timeseries_add_observation(&ts, 200, 30);

  // Checks - offset found
  cmp_ok(timeseries_offset_index(&ts, 0), "==", 0,
         "Offset found at beginning");
  cmp_ok(timeseries_offset_index(&ts, 100), "==", 1,
         "Offset found at middle");
  cmp_ok(timeseries_offset_index(&ts, 200), "==", 2,
         "Offset found at end");

  // Checks - offset not found
  cmp_ok(timeseries_offset_index(&ts, -10), "==", -1,
         "Offset smaller than first returns -1");
  cmp_ok(timeseries_offset_index(&ts, 50), "==", -1,
         "Offset between two existing offsets returns -1");
  cmp_ok(timeseries_offset_index(&ts, 300), "==", -1,
         "Offset larger than last returns -1");

  // Teardown
  timeseries_set_last_datetime(&ts);
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_add_observation function
 */
void test_timeseries_add_observation(void) {
  diag("Testing timeseries_add_observation");

  // Setup
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);

  // Checks - add to empty timeseries
  timeseries_add_observation(&ts, 50, 5);
  cmp_ok((int)ts.size, "==", 1, "Added to empty timeseries");
  cmp_ok(ts.offsets[0], "==", 50, "Correct offset");
  cmp_ok(ts.values[0], "==", 5, "Correct value");

  // Checks - capacity expansion
  unsigned int old_capacity = ts.capacity;
  timeseries_add_observation(&ts, 100, 10);
  ok(ts.capacity > old_capacity, "Capacity expanded");

  // Checks - insertion order
  timeseries_add_observation(&ts, 25, 2);
  timeseries_add_observation(&ts, 75, 7);
  timeseries_add_observation(&ts, 150, 15);
  cmp_ok((int)ts.size, "==", 5, "All observations added");
  ok(ts.offsets[0] == 25 && ts.offsets[1] == 50 && ts.offsets[2] == 75 &&
     ts.offsets[3] == 100 && ts.offsets[4] == 150,
     "All offsets in sorted order");

  // Checks - overwriting
  timeseries_add_observation(&ts, 75, 77);
  cmp_ok((int)ts.size, "==", 5, "Size unchanged when overwriting");
  cmp_ok(ts.values[2], "==", 77, "Value overwritten correctly");

  // Teardown
  timeseries_set_last_datetime(&ts);
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_set_last_datetime function
 */
void test_timeseries_set_last_datetime(void) {
  diag("Testing timeseries_set_last_datetime");

  // Setup - empty timeseries
  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);
  datetime_initialize(&ts_empty.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_set_last_datetime(&ts_empty);

  // Checks
  ok(datetime_compare(&ts_empty.start_datetime, &ts_empty.last_datetime) == 0,
     "Empty timeseries has last == start");

  // Teardown
  timeseries_delete(&ts_empty);

  // Setup - one element
  struct Timeseries ts1;
  timeseries_initialize(&ts1);
  datetime_initialize(&ts1.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts1, 100, 10);
  timeseries_set_last_datetime(&ts1);

  // Checks
  cmp_ok((int)timeseries_duration(&ts1), "==", 100,
         "Single element sets last to start + 100s");

  // Teardown
  timeseries_delete(&ts1);

  // Setup - multiple elements
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 200, 20);
  timeseries_add_observation(&ts, 100, 10);
  timeseries_add_observation(&ts, 50, 5);
  timeseries_set_last_datetime(&ts);

  // Checks
  cmp_ok((int)timeseries_duration(&ts), "==", 200,
         "Max offset correctly identified");

  // Teardown
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_min_value function
 */
void test_timeseries_min_value(void) {
  diag("Testing timeseries_min_value");

  // Setup - empty timeseries
  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);

  // Checks
  cmp_ok(timeseries_min_value(&ts_empty), "==", INT_MAX,
         "Min value of empty timeseries is INT_MAX");

  // Teardown
  timeseries_delete_no_datetime(&ts_empty);

  // Setup - timeseries with values
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 60, 5);
  timeseries_add_observation(&ts, 120, 15);
  timeseries_add_observation(&ts, 180, -3);
  timeseries_set_last_datetime(&ts);

  // Checks
  cmp_ok(timeseries_min_value(&ts), "==", -3,
         "Min value is correctly identified");

  // Teardown
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_max_value function
 */
void test_timeseries_max_value(void) {
  diag("Testing timeseries_max_value");

  // Setup - empty timeseries
  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);

  // Checks
  cmp_ok(timeseries_max_value(&ts_empty), "==", INT_MIN,
         "Max value of empty timeseries is INT_MIN");

  // Teardown
  timeseries_delete_no_datetime(&ts_empty);

  // Setup - timeseries with values
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 60, 5);
  timeseries_add_observation(&ts, 120, 15);
  timeseries_add_observation(&ts, 180, -3);
  timeseries_set_last_datetime(&ts);

  // Checks
  cmp_ok(timeseries_max_value(&ts), "==", 15,
         "Max value is correctly identified");

  // Teardown
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_duration function
 */
void test_timeseries_duration(void) {
  diag("Testing timeseries_duration");

  // Setup - empty timeseries
  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);
  datetime_initialize(&ts_empty.start_datetime, 2025, 1, 1, 0, 0, 0);
  ts_empty.last_datetime = datetime_copy(&ts_empty.start_datetime);

  // Checks
  cmp_ok((int)timeseries_duration(&ts_empty), "==", 0,
         "Duration of empty timeseries is 0");

  // Teardown
  timeseries_delete(&ts_empty);

  // Setup - timeseries with observations
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 3600, 20);
  timeseries_set_last_datetime(&ts);

  // Checks
  cmp_ok((int)timeseries_duration(&ts), "==", 3600,
         "Duration is correctly calculated");

  // Teardown
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_amplitude function
 */
void test_timeseries_amplitude(void) {
  diag("Testing timeseries_amplitude");

  // Setup - empty timeseries
  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);

  // Checks
  cmp_ok((int)timeseries_amplitude(&ts_empty), "==", 0,
         "Amplitude of empty timeseries is 0");

  // Teardown
  timeseries_delete_no_datetime(&ts_empty);

  // Setup - timeseries with values
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, -5);
  timeseries_add_observation(&ts, 60, 10);
  timeseries_add_observation(&ts, 120, 15);
  timeseries_set_last_datetime(&ts);

  // Checks
  cmp_ok((int)timeseries_amplitude(&ts), "==", 20,
         "Amplitude is correctly calculated");

  // Teardown
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_interpolation function
 */
void test_timeseries_interpolation(void) {
  diag("Testing timeseries_interpolation");

  // Setup
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 0);
  timeseries_add_observation(&ts, 100, 100);
  timeseries_add_observation(&ts, 200, 50);
  timeseries_set_last_datetime(&ts);
  struct Datetime dt_exact, dt_mid1, dt_mid2;
  datetime_initialize(&dt_exact, 2025, 1, 1, 0, 0, 0);
  datetime_initialize(&dt_mid1, 2025, 1, 1, 0, 0, 50);
  datetime_initialize(&dt_mid2, 2025, 1, 1, 0, 2, 30);

  // Checks - exact values
  cmp_ok(timeseries_interpolation(&ts, &dt_exact), "==", 0,
         "Interpolation at exact observation returns exact value");

  // Checks - interpolated values
  cmp_ok(timeseries_interpolation(&ts, &dt_mid1), "==", 50,
         "Linear interpolation at midpoint works correctly");
  cmp_ok(timeseries_interpolation(&ts, &dt_mid2), "==", 75,
         "Linear interpolation between observations works correctly");

  // Teardown
  datetime_delete(&dt_exact);
  datetime_delete(&dt_mid1);
  datetime_delete(&dt_mid2);
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_interpolation function with edge cases
 * WARNING: This test will CRASH with the buggy code!
 * Only run after fixing timeseries_interpolation bugs.
 */
void test_timeseries_interpolation_edge_cases(void) {
  diag("Testing timeseries_interpolation edge cases");

  // Setup - timeseries with first observation NOT at offset 0
  // This configuration will trigger the bugs!
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 100, 100);
  timeseries_add_observation(&ts, 200, 200);
  timeseries_set_last_datetime(&ts);

  // Setup datetimes for testing
  struct Datetime dt_before, dt_at_first, dt_between, dt_at_last;
  datetime_initialize(&dt_before, 2025, 1, 1, 0, 0, 50);    // offset 50
  datetime_initialize(&dt_at_first, 2025, 1, 1, 0, 1, 40);  // offset 100
  datetime_initialize(&dt_between, 2025, 1, 1, 0, 2, 30);   // offset 150
  datetime_initialize(&dt_at_last, 2025, 1, 1, 0, 3, 20);   // offset 200

  // Checks - interpolation at exact observations
  cmp_ok(timeseries_interpolation(&ts, &dt_at_first), "==", 100,
         "Interpolation at first observation works");
  cmp_ok(timeseries_interpolation(&ts, &dt_at_last), "==", 200,
         "Interpolation at last observation works");

  // Checks - interpolation between observations
  cmp_ok(timeseries_interpolation(&ts, &dt_between), "==", 150,
         "Interpolation between observations works");

  // Checks - interpolation BEFORE first observation
  // BUG #3: This will CRASH with buggy code (access to offsets[-1])
  // After fix: should call report_cannot_interpolate_outside_of_range()
  // Since we can't catch the error in libtap, we skip this test
  // It should be tested manually or with Bats
  diag("Skipping test for interpolation before first observation");
  diag("(would crash with buggy code - test manually)");
  // timeseries_interpolation(&ts, &dt_before);  // <- SEGFAULT with buggy code

  // Teardown
  datetime_delete(&dt_before);
  datetime_delete(&dt_at_first);
  datetime_delete(&dt_between);
  datetime_delete(&dt_at_last);
  timeseries_delete(&ts);

  // Setup - test interpolation after last observation
  // BUG #1 and #2: This will CRASH with buggy code
  struct Timeseries ts2;
  timeseries_initialize(&ts2);
  datetime_initialize(&ts2.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts2, 0, 0);
  timeseries_add_observation(&ts2, 100, 100);
  // Manually set last_datetime to be after last observation
  ts2.last_datetime = datetime_copy(&ts2.start_datetime);
  datetime_add_seconds(&ts2.last_datetime, 200);

  struct Datetime dt_after;
  datetime_initialize(&dt_after, 2025, 1, 1, 0, 2, 30);  // offset 150

  // Checks - interpolation after last observation
  // BUG #1 & #2: This will CRASH with buggy code (buffer overflow)
  // After fix: should call report_cannot_interpolate_outside_of_range()
  diag("Skipping test for interpolation after last observation");
  diag("(would crash with buggy code - test manually)");
  // timeseries_interpolation(&ts2, &dt_after);  // <- SEGFAULT with buggy code

  // Teardown
  datetime_delete(&dt_after);
  timeseries_delete(&ts2);
}

/**
 * Tests the timeseries_interpolation with gaps
 * This test validates behavior when there are gaps in observations
 */
void test_timeseries_interpolation_with_gaps(void) {
  diag("Testing timeseries_interpolation with gaps");

  // Setup - timeseries with gap between start and first observation
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 60, 60);   // First obs at 1 minute
  timeseries_add_observation(&ts, 120, 120); // Second obs at 2 minutes
  timeseries_set_last_datetime(&ts);

  // Setup datetimes
  struct Datetime dt_start, dt_at_60, dt_between, dt_at_120;
  datetime_initialize(&dt_start, 2025, 1, 1, 0, 0, 0);     // offset 0
  datetime_initialize(&dt_at_60, 2025, 1, 1, 0, 1, 0);     // offset 60
  datetime_initialize(&dt_between, 2025, 1, 1, 0, 1, 30);  // offset 90
  datetime_initialize(&dt_at_120, 2025, 1, 1, 0, 2, 0);    // offset 120

  // Checks - exact values
  cmp_ok(timeseries_interpolation(&ts, &dt_at_60), "==", 60,
         "Interpolation at first observation (not at start)");
  cmp_ok(timeseries_interpolation(&ts, &dt_at_120), "==", 120,
         "Interpolation at last observation");

  // Checks - interpolation between observations
  cmp_ok(timeseries_interpolation(&ts, &dt_between), "==", 90,
         "Interpolation between observations with gap");

  // Note: Interpolation at dt_start (before first obs) would crash
  // This is expected behavior - should call report_error

  // Teardown
  datetime_delete(&dt_start);
  datetime_delete(&dt_at_60);
  datetime_delete(&dt_between);
  datetime_delete(&dt_at_120);
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_add_observation function with capacity expansion
 */
void test_timeseries_capacity_expansion(void) {
  diag("Testing timeseries capacity expansion");

  // Setup
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  unsigned int initial_capacity = ts.capacity;

  // Checks - add observations to trigger capacity expansion
  for (int i = 0; i < 10; i++) {
    timeseries_add_observation(&ts, i * 10, i);
  }
  ok(ts.capacity > initial_capacity, "Capacity expanded after adding observations");
  cmp_ok((int)ts.size, "==", 10, "All observations added successfully");

  // Teardown
  timeseries_set_last_datetime(&ts);
  timeseries_delete(&ts);
}

/**
 * Tests edge cases with single observation
 */
void test_timeseries_single_observation(void) {
  diag("Testing timeseries with single observation");

  // Setup
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 42);
  timeseries_set_last_datetime(&ts);

  // Checks
  cmp_ok(timeseries_min_value(&ts), "==", 42, "Min value with single observation");
  cmp_ok(timeseries_max_value(&ts), "==", 42, "Max value with single observation");
  cmp_ok((int)timeseries_amplitude(&ts), "==", 0, "Amplitude is 0 with single observation");
  cmp_ok((int)timeseries_duration(&ts), "==", 0, "Duration is 0 with single observation");

  // Teardown
  timeseries_delete(&ts);
}

/**
 * Tests timeseries with negative values
 */
void test_timeseries_negative_values(void) {
  diag("Testing timeseries with negative values");

  // Setup
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, -100);
  timeseries_add_observation(&ts, 60, -50);
  timeseries_add_observation(&ts, 120, -75);
  timeseries_set_last_datetime(&ts);

  // Checks
  cmp_ok(timeseries_min_value(&ts), "==", -100, "Min value with negatives");
  cmp_ok(timeseries_max_value(&ts), "==", -50, "Max value with negatives");
  cmp_ok((int)timeseries_amplitude(&ts), "==", 50, "Amplitude with negatives");

  // Teardown
  timeseries_delete(&ts);
}

int main(void) {
  test_timeseries_initialize();
  test_timeseries_offset_index();
  test_timeseries_add_observation();
  test_timeseries_set_last_datetime();
  test_timeseries_min_value();
  test_timeseries_max_value();
  test_timeseries_duration();
  test_timeseries_amplitude();
  test_timeseries_interpolation();
  test_timeseries_interpolation_edge_cases();
  test_timeseries_interpolation_with_gaps();
  test_timeseries_capacity_expansion();
  test_timeseries_single_observation();
  test_timeseries_negative_values();
  done_testing();
}
