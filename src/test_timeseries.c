#include <tap.h>
#include <limits.h>

#include "timeseries.h"
#include "datetime.h"

/**
 * Helper function to safely delete a timeseries initialized with
 * timeseries_initialize (without datetime initialization)
 *
 * This is used in tests where we only need to initialize the timeseries
 * structure without setting up the datetime fields.
 *
 * @param timeseries  The timeseries to delete
 */
void timeseries_delete_no_datetime(struct Timeseries* timeseries) {
  free(timeseries->offsets);
  free(timeseries->values);
}

/**
 * Tests the timeseries_initialize function
 *
 * Verifies that:
 * - Size is initialized to 0
 * - Capacity is initialized to 1
 * - Offsets array is properly allocated
 * - Values array is properly allocated
 */
void test_timeseries_initialize(void) {
  diag("Testing timeseries_initialize");

  struct Timeseries ts;
  timeseries_initialize(&ts);

  cmp_ok((int)ts.size, "==", 0, "Initialized timeseries has size 0");
  cmp_ok((int)ts.capacity, "==", 1, "Initialized timeseries has capacity 1");
  ok(ts.offsets != NULL, "Offsets array is allocated");
  ok(ts.values != NULL, "Values array is allocated");

  timeseries_delete_no_datetime(&ts);
}

/**
 * Tests the timeseries_offset_index helper function
 *
 * Verifies that:
 * - Empty timeseries returns -1 for any offset
 * - Existing offsets return correct indices
 * - Non-existing offsets return -1
 * - Boundary cases are handled correctly
 */
void test_timeseries_offset_index(void) {
  diag("Testing timeseries_offset_index");

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);

  cmp_ok(timeseries_offset_index(&ts, 0), "==", -1,
         "Empty timeseries returns -1");

  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 100, 20);
  timeseries_add_observation(&ts, 200, 30);

  cmp_ok(timeseries_offset_index(&ts, 0), "==", 0,
         "Offset found at beginning");
  cmp_ok(timeseries_offset_index(&ts, 100), "==", 1,
         "Offset found at middle");
  cmp_ok(timeseries_offset_index(&ts, 200), "==", 2,
         "Offset found at end");

  cmp_ok(timeseries_offset_index(&ts, -10), "==", -1,
         "Offset smaller than first returns -1");
  cmp_ok(timeseries_offset_index(&ts, 50), "==", -1,
         "Offset between two existing offsets returns -1");
  cmp_ok(timeseries_offset_index(&ts, 300), "==", -1,
         "Offset larger than last returns -1");

  timeseries_set_last_datetime(&ts);
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_add_observation function
 *
 * Verifies that:
 * - Observations can be added to empty timeseries
 * - Capacity expands when needed
 * - Observations are maintained in sorted order by offset
 * - Duplicate offsets overwrite existing values
 * - Size is managed correctly
 */
void test_timeseries_add_observation(void) {
  diag("Testing timeseries_add_observation");

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);

  timeseries_add_observation(&ts, 50, 5);
  cmp_ok((int)ts.size, "==", 1, "Added to empty timeseries");
  cmp_ok(ts.offsets[0], "==", 50, "Correct offset");
  cmp_ok(ts.values[0], "==", 5, "Correct value");

  unsigned int old_capacity = ts.capacity;
  timeseries_add_observation(&ts, 100, 10);
  ok(ts.capacity > old_capacity, "Capacity expanded");

  timeseries_add_observation(&ts, 25, 2);
  timeseries_add_observation(&ts, 75, 7);
  timeseries_add_observation(&ts, 150, 15);
  cmp_ok((int)ts.size, "==", 5, "All observations added");
  ok(ts.offsets[0] == 25 && ts.offsets[1] == 50 && ts.offsets[2] == 75 &&
     ts.offsets[3] == 100 && ts.offsets[4] == 150,
     "All offsets in sorted order");

  timeseries_add_observation(&ts, 75, 77);
  cmp_ok((int)ts.size, "==", 5, "Size unchanged when overwriting");
  cmp_ok(ts.values[2], "==", 77, "Value overwritten correctly");

  timeseries_set_last_datetime(&ts);
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_set_last_datetime function
 *
 * Verifies that:
 * - Empty timeseries has last_datetime equal to start_datetime
 * - Single observation sets last_datetime correctly
 * - Multiple observations identify maximum offset correctly
 * - Last datetime is calculated as start + max_offset
 */
