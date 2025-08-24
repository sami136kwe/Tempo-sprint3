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
  assert_output "An empty timeseries with origin 2025-09-01T09:00:00"
}

# Wrong usage
# -----------

@test "tempo without subcommand returns an error" {
  run tempo
  assert_failure
  assert_line --index 0 "error: subcommand is mandatory"
}
