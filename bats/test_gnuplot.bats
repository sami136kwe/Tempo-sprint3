#!/usr/bin/env bats

setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Tests gnuplot - format texte
# -----------------------------

@test "tempo gnuplot with timeseries generates valid script" {
  run tempo gnuplot < "$examples_dir/2.ts"
  assert_success
  assert_line --partial "set terminal png"
  assert_line --partial "plot '-'"
}

@test "tempo gnuplot script executes without error" {
  tempo gnuplot < "$examples_dir/6.ts" > /tmp/test_plot.gp
  run gnuplot /tmp/test_plot.gp
  assert_success
  rm -f /tmp/test_plot.gp
}

# Tests gnuplot - format JSON
# ----------------------------

@test "tempo gnuplot -J with timeseries generates valid script" {
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10},{\"offset\":3600,\"value\":20}]}" | tempo gnuplot -J'
  assert_success
  assert_line --partial "set terminal png"
  assert_line --partial "plot '-'"
}

@test "tempo gnuplot --from-json script executes without error" {
  bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10},{\"offset\":3600,\"value\":20}]}" | tempo gnuplot --from-json' > /tmp/test_plot_json.gp
  run gnuplot /tmp/test_plot_json.gp
  assert_success
  rm -f /tmp/test_plot_json.gp
}

# Validation arguments
# --------------------

@test "tempo gnuplot rejects too many arguments" {
  run tempo gnuplot extra < "$examples_dir/1.ts"
  [ "$status" -eq 1 ]
  assert_output "error: too many arguments to 'gnuplot' subcommand"
}

@test "tempo gnuplot -J rejects too many arguments" {
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[]}" | tempo gnuplot -J extra'
  [ "$status" -eq 1 ]
  assert_output "error: too many arguments to 'gnuplot' subcommand"
}