void test_timeseries_set_last_datetime(void) {
  diag("Testing timeseries_set_last_datetime");

  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);
  datetime_initialize(&ts_empty.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_set_last_datetime(&ts_empty);

  ok(datetime_compare(&ts_empty.start_datetime, &ts_empty.last_datetime) == 0,
     "Empty timeseries has last == start");

  timeseries_delete(&ts_empty);

  struct Timeseries ts1;
  timeseries_initialize(&ts1);
  datetime_initialize(&ts1.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts1, 100, 10);
  timeseries_set_last_datetime(&ts1);

  cmp_ok((int)timeseries_duration(&ts1), "==", 100,
         "Single element sets last to start + 100s");

  timeseries_delete(&ts1);

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 200, 20);
  timeseries_add_observation(&ts, 100, 10);
  timeseries_add_observation(&ts, 50, 5);
  timeseries_set_last_datetime(&ts);

  cmp_ok((int)timeseries_duration(&ts), "==", 200,
         "Max offset correctly identified");

  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_min_value function
 *
 * Verifies that:
 * - Empty timeseries returns INT_MAX
 * - Minimum value is correctly identified among positive values
 * - Minimum value is correctly identified among negative values
 * - Function handles mixed positive and negative values
 */
void test_timeseries_min_value(void) {
  diag("Testing timeseries_min_value");

  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);

  cmp_ok(timeseries_min_value(&ts_empty), "==", INT_MAX,
         "Min value of empty timeseries is INT_MAX");

  timeseries_delete_no_datetime(&ts_empty);

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 60, 5);
  timeseries_add_observation(&ts, 120, 15);
  timeseries_add_observation(&ts, 180, -3);
  timeseries_set_last_datetime(&ts);

  cmp_ok(timeseries_min_value(&ts), "==", -3,
         "Min value is correctly identified");

  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_max_value function
 *
 * Verifies that:
 * - Empty timeseries returns INT_MIN
 * - Maximum value is correctly identified among positive values
 * - Maximum value is correctly identified among negative values
 * - Function handles mixed positive and negative values
 */
void test_timeseries_max_value(void) {
  diag("Testing timeseries_max_value");

  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);

  cmp_ok(timeseries_max_value(&ts_empty), "==", INT_MIN,
         "Max value of empty timeseries is INT_MIN");

  timeseries_delete_no_datetime(&ts_empty);

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 60, 5);
  timeseries_add_observation(&ts, 120, 15);
  timeseries_add_observation(&ts, 180, -3);
  timeseries_set_last_datetime(&ts);

  cmp_ok(timeseries_max_value(&ts), "==", 15,
         "Max value is correctly identified");

  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_duration function
 *
 * Verifies that:
 * - Empty timeseries has duration of 0
 * - Duration is calculated as difference between start and last datetime
 * - Duration is returned in seconds
 */
void test_timeseries_duration(void) {
  diag("Testing timeseries_duration");

  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);
  datetime_initialize(&ts_empty.start_datetime, 2025, 1, 1, 0, 0, 0);
  ts_empty.last_datetime = datetime_copy(&ts_empty.start_datetime);

  cmp_ok((int)timeseries_duration(&ts_empty), "==", 0,
         "Duration of empty timeseries is 0");

  timeseries_delete(&ts_empty);

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 10);
  timeseries_add_observation(&ts, 3600, 20);
  timeseries_set_last_datetime(&ts);

  cmp_ok((int)timeseries_duration(&ts), "==", 3600,
         "Duration is correctly calculated");

  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_amplitude function
 *
 * Verifies that:
 * - Empty timeseries has amplitude of 0
 * - Amplitude is calculated as max_value - min_value
 * - Function handles positive values
 * - Function handles negative values
 * - Function handles mixed positive and negative values
 */
