setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

@test "tempo describe with empty timeseries displays description" {
  run tempo describe < "$examples_dir/empty.ts"
  assert_line --index 0 "Range: [2025-09-01T09:00:00, 2025-09-01T09:00:00]"
  assert_line --index 1 "Size: 0"
}

@test "tempo describe with timeseries of size 1 displays description" {
  run tempo describe < "$examples_dir/1.ts"
  assert_line --index 0 "Range: [2025-09-01T09:00:00, 2025-09-01T09:00:00]"
  assert_line --index 1 "Size: 1"
}
