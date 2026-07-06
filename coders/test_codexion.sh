#!/usr/bin/env bash

set -u

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BIN="$SCRIPT_DIR/codexion"
MAKEFILE_DIR="$SCRIPT_DIR"
TMP_DIR="$(mktemp -d)"

PASS_COUNT=0
FAIL_COUNT=0

cleanup() {
	rm -rf "$TMP_DIR"
}
trap cleanup EXIT

pass() {
	PASS_COUNT=$((PASS_COUNT + 1))
	printf '[PASS] %s\n' "$1"
}

fail() {
	FAIL_COUNT=$((FAIL_COUNT + 1))
	printf '[FAIL] %s\n' "$1"
}

build_project() {
	if ! (cd "$MAKEFILE_DIR" && make --quiet >/dev/null 2>&1); then
		printf '[FATAL] Build failed\n'
		exit 1
	fi
}

run_codexion() {
	local outfile="$1"
	shift
	timeout 4s "$BIN" "$@" >"$outfile" 2>&1
	return $?
}

expect_invalid_input() {
	local name="$1"
	local pattern="$2"
	shift 2
	local out="$TMP_DIR/$name.out"
	local status

	run_codexion "$out" "$@"
	status=$?
	if [ "$status" -eq 0 ]; then
		fail "$name (expected non-zero exit)"
		return
	fi
	if ! grep -Eiq "$pattern" "$out"; then
		fail "$name (missing expected error pattern: $pattern)"
		return
	fi
	pass "$name"
}

expect_valid_run() {
	local name="$1"
	shift
	local out="$TMP_DIR/$name.out"
	local status

	run_codexion "$out" "$@"
	status=$?
	if [ "$status" -eq 124 ]; then
		fail "$name (timed out)"
		return
	fi
	if [ "$status" -ne 0 ]; then
		fail "$name (exit code $status)"
		return
	fi
	pass "$name"
}

check_log_format() {
	local name="$1"
	shift
	local out="$TMP_DIR/$name.out"
	local status

	run_codexion "$out" "$@"
	status=$?
	if [ "$status" -ne 0 ]; then
		fail "$name (run failed with exit code $status)"
		return
	fi
	if grep -Ev '^[0-9]+ [0-9]+ (has taken a dongle|is compiling|is debugging|is refactoring|burned out)$' "$out" | grep -q '.'; then
		fail "$name (invalid log line format)"
		return
	fi
	pass "$name"
}

check_contains() {
	local name="$1"
	local expected="$2"
	shift 2
	local out="$TMP_DIR/$name.out"
	local status

	run_codexion "$out" "$@"
	status=$?
	if [ "$status" -ne 0 ]; then
		fail "$name (run failed with exit code $status)"
		return
	fi
	if ! grep -q "$expected" "$out"; then
		fail "$name (missing expected output: $expected)"
		return
	fi
	pass "$name"
}

check_not_contains() {
	local name="$1"
	local forbidden="$2"
	shift 2
	local out="$TMP_DIR/$name.out"
	local status

	run_codexion "$out" "$@"
	status=$?
	if [ "$status" -ne 0 ]; then
		fail "$name (run failed with exit code $status)"
		return
	fi
	if grep -q "$forbidden" "$out"; then
		fail "$name (forbidden output present: $forbidden)"
		return
	fi
	pass "$name"
}

build_project

# ---- Error handling tests (subject: all args mandatory, numeric, valid scheduler) ----
expect_invalid_input "missing_argument" "Invalid number of argument|Should be 8" \
	3 800 200 200 200 3 100

expect_invalid_input "too_many_arguments" "Invalid number of argument|Should be 8" \
	3 800 200 200 200 3 100 fifo extra

expect_invalid_input "invalid_scheduler" "Invalid scheduler|Check last parameter|fifo|edf" \
	3 800 200 200 200 3 100 rr

expect_invalid_input "negative_numeric" "Should be number|Check parameter" \
	3 -800 200 200 200 3 100 fifo

expect_invalid_input "non_integer_numeric" "Should be number|Check parameter" \
	3 800x 200 200 200 3 100 fifo

expect_invalid_input "not_enough_coders" "Not enough coder" \
	1 800 200 200 200 3 100 fifo

# Per subject, invalid integers should be rejected.
expect_invalid_input "integer_overflow_argument" "Number too large|Invalid parameter|Check parameter" \
	3 2147483648 200 200 200 3 100 fifo

# ---- Functional behavior tests ----
check_contains "burnout_stops_simulation_fifo" "burned out" \
	3 50 200 200 200 0 100 fifo

check_not_contains "compiles_required_stops_without_burnout_fifo" "burned out" \
	3 1200 80 80 80 2 20 fifo

check_log_format "log_format_fifo" \
	3 1000 60 60 60 2 10 fifo

check_log_format "log_format_edf" \
	3 1000 60 60 60 2 10 edf

printf '\nResult: %d passed, %d failed\n' "$PASS_COUNT" "$FAIL_COUNT"
if [ "$FAIL_COUNT" -ne 0 ]; then
	exit 1
fi
exit 0