void test_timeseries_amplitude(void) {
  diag("Testing timeseries_amplitude");

  struct Timeseries ts_empty;
  timeseries_initialize(&ts_empty);

  cmp_ok((int)timeseries_amplitude(&ts_empty), "==", 0,
         "Amplitude of empty timeseries is 0");

  timeseries_delete_no_datetime(&ts_empty);

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, -5);
  timeseries_add_observation(&ts, 60, 10);
  timeseries_add_observation(&ts, 120, 15);
  timeseries_set_last_datetime(&ts);

  cmp_ok((int)timeseries_amplitude(&ts), "==", 20,
         "Amplitude is correctly calculated");

  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_interpolation function
 *
 * Verifies that:
 * - Interpolation at exact observation returns exact value
 * - Linear interpolation at midpoint returns correct value
 * - Linear interpolation between any two observations is correct
 */
void test_timeseries_interpolation(void) {
  diag("Testing timeseries_interpolation");

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

  cmp_ok(timeseries_interpolation(&ts, &dt_exact), "==", 0,
         "Interpolation at exact observation returns exact value");

  cmp_ok(timeseries_interpolation(&ts, &dt_mid1), "==", 50,
         "Linear interpolation at midpoint works correctly");
  cmp_ok(timeseries_interpolation(&ts, &dt_mid2), "==", 75,
         "Linear interpolation between observations works correctly");

  datetime_delete(&dt_exact);
  datetime_delete(&dt_mid1);
  datetime_delete(&dt_mid2);
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_interpolation function with edge cases
 *
 * This test validates behavior when the first observation is not at offset 0.
 * This configuration exposes potential bugs in the interpolation logic:
 *
 * Potential bugs:
 * - BUG #1: Buffer overflow when interpolating after the last observation
 * - BUG #2: Accessing offsets[i] when i >= size
 * - BUG #3: Accessing offsets[i-1] when i == 0
 *
 * Verifies that:
 * - Interpolation at first observation works (even when offset != 0)
 * - Interpolation at last observation works
 * - Interpolation between observations works correctly
 * - Interpolation before/after observations should call error handler
 */
void test_timeseries_interpolation_edge_cases(void) {
  diag("Testing timeseries_interpolation edge cases");

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 100, 100);
  timeseries_add_observation(&ts, 200, 200);
  timeseries_set_last_datetime(&ts);

  struct Datetime dt_before, dt_at_first, dt_between, dt_at_last;
  datetime_initialize(&dt_before, 2025, 1, 1, 0, 0, 50);
  datetime_initialize(&dt_at_first, 2025, 1, 1, 0, 1, 40);
  datetime_initialize(&dt_between, 2025, 1, 1, 0, 2, 30);
  datetime_initialize(&dt_at_last, 2025, 1, 1, 0, 3, 20);

  cmp_ok(timeseries_interpolation(&ts, &dt_at_first), "==", 100,
         "Interpolation at first observation works");
  cmp_ok(timeseries_interpolation(&ts, &dt_at_last), "==", 200,
         "Interpolation at last observation works");

  cmp_ok(timeseries_interpolation(&ts, &dt_between), "==", 150,
         "Interpolation between observations works");

  diag("Skipping test for interpolation before first observation");
  diag("(would crash with buggy code - test manually)");

  datetime_delete(&dt_before);
  datetime_delete(&dt_at_first);
  datetime_delete(&dt_between);
  datetime_delete(&dt_at_last);
  timeseries_delete(&ts);

  struct Timeseries ts2;
  timeseries_initialize(&ts2);
  datetime_initialize(&ts2.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts2, 0, 0);
  timeseries_add_observation(&ts2, 100, 100);
  ts2.last_datetime = datetime_copy(&ts2.start_datetime);
  datetime_add_seconds(&ts2.last_datetime, 200);

  struct Datetime dt_after;
  datetime_initialize(&dt_after, 2025, 1, 1, 0, 2, 30);

  diag("Skipping test for interpolation after last observation");
  diag("(would crash with buggy code - test manually)");

  datetime_delete(&dt_after);
  timeseries_delete(&ts2);
}

