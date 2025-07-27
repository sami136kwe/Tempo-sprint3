setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

@test "tempo without subcommand shows help" {
  run tempo
  assert_line --index 0 "Usage: tempo"
  assert_line --index 1 "Displays information about a timeseries."
}

@test "tempo describe with empty timeseries displays description" {
  run tempo describe < "$examples_dir/empty.ts"
  assert_output "An empty timeseries with origin 2025-09-01T09:00:00"
}
