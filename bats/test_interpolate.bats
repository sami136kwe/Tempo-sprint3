setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

@test "tempo interpolate with empty timeseries works" {
  run tempo interpolate < "$examples_dir/empty.ts"
  [ "$status" -eq 2 ]
  assert_output "error: cannot interpolate with empty timeseries"
}

@test "tempo interpolate with timeseries of size 1 works" {
  run tempo interpolate < "$examples_dir/1.ts"
  assert_output "2025-09-01T09:00:00 10"
}

#@test "tempo interpolate with chronological timeseries of size 2 works" {
#  run tempo interpolate < "$examples_dir/2.ts"
#  assert_line --index 0 "2025-09-01T09:00:00 10"
#  assert_line --index 1 "2025-09-01T10:00:00 20"
#}
#
#@test "tempo interpolate with antichronological timeseries of size 2 works" {
#  run tempo interpolate < "$examples_dir/2rev.ts"
#  assert_line --index 0 "2025-09-01T09:00:00 10"
#  assert_line --index 1 "2025-09-01T10:00:00 20"
#}
#
#@test "tempo interpolate with timeseries with repeated offsets works" {
#  run tempo interpolate < "$examples_dir/2dbl.ts"
#  assert_output "2025-09-01T09:00:00 20"
#}
#
#@test "tempo interpolate with timeseries with multiple repeated offsets works" {
#  run tempo interpolate < "$examples_dir/3dbl.ts"
#  assert_output "2025-09-01T09:00:00 20"
#}
#
#@test "tempo interpolate with year-changing timeseries works" {
#  run tempo interpolate < "$examples_dir/y.ts"
#  assert_line --index 0 "1999-12-31T23:59:58 10"
#  assert_line --index 1 "1999-12-31T23:59:59 20"
#  assert_line --index 2 "2000-01-01T00:00:00 30"
#  assert_line --index 3 "2000-01-01T00:00:01 40"
#  assert_line --index 4 "2000-01-01T00:00:02 50"
#}
#
#@test "tempo interpolate with month-changing timeseries works" {
#  run tempo interpolate < "$examples_dir/m.ts"
#  assert_line --index 0 "2025-09-30T23:59:58 10"
#  assert_line --index 1 "2025-09-30T23:59:59 20"
#  assert_line --index 2 "2025-10-01T00:00:00 30"
#  assert_line --index 3 "2025-10-01T00:00:01 40"
#  assert_line --index 4 "2025-10-01T00:00:02 50"
#}
#
#@test "tempo interpolate with day-changing timeseries works" {
#  run tempo interpolate < "$examples_dir/d.ts"
#  assert_line --index 0 "2025-12-24T23:59:58 10"
#  assert_line --index 1 "2025-12-24T23:59:59 20"
#  assert_line --index 2 "2025-12-25T00:00:00 30"
#  assert_line --index 3 "2025-12-25T00:00:01 40"
#  assert_line --index 4 "2025-12-25T00:00:02 50"
#}
#
#@test "tempo interpolate with hour-changing timeseries works" {
#  run tempo interpolate < "$examples_dir/h.ts"
#  assert_line --index 0 "2025-09-01T09:59:58 10"
#  assert_line --index 1 "2025-09-01T09:59:59 20"
#  assert_line --index 2 "2025-09-01T10:00:00 30"
#  assert_line --index 3 "2025-09-01T10:00:01 40"
#  assert_line --index 4 "2025-09-01T10:00:02 50"
#}
#
#@test "tempo interpolate with minute-changing timeseries works" {
#  run tempo interpolate < "$examples_dir/min.ts"
#  assert_line --index 0 "2025-09-01T09:29:58 10"
#  assert_line --index 1 "2025-09-01T09:29:59 20"
#  assert_line --index 2 "2025-09-01T09:30:00 30"
#  assert_line --index 3 "2025-09-01T09:30:01 40"
#  assert_line --index 4 "2025-09-01T09:30:02 50"
#}
