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
  assert_output "error: cannot interpolate (empty timeseries)"
}

@test "tempo interpolate with timeseries of size 1 works" {
  run tempo interpolate < "$examples_dir/1.ts"
  assert_output "2025-09-01T09:00:00 10"
}

@test "tempo interpolate with timeseries of size 2 works" {
  run tempo interpolate < "$examples_dir/2_5s.ts"
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 1 "2025-09-01T09:00:01 11"
  assert_line --index 2 "2025-09-01T09:00:02 12"
  assert_line --index 3 "2025-09-01T09:00:03 13"
  assert_line --index 4 "2025-09-01T09:00:04 14"
  assert_line --index 5 "2025-09-01T09:00:05 15"
}

@test "tempo interpolate with timeseries of size 3 works" {
  run tempo interpolate < "$examples_dir/3_10s.ts"
  assert_line --index 0  "2025-09-01T09:00:00 10"
  assert_line --index 1  "2025-09-01T09:00:01 12"
  assert_line --index 2  "2025-09-01T09:00:02 14"
  assert_line --index 3  "2025-09-01T09:00:03 16"
  assert_line --index 4  "2025-09-01T09:00:04 18"
  assert_line --index 5  "2025-09-01T09:00:05 20"
  assert_line --index 6  "2025-09-01T09:00:06 19"
  assert_line --index 7  "2025-09-01T09:00:07 18"
  assert_line --index 8  "2025-09-01T09:00:08 17"
  assert_line --index 9  "2025-09-01T09:00:09 16"
  assert_line --index 10 "2025-09-01T09:00:10 15"
}

@test "tempo interpolate -s 4s with timeseries of size 3 works" {
  run tempo interpolate -s 4s < "$examples_dir/3_10s.ts"
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 1 "2025-09-01T09:00:04 18"
  assert_line --index 2 "2025-09-01T09:00:08 17"
}

@test "tempo interpolate -s 30m with timeseries of size 2 works" {
  run tempo interpolate -s 30m < "$examples_dir/2.ts"
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 1 "2025-09-01T09:30:00 15"
  assert_line --index 2 "2025-09-01T10:00:00 20"
}

@test "tempo interpolate -s 4h with timeseries 24h.ts works" {
  run tempo interpolate -s 4h < "$examples_dir/24h.ts"
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 1 "2025-09-01T13:00:00 12"
  assert_line --index 2 "2025-09-01T17:00:00 14"
  assert_line --index 3 "2025-09-01T21:00:00 16"
  assert_line --index 4 "2025-09-02T01:00:00 18"
  assert_line --index 5 "2025-09-02T05:00:00 20"
  assert_line --index 6 "2025-09-02T09:00:00 22"
}
