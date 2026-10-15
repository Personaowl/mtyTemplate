#!/usr/bin/env bash
set -euo pipefail
cd -- "$(dirname -- "$0")"
limit=${1:-1000}
seconds=${2:-3}
[[ "$limit" =~ ^[1-9][0-9]*$ ]] || { echo "limit must be positive" >&2; exit 2; }
for name in gen brute main; do
	g++ -std=c++17 -O2 -Wall -Wextra "$name.cpp" -o "$name" || { echo "compile failed: $name" >&2; exit 2; }
done
for ((seed=1; seed<=limit; seed++)); do
	printf '%s\n' "$seed" > stress.seed
	timeout "$seconds" ./gen "$seed" > stress.in 2> gen.err || { echo "generator failed: seed=$seed" >&2; exit 2; }
	timeout "$seconds" ./brute < stress.in > stress.ans 2> brute.err || { echo "brute failed: seed=$seed" >&2; exit 2; }
	timeout "$seconds" ./main < stress.in > stress.out 2> main.err || { echo "main failed: seed=$seed" >&2; exit 2; }
	if diff -u stress.ans stress.out > stress.diff; then
		echo "AC #$seed"
	else
		status=$?
		if ((status==1)); then echo "WA #$seed: inspect stress.in / stress.diff"; exit 1; fi
		echo "comparison failed" >&2; exit 2
	fi
done
