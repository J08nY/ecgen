#!/usr/bin/env bash

. ./common.sh

function runs() {
	start_test
	assert_raises "${ecgen} --help"
	assert_raises "${ecgen} --version"
	assert_raises "${ecgen}" 64
}

function csv() {
	start_test
	assert_matches "${ecgen} --fp -tcsv --input=fp_10_a.csv.in 10" "$(cat fp_10_a.csv)"
	assert_matches "${ecgen} --f2m -tcsv --input=f2m_10_a.csv.in 10" "$(cat f2m_10_a.csv)"
}

function json() {
	start_test
	fp=$(${ecgen} --fp -tjson --input=fp_10_a.csv.in 10)
	f2m=$(${ecgen} --f2m -tjson --input=f2m_10_a.csv.in 10)
	assert_raises "${JSON}" 0 "${fp}"
	assert_matches "${JSON} -x field\\\",\\\"p" "0x000b" "${fp}"
	assert_matches "${JSON} -x \\\"a\\\"" "0x0001" "${fp}"
	assert_matches "${JSON} -x \\\"b\\\"" "0x0002" "${fp}"
	assert_matches "${JSON} -x \\\"order\\\"" "0x0010" "${fp}"

	assert_raises "${JSON}" 0 "${f2m}"
	assert_matches "${JSON} -x field\\\",\\\"m" "0xa" "${f2m}"
	assert_matches "${JSON} -x field\\\",\\\"e1" "0x3" "${f2m}"
	assert_matches "${JSON} -x field\\\",\\\"e2" "0x0" "${f2m}"
	assert_matches "${JSON} -x field\\\",\\\"e3" "0x0" "${f2m}"
	assert_matches "${JSON} -x \\\"a\\\"" "0x02ed" "${f2m}"
	assert_matches "${JSON} -x \\\"b\\\"" "0x00b7" "${f2m}"
	assert_matches "${JSON} -x \\\"order\\\"" "0x3de" "${f2m}"
}

. ${ASSERT} -v
runs
csv
json
assert_end ecgen