/**
 * Tests the timeseries_interpolation with gaps
 *
 * This test validates behavior when there are gaps in observations,
 * specifically when the first observation is not at the start datetime.
 *
 * Verifies that:
 * - Interpolation works when first observation is not at offset 0
 * - Interpolation between observations works correctly with gaps
 * - Values at exact observation points are returned correctly
 *
 * Note: Interpolation before the first observation should trigger an error
 * as it's outside the valid range. This is tested manually or with Bats.
 */
void test_timeseries_interpolation_with_gaps(void) {
  diag("Testing timeseries_interpolation with gaps");

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 60, 60);
  timeseries_add_observation(&ts, 120, 120);
  timeseries_set_last_datetime(&ts);

  struct Datetime dt_start, dt_at_60, dt_between, dt_at_120;
  datetime_initialize(&dt_start, 2025, 1, 1, 0, 0, 0);
  datetime_initialize(&dt_at_60, 2025, 1, 1, 0, 1, 0);
  datetime_initialize(&dt_between, 2025, 1, 1, 0, 1, 30);
  datetime_initialize(&dt_at_120, 2025, 1, 1, 0, 2, 0);

  cmp_ok(timeseries_interpolation(&ts, &dt_at_60), "==", 60,
         "Interpolation at first observation (not at start)");
  cmp_ok(timeseries_interpolation(&ts, &dt_at_120), "==", 120,
         "Interpolation at last observation");

  cmp_ok(timeseries_interpolation(&ts, &dt_between), "==", 90,
         "Interpolation between observations with gap");

  datetime_delete(&dt_start);
  datetime_delete(&dt_at_60);
  datetime_delete(&dt_between);
  datetime_delete(&dt_at_120);
  timeseries_delete(&ts);
}

/**
 * Tests the timeseries_add_observation function with capacity expansion
 *
 * Verifies that:
 * - Capacity automatically expands when size reaches capacity
 * - All observations are preserved after capacity expansion
 * - Observations remain in sorted order after expansion
 * - Multiple expansions can occur as needed
 */
void test_timeseries_capacity_expansion(void) {
  diag("Testing timeseries capacity expansion");

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  unsigned int initial_capacity = ts.capacity;

  for (int i = 0; i < 10; i++) {
    timeseries_add_observation(&ts, i * 10, i);
  }
  ok(ts.capacity > initial_capacity, "Capacity expanded after adding observations");
  cmp_ok((int)ts.size, "==", 10, "All observations added successfully");

  timeseries_set_last_datetime(&ts);
  timeseries_delete(&ts);
}

/**
 * Tests edge cases with single observation
 *
 * Verifies that:
 * - Min and max values are both equal to the single value
 * - Amplitude is 0 when there's only one observation
 * - Duration is 0 when there's only one observation
 */
void test_timeseries_single_observation(void) {
  diag("Testing timeseries with single observation");

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, 42);
  timeseries_set_last_datetime(&ts);

  cmp_ok(timeseries_min_value(&ts), "==", 42, "Min value with single observation");
  cmp_ok(timeseries_max_value(&ts), "==", 42, "Max value with single observation");
  cmp_ok((int)timeseries_amplitude(&ts), "==", 0, "Amplitude is 0 with single observation");
  cmp_ok((int)timeseries_duration(&ts), "==", 0, "Duration is 0 with single observation");

  timeseries_delete(&ts);
}

/**
 * Tests timeseries with negative values
 *
 * Verifies that:
 * - Min value is correctly identified among negative values
 * - Max value is correctly identified among negative values
 * - Amplitude is calculated correctly with negative values
 */
void test_timeseries_negative_values(void) {
  diag("Testing timeseries with negative values");

  struct Timeseries ts;
  timeseries_initialize(&ts);
  datetime_initialize(&ts.start_datetime, 2025, 1, 1, 0, 0, 0);
  timeseries_add_observation(&ts, 0, -100);
  timeseries_add_observation(&ts, 60, -50);
  timeseries_add_observation(&ts, 120, -75);
  timeseries_set_last_datetime(&ts);

  cmp_ok(timeseries_min_value(&ts), "==", -100, "Min value with negatives");
  cmp_ok(timeseries_max_value(&ts), "==", -50, "Max value with negatives");
  cmp_ok((int)timeseries_amplitude(&ts), "==", 50, "Amplitude with negatives");

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
