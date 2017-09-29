#!/usr/bin/env bash
####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####

. ./common.sh

function runs() {
	start_test
	assert_raises "${ecgen} --help"
	assert_raises "${ecgen} --version"
	assert_raises "${ecgen}" 64
}

function csv() {
	start_test
	assert_matches "${ecgen} --fp -tcsv --input=data/fp_10_a.csv.in 10" "$(cat data/fp_10_a.csv)"
	assert_matches "${ecgen} --f2m -tcsv --input=data/f2m_10_a.csv.in 10" "$(cat data/f2m_10_a.csv)"
}

function json() {
	start_test
	assert_raises "${ecgen} --fp -tjson --input=data/fp_10_a.csv.in 10"
	assert_raises "${ecgen} --f2m -tjson --input=data/f2m_10_a.csv.in 10"
	fp=$(${ecgen} --fp -tjson --input=data/fp_10_a.csv.in 10 2>/dev/null)
	f2m=$(${ecgen} --f2m -tjson --input=data/f2m_10_a.csv.in 10 2>/dev/null)
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

function exhaustive() {
	start_test
	assert_raises "${ecgen} --fp -r 10"
	assert_raises "${ecgen} --f2m -r 10"
	assert_raises "${ecgen} --fp -r -p 10"
	assert_raises "${ecgen} --f2m -r -u 10"
	assert_raises "${ecgen} --fp -r -i -u 10"
	assert_raises "${ecgen} --f2m -r -i -u 10"
	assert_raises "${ecgen} --fp -r -k 10 10"
	assert_raises "${ecgen} --fp -r -K 10"
	assert_raises "${ecgen} --f2m -r -K 10"
	assert_raises "${ecgen} --fp -r -K1 10"
	assert_raises "${ecgen} --f2m -r -K1 10"

	assert_raises "${ecgen} --fp -r --points=random 10"
	assert_raises "${ecgen} --fp -r --points=10random 10"
	assert_raises "${ecgen} --fp -r --points=prime 10"
	assert_raises "${ecgen} --fp -r --points=nonprime 10"
	assert_raises "${ecgen} --fp -r --points=all 10"
	assert_raises "${ecgen} --fp -r --points=none 10"

	assert_raises "${ecgen} --f2m -r --points=random 10"
	assert_raises "${ecgen} --f2m -r --points=10random 10"
	assert_raises "${ecgen} --f2m -r --points=prime 10"
	assert_raises "${ecgen} --f2m -r --points=nonprime 10"
	assert_raises "${ecgen} --f2m -r --points=all 10"
	assert_raises "${ecgen} --f2m -r --points=none 10"

	assert_raises "${ecgen} --fp -r -c 5 10"
}

function anomalous() {
	start_test
	assert_raises "${ecgen} --fp --anomalous -r 20"
	out=$(${ecgen} --fp -tjson --anomalous -r 20 2>/dev/null)
	p=$(echo $out | ${JSON} -x field\",\"p | cut -f 2)
	order=$(echo $out | ${JSON} -x ^0,\"order\" | cut -f 2)
	assert "strip_num $p" $(strip_num $order)
}

function invalid() {
	start_test
	assert_raises "${ecgen} --fp -r -i 10"
	assert_raises "${ecgen} --f2m -r -i 10"
	assert_raises "${ecgen} --fp --threads=2 -r -i -u 10"
	assert_raises "${ecgen} --f2m --threads=2 -r -i -u 10"
	assert_raises "${ecgen} --fp --threads=auto -r -i -u 10"
	assert_raises "${ecgen} --f2m --threads=auto -r -i -u 10"
}

function cli() {
	start_test
	assert_raises "${ecgen} --threads=a" 1
	assert_raises "${ecgen} --format=something" 1
	assert_raises "${ecgen} --koblitz=2" 1
	assert_raises "${ecgen} --points=something" 1
	assert_raises "${ecgen} --seed=some" 64
	assert_raises "${ecgen} 1 2 3" 64
	assert_raises "${ecgen} --fp --f2m 1" 1
}


. ${ASSERT} -v
start_suite
runs
csv
json
exhaustive
anomalous
invalid
cli
end_suite ecgen