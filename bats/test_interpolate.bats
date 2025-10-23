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

@test "tempo interpolate with 5 seconds range timeseries of size 2 works" {
  run tempo interpolate < "$examples_dir/2_5s.ts"
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 1 "2025-09-01T09:00:01 11"
  assert_line --index 2 "2025-09-01T09:00:02 12"
  assert_line --index 3 "2025-09-01T09:00:03 13"
  assert_line --index 4 "2025-09-01T09:00:04 14"
  assert_line --index 5 "2025-09-01T09:00:05 15"
}
