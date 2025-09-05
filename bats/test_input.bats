setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

@test "tempo show with datetime missing the character T returns an error" {
  run tempo show < "$examples_dir/missing_t.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid datetime format (should be YYYY:mm:DDTHH:MM:SS)"
}
