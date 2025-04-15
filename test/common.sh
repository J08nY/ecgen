#!/usr/bin/env bash
####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017 J08nY
#
####

ecgen="../ecgen"
ASSERT="lib/assert.sh/assert.sh"
JSON="lib/JSON.sh/JSON.sh"


start_suite() {
	echo
	echo -n "######################################################################"
}

end_suite() {
	assert_end $1
	echo "######################################################################"
	echo
}

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

canonical_num() {
    num=$(strip_num "$1")
    echo "ibase=16;${num^^}" | bc
}

get_pari_order() {
  p=$(canonical_num $(echo $1 | ${JSON} -x field\",\"p | cut -f 2))
  a=$(canonical_num $(echo $1 | ${JSON} -x \"a | cut -f 2))
  b=$(canonical_num $(echo $1 | ${JSON} -x \"b | cut -f 2))
  echo "ellcard(ellinit([${a}, ${b}], ${p}))" | gp -q 2>/dev/null
}