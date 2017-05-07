#!/usr/bin/env bash

ecgen="../ecgen"
econvert="../econvert"
ASSERT="lib/assert.sh/assert.sh"
JSON="lib/JSON.sh/JSON.sh"

start_test() {
	echo
	printf "[*] Test %-20s" "${FUNCNAME[1]}"
}

strip_num() {
	num="$1"
	num="${num%\"}"
	num="${num#\"}"
	num="${num:2}"
	num="${num##+(0)}"
	echo $num
}