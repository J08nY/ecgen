#!/usr/bin/env bash

ecgen="../ecgen"
econvert="../econvert"
ASSERT="lib/assert.sh/assert.sh"
JSON="lib/JSON.sh/JSON.sh"

start_test() {
	echo
	printf "[*] Test %-20s" "${FUNCNAME[1]}"
}