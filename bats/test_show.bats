setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

@test "tempo show with empty timeseries displays the observations" {
  run tempo show < "$examples_dir/empty.ts"
  assert_output ""
}

@test "tempo show with timeseries of size 1 displays the observations" {
  run tempo show < "$examples_dir/1.ts"
  assert_output "2025-09-01T09:00:00 10"
}
