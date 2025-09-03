setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

@test "tempo show with empty timeseries works" {
  run tempo show < "$examples_dir/empty.ts"
  assert_output ""
}

@test "tempo show with timeseries of size 1 works" {
  run tempo show < "$examples_dir/1.ts"
  assert_output "2025-09-01T09:00:00 10"
}

@test "tempo show with chronological timeseries of size 2 works" {
  run tempo show < "$examples_dir/2.ts"
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 1 "2025-09-01T10:00:00 20"
}

@test "tempo show with antichronological timeseries of size 2 works" {
  run tempo show < "$examples_dir/2rev.ts"
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 1 "2025-09-01T10:00:00 20"
}

@test "tempo show with timeseries with repeated offsets works" {
  run tempo show < "$examples_dir/2dbl.ts"
  assert_output "2025-09-01T09:00:00 20"
}
