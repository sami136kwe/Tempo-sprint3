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
  assert_line --index 0 "Usage: tempo SUBCOMMAND [options]"
  assert_line --index 1 "Displays information about a timeseries."
}

@test "tempo help describes a valid timeseries text stream" {
  run tempo help
  assert_line --partial "a text stream that must satisfy the following syntax"
}

@test "tempo help mentions the describe subcommand" {
  run tempo help
  assert_line --partial "  describe:"
}

@test "tempo help mentions the help subcommand" {
  run tempo help
  assert_line --partial "  help:"
}

@test "tempo help mentions the interpolate subcommand" {
  run tempo help
  assert_line --partial "  interpolate:"
}

@test "tempo help mentions the -s|--step option" {
  run tempo help
  assert_line --partial "    -s|--step DURATION:"
}

@test "tempo help mentions the show subcommand" {
  run tempo help
  assert_line --partial "  show:"
}

# Wrong usage
# -----------

@test "tempo without subcommand returns an error" {
  run tempo
  [ "$status" -eq 1 ]
  assert_line --index 0 "error: subcommand is mandatory"
}

@test "tempo with unrecognized subcommand returns an error" {
  run tempo sc
  [ "$status" -eq 1 ]
  assert_line --index 0 "error: unrecognized subcommand 'sc'"
}

@test "tempo describe with additional arguments returns an error" {
  run tempo describe timeseries < "$examples_dir/1.ts"
  [ "$status" -eq 1 ]
  assert_line --index 0 "error: too many arguments to 'describe' subcommand"
}

@test "tempo help with additional arguments returns an error" {
  run tempo help me
  [ "$status" -eq 1 ]
  assert_line --index 0 "error: too many arguments to 'help' subcommand"
}

@test "tempo show with additional arguments returns an error" {
  run tempo show timeseries < "$examples_dir/1.ts"
  [ "$status" -eq 1 ]
  assert_line --index 0 "error: too many arguments to 'show' subcommand"
}
