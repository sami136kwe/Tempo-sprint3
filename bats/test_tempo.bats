setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
}

@test "tempo without subcommand shows help" {
  run tempo
  assert_line --index 0 "Usage: tempo"
  assert_line --index 1 "Displays information about a timeseries."
}
