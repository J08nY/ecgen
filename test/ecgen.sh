#!/usr/bin/env bash

. lib/assert.sh -v
ecgen="../ecgen"

function runs() {
	assert_raises "${ecgen} --help"
	assert_raises "${ecgen} --version"
	assert_raises "${ecgen}" 64
}

function csv() {
	assert_matches "${ecgen} --fp -tcsv --input=fp_10_a.csv.in 10" "$(cat fp_10_a.csv)"
	assert_matches "${ecgen} --f2m -tcsv --input=f2m_10_a.csv.in 10" "$(cat f2m_10_a.csv)"
}

runs
csv
assert_end ecgen
