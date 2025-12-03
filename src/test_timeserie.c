#include <tap.h>
#include <limits.h>

#include "timeseries.h"
#include "datetime.h"

/**
 * Helper function to safely delete a timeseries initialized with timeseries_initialize
 * (without datetime initialization)
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
 * Tests the timeseries_min_value function
 */
void test_timeseries_min_value(void) {
  diag("Testing timeseries_min_value");

  // Setup - empty timeseries
  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);

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
  cmp_ok(timeseries_min_value(&ts_empty), "==", INT_MAX,
         "Min value of empty timeseries is INT_MAX");
  cmp_ok(timeseries_min_value(&ts), "==", -3,
         "Min value is correctly identified");

  // Teardown
  timeseries_delete_no_datetime(&ts_empty);
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
  cmp_ok(timeseries_max_value(&ts_empty), "==", INT_MIN,
         "Max value of empty timeseries is INT_MIN");
  cmp_ok(timeseries_max_value(&ts), "==", 15,
         "Max value is correctly identified");

  // Teardown
  timeseries_delete_no_datetime(&ts_empty);
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

  // Setup - timeseries with observations
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 3600, 20);
  timeseries_set_last_datetime(&ts);

  // Checks
  cmp_ok((int)timeseries_duration(&ts_empty), "==", 0,
         "Duration of empty timeseries is 0");
  cmp_ok((int)timeseries_duration(&ts), "==", 3600,
         "Duration is correctly calculated (1 hour = 3600 seconds)");

  // Teardown
  timeseries_delete(&ts_empty);
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

  // Setup - timeseries with values
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, -5);
  timeseries_add_observation(&ts, 60, 10);
  timeseries_add_observation(&ts, 120, 15);
  timeseries_set_last_datetime(&ts);

  // Checks
  cmp_ok((int)timeseries_amplitude(&ts_empty), "==", 0,
         "Amplitude of empty timeseries is 0");
  cmp_ok((int)timeseries_amplitude(&ts), "==", 20,
         "Amplitude is correctly calculated (15 - (-5) = 20)");

  // Teardown
  timeseries_delete_no_datetime(&ts_empty);
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
         "Linear interpolation at midpoint works correctly (0 + 100)/2 = 50");
  cmp_ok(timeseries_interpolation(&ts, &dt_mid2), "==", 75,
         "Linear interpolation between 100 and 50 works correctly");

  // Teardown
  datetime_delete(&dt_exact);
  datetime_delete(&dt_mid1);
  datetime_delete(&dt_mid2);
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_add_observation function for sorted insertion
 */
void test_timeseries_add_observation(void) {
  diag("Testing timeseries_add_observation");

  // Setup
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);

  // Add observations out of order
  timeseries_add_observation(&ts, 100, 10);
  timeseries_add_observation(&ts, 50, 5);
  timeseries_add_observation(&ts, 150, 15);
  timeseries_add_observation(&ts, 25, 2);

  // Checks - verify sorted order
  cmp_ok((int)ts.size, "==", 4, "All observations added");
  cmp_ok(ts.offsets[0], "==", 25, "First offset is smallest");
  cmp_ok(ts.offsets[1], "==", 50, "Second offset is correct");
  cmp_ok(ts.offsets[2], "==", 100, "Third offset is correct");
  cmp_ok(ts.offsets[3], "==", 150, "Fourth offset is largest");
  
  cmp_ok(ts.values[0], "==", 2, "First value matches offset");
  cmp_ok(ts.values[1], "==", 5, "Second value matches offset");
  cmp_ok(ts.values[2], "==", 10, "Third value matches offset");
  cmp_ok(ts.values[3], "==", 15, "Fourth value matches offset");

  // Test overwriting existing observation
  timeseries_add_observation(&ts, 50, 100);
  cmp_ok((int)ts.size, "==", 4, "Size unchanged when overwriting");
  cmp_ok(ts.values[1], "==", 100, "Value overwritten correctly");

  // Set last datetime before teardown
  timeseries_set_last_datetime(&ts);

  // Teardown
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries capacity expansion
 */
void test_timeseries_capacity_expansion(void) {
  diag("Testing timeseries capacity expansion");

  // Setup
  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);

  unsigned int initial_capacity = ts.capacity;

  // Add observations to trigger capacity expansion
  for (int i = 0; i < 10; i++) {
    timeseries_add_observation(&ts, i * 10, i);
  }

  // Checks
  ok(ts.capacity > initial_capacity, "Capacity expanded after adding observations");
  cmp_ok((int)ts.size, "==", 10, "All observations added successfully");

  // Set last datetime before teardown
  timeseries_set_last_datetime(&ts);

  // Teardown
  timeseries_delete(&ts);
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
  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 100, 20);
  timeseries_add_observation(&ts, 200, 30);

  // Checks
  cmp_ok(timeseries_offset_index(&ts, 0), "==", 0,
         "Index of first offset is 0");
  cmp_ok(timeseries_offset_index(&ts, 100), "==", 1,
         "Index of middle offset is 1");
  cmp_ok(timeseries_offset_index(&ts, 200), "==", 2,
         "Index of last offset is 2");
  cmp_ok(timeseries_offset_index(&ts, 50), "==", -1,
         "Non-existent offset returns -1");
  cmp_ok(timeseries_offset_index(&ts, 300), "==", -1,
         "Offset beyond range returns -1");

  // Set last datetime before teardown
  timeseries_set_last_datetime(&ts);

  // Teardown
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
  cmp_ok((int)timeseries_amplitude(&ts), "==", 50, "Amplitude with negatives (-50 - (-100) = 50)");

  // Teardown
  timeseries_delete(&ts);
}

int main(void) {
  test_timeseries_initialize();
  test_timeseries_min_value();
  test_timeseries_max_value();
  test_timeseries_duration();
  test_timeseries_amplitude();
  test_timeseries_interpolation();
  test_timeseries_add_observation();
  test_timeseries_capacity_expansion();
  test_timeseries_offset_index();
  test_timeseries_single_observation();
  test_timeseries_negative_values();
  done_testing();
}
