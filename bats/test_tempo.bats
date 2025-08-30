setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Normal usage
# ------------

@test "tempo help shows the help" {
  run tempo help
  assert_line --index 0 "Usage: tempo SUBCOMMAND"
  assert_line --index 1 "Displays information about a timeseries."
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

@test "tempo show with empty timeseries displays the observations" {
  run tempo show < "$examples_dir/empty.ts"
  assert_output ""
}

@test "tempo show with timeseries of size 1 displays the observations" {
  run tempo show < "$examples_dir/1.ts"
  assert_output "2025-09-01T09:00:00 10"
}

# Wrong usage
# -----------

@test "tempo without subcommand returns an error" {
  run tempo
  assert_failure
  assert_line --index 0 "error: subcommand is mandatory"
}

@test "tempo with unrecognized subcommand returns an error" {
  run tempo sc
  assert_failure
  assert_line --index 0 "error: unrecognized subcommand 'sc'"
}
