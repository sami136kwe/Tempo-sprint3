#!/usr/bin/env bats

setup() {
  load "$BATS_TEST_DIRNAME/bats-support/load"
  load "$BATS_TEST_DIRNAME/bats-assert/load"
  
  root_dir="$(cd "$BATS_TEST_DIRNAME/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Tests JSON spécifiques - describe
# ----------------------------------

@test "tempo describe -J with basic timeseries works" {
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10},{\"offset\":3600,\"value\":20}]}" | tempo describe -J'
  assert_success
  assert_line "Duration: 3600"
  assert_line "Amplitude: 10"
}

@test "tempo describe --from-json handles repeated offsets" {
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10},{\"offset\":0,\"value\":20}]}" | tempo describe --from-json'
  assert_success
  assert_line "Size: 1"
  assert_line "Codomain: [20, 20]"
}

# Tests JSON spécifiques - show
# ------------------------------

@test "tempo show -J with basic timeseries works" {
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10}]}" | tempo show -J'
  assert_success
  assert_output "2025-09-01T09:00:00 10"
}

@test "tempo show --from-json handles antichronological order" {
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":3600,\"value\":20},{\"offset\":0,\"value\":10}]}" | tempo show --from-json'
  assert_success
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 1 "2025-09-01T10:00:00 20"
}

# Tests JSON spécifiques - interpolate
# -------------------------------------

@test "tempo interpolate -J with basic timeseries works" {
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10},{\"offset\":5,\"value\":15}]}" | tempo interpolate -J'
  assert_success
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 5 "2025-09-01T09:00:05 15"
}

@test "tempo interpolate --from-json with step options works" {
  # Test -s avec secondes
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10},{\"offset\":10,\"value\":20}]}" | tempo interpolate -J -s 2s'
  assert_success
  assert_line --index 0 "2025-09-01T09:00:00 10"
  assert_line --index 1 "2025-09-01T09:00:02 12"
  
  # Test --step avec minutes
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10},{\"offset\":3600,\"value\":20}]}" | tempo interpolate --from-json --step 30m'
  assert_success
  assert_line --index 1 "2025-09-01T09:30:00 15"
  
  # Test ordre des options flexible
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10},{\"offset\":6,\"value\":16}]}" | tempo interpolate -s 3s --from-json'
  assert_success
  assert_line --index 1 "2025-09-01T09:00:03 13"
}

# Validation JSON - Structure
# ----------------------------

@test "tempo -J validates JSON structure" {
  # JSON invalide
  run bash -c 'echo "{invalid}" | tempo describe -J'
  assert_failure 2
  assert_output --partial "error: JSON parsing failed"
  
  # Champ "origin" manquant
  run bash -c 'echo "{\"observations\":[]}" | tempo describe --from-json'
  assert_failure 2
  assert_output --partial "invalid timeseries structure"
  
  # Champ "observations" manquant
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\"}" | tempo show -J'
  assert_failure 2
  assert_output --partial "invalid timeseries structure"
  
  # Type incorrect pour "origin"
  run bash -c 'echo "{\"origin\":123,\"observations\":[]}" | tempo describe -J'
  assert_failure 2
  assert_output --partial "invalid timeseries structure"
  
  # Type incorrect pour "observations"
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":\"invalid\"}" | tempo show --from-json'
  assert_failure 2
  assert_output --partial "invalid timeseries structure"
}

# Validation JSON - Observations
# -------------------------------

@test "tempo -J validates observation structure" {
  # Observation n'est pas un objet
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[123]}" | tempo show -J'
  assert_failure 2
  assert_output "error: JSON parse error (observation must be an object)"
  
  # Champ "offset" manquant
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"value\":10}]}" | tempo describe --from-json'
  assert_failure 2
  assert_output "error: JSON parse error (invalid or missing offset)"
  
  # Champ "value" manquant
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0}]}" | tempo show -J'
  assert_failure 2
  assert_output "error: JSON parse error (invalid or missing value)"
  
  # Type incorrect pour "offset" (string au lieu d'integer)
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":\"0\",\"value\":10}]}" | tempo describe -J'
  assert_failure 2
  assert_output "error: JSON parse error (invalid or missing offset)"
  
  # Type incorrect pour "value" (string au lieu d'integer)
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":\"10\"}]}" | tempo show --from-json'
  assert_failure 2
  assert_output "error: JSON parse error (invalid or missing value)"
  
  # Offset négatif (spécifique à JSON, car déjà testé pour format texte)
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":-10,\"value\":10}]}" | tempo describe -J'
  assert_failure 2
  assert_output "error: JSON parse error (offset must be non-negative)"
}

# Validation CLI - Arguments JSON
# --------------------------------

@test "tempo -J validates CLI arguments" {
  # Trop d'arguments avec -J
  run bash -c 'echo "{}" | tempo describe -J extra'
  assert_failure 1
  assert_output "error: too many arguments to 'describe' subcommand"
  
  run bash -c 'echo "{}" | tempo show --from-json extra'
  assert_failure 1
  assert_output "error: too many arguments to 'show' subcommand"
}

# Documentation - Help
# --------------------

@test "tempo help documents JSON options" {
  run tempo help
  assert_success
  assert_line --partial "    -J|--from-json:"
  assert_line --partial "When using -J|--from-json"
}

# Tests de scénarios JSON spécifiques
# ------------------------------------

@test "tempo -J handles special values" {
  # Valeurs négatives
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":-10}]}" | tempo show -J'
  assert_success
  assert_output "2025-09-01T09:00:00 -10"
  
  # Valeur zéro
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":0}]}" | tempo describe --from-json'
  assert_success
  assert_line "Codomain: [0, 0]"
  
  # Grand offset (24 heures)
  run bash -c 'echo "{\"origin\":\"2025-09-01T09:00:00\",\"observations\":[{\"offset\":0,\"value\":10},{\"offset\":86400,\"value\":20}]}" | tempo describe -J'
  assert_success
  assert_line "Duration: 86400"
}