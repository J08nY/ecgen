#!/usr/bin/env bash
####
#
# ecgen, tool for generating Elliptic curve domain parameters
# Copyright (C) 2017, 2024 J08nY
#
####

. ./common.sh

function runs() {
    start_test
    assert_raises "${ecgen} --help"
    assert_raises "${ecgen} --version"
    assert_raises "${ecgen}" 64
}

function json() {
    start_test
    assert_raises "${ecgen} --fp --input=data/fp_10_a.csv.in 10"
    assert_raises "${ecgen} --f2m --input=data/f2m_10_a.csv.in 10"
    fp=$(${ecgen} --fp --input=data/fp_10_a.csv.in 10 2>/dev/null)
    f2m=$(${ecgen} --f2m --input=data/f2m_10_a.csv.in 10 2>/dev/null)
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
    assert_raises "${ecgen} --fp -r -p -u 10"
    assert_raises "${ecgen} --fp -r -B 5 -u 10"
    assert_raises "${ecgen} --fp -r -k 10 10"
    assert_raises "${ecgen} --f2m -r -k 10 10"
    assert_raises "${ecgen} --fp -r -K 10"
    assert_raises "${ecgen} --f2m -r -K 10"
    assert_raises "${ecgen} --fp -r -K1 10"
    assert_raises "${ecgen} --f2m -r -K1 10"

    assert_raises "${ecgen} --f2m -r -K1 101"

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

    assert_raises "${ecgen} --fp --random=seed,field,a,b,equation 10"

    assert_raises "${ecgen} --fp -r -c 5 10"

    assert_raises "${ecgen} --fp -r --metadata 10"
    assert_raises "${ecgen} --f2m -r --metadata 10"
}

function ansix962() {
    start_test
    assert_raises "${ecgen} --fp -r --ansi 10"
    assert_raises "${ecgen} --f2m -r --ansi 10"
}

function brainpool() {
    start_test
    assert_raises "${ecgen} --fp -r --brainpool 10"
    assert_raises "${ecgen} --f2m -r --brainpool 10" 1
    assert_raises "${ecgen} --fp -r --brainpool-rfc 10"
    assert_raises "${ecgen} --f2m -r --brainpool-rfc 10" 1
}

function nums() {
    start_test
    assert_raises "${ecgen} --fp --nums 10"
    assert_raises "${ecgen} --f2m --nums 10" 1
}

function anomalous() {
    start_test
    assert_raises "${ecgen} --fp --anomalous -r 20"
    for i in $(seq 10); do
      out=$(${ecgen} --fp --anomalous -r 20 2>/dev/null)
      p=$(echo $out | ${JSON} -x field\",\"p | cut -f 2)
      order=$(echo $out | ${JSON} -x ^0,\"order\" | cut -f 2)
      assert "strip_num $p" $(strip_num $order)
    done
}

function supersingular() {
    start_test
    assert_raises "${ecgen} --fp --supersingular -r -c 5 20"
    for i in $(seq 10); do
      out=$(${ecgen} --fp --supersingular -r 20 2>/dev/null)
      p=$(echo $out | ${JSON} -x field\",\"p | cut -f 2)
      order=$(echo $out | ${JSON} -x ^0,\"order\" | cut -f 2)
      order_m1=$(echo $(canonical_num $order) - 1 | bc)
      assert "canonical_num $p" $order_m1
    done

    assert_raises "${ecgen} --fp --supersingular --input=data/prime.in 64"

    assert_raises "${ecgen} --fp --supersingular -r -u --metadata 20"
    assert_raises "${ecgen} --fp --supersingular -r --points=random 16"
    assert_raises "${ecgen} --fp --supersingular -r --points=5random 16"
    assert_raises "${ecgen} --fp --supersingular -r --points=nonprime 16"
    assert_raises "${ecgen} --fp --supersingular -r --points=all 16"
    assert_raises "${ecgen} --fp --supersingular -r --points=none 20"
}

function family() {
    start_test
    assert_raises "${ecgen} --fp -r --family=BN 16"
    assert_raises "${ecgen} --fp -r --family=BLS12 16"
    assert_raises "${ecgen} --fp -r --family=BLS24 16"

    assert_raises "${ecgen} --fp --family=BN 16"    0 "0xe5a2"
    assert_raises "${ecgen} --fp --family=BLS12 16" 0 "0xafa2"
    assert_raises "${ecgen} --fp --family=BLS24 16" 0 "0x8278"
    assert_raises "${ecgen} --fp --family=KSS16 16" 0 "0x8acc"
    assert_raises "${ecgen} --fp --family=KSS18 16" 0 "0xd2ac"
    assert_raises "${ecgen} --fp --family=KSS36 16" 0 "0xf07f"
    assert_raises "${ecgen} --fp --family=KSS40 16" 0 "-0xb18f"
}

function invalid() {
    start_test
    assert_raises "${ecgen} --fp -r -i 10"
    assert_raises "${ecgen} --f2m -r -i 10"
    assert_raises "${ecgen} --fp -r -i5-15 10"
    assert_raises "${ecgen} --f2m -r -i5-15 10"
    assert_raises "${ecgen} --fp -r -i17 10"
    assert_raises "${ecgen} --f2m -r -i17 10"
    assert_raises "${ecgen} --fp --threads=2 -r -i -u 10"
    assert_raises "${ecgen} --f2m --threads=2 -r -i -u 10"
    assert_raises "${ecgen} --fp --threads=auto -r -i -u 10"
    assert_raises "${ecgen} --f2m --threads=auto -r -i -u 10"
}

function twist() {
    start_test
    assert_raises "${ecgen} --fp --twist -r 10"
    assert_raises "${ecgen} --f2m --twist -r 10"
}

function cli() {
    start_test
    assert_raises "${ecgen} --threads=a" 1
    assert_raises "${ecgen} --koblitz=2" 1
    assert_raises "${ecgen} --points=something" 1
    assert_raises "${ecgen} --seed=some" 64
    assert_raises "${ecgen} 1 2 3" 64
    assert_raises "${ecgen} --fp --f2m 1" 1
    assert_raises "${ecgen} --brainpool --anomalous --fp 10" 1
    assert_raises "${ecgen} --brainpool=01234 --fp 10" 1
    assert_raises "${ecgen} --brainpool-rfc=01234 --fp 10" 1
    assert_raises "${ecgen} --ansi=01234 --fp 10" 1
    assert_raises "${ecgen} --hex-check=not_hex --fp 10" 1
    assert_raises "${ecgen} abc" 1
    assert_raises "${ecgen} --supersingular --f2m 10" 1
    assert_raises "${ecgen} --fp --order=not_a_number 32" 1
}

function hex() {
    start_test
    assert_raises "${ecgen} --fp -r --hex-check=\"abc\" 32 | grep \"abc\""
}

function cm() {
    start_test
    assert_raises "${ecgen} --fp --order=2147483723 32"
    assert_raises "${ecgen} --fp --order=123456789012345678901234567890123456789012345678901234568197 --threads=5 196"
    assert_raises "${ecgen} --fp --order=46874566546,3546,3125 64"
    assert_raises "${ecgen} --fp -u --order=46874566546,3546,3125 64"
    assert_raises "${ecgen} --fp --order=0 16" 1
    assert_raises "${ecgen} --fp --order=0x1000 8" 1
}

function cm_orders() {
  start_test
  for i in $(seq 5 100); do
    out=$(timeout -k 4 3 ${ecgen} --fp -n $i --points=none 6 2>/dev/null)
    if [[ -z "$out" || "$out" = "[" ]]; then
      continue
    fi
    order=$(echo $out | ${JSON} -x ^0,\"order\" | cut -f 2)
    pari_order=$(get_pari_order "$out")
    assert "canonical_num $order" "$pari_order"
  done

  prime_orders=(45678945611413 47889465415131 78246132456157 3879641663983 134537095890397 3790687732807)
  for ord in "${prime_orders[@]}"; do
    out=$(${ecgen} --fp -r --order=$ord 64 2>/dev/null)
    p=$(echo $out | ${JSON} -x field\",\"p | cut -f 2)
    order=$(echo $out | ${JSON} -x ^0,\"order\" | cut -f 2)
    assert "canonical_num $order" $ord
  done

  composite_orders=(106618070007935 32268705670290 78286235471710 93953327960423 17042092126557 43615536370894)
  for ord in "${composite_orders[@]}"; do
    out=$(${ecgen} --fp -r --order=$ord 64 2>/dev/null)
    p=$(echo $out | ${JSON} -x field\",\"p | cut -f 2)
    order=$(echo $out | ${JSON} -x ^0,\"order\" | cut -f 2)
    assert "canonical_num $order" $ord
  done
}

function secg() {
    function test_order() {
        name="${1}"
        field="${2}"
        bits="${3}"
        result=$(${ecgen} --${field} --input=data/${name}.csv.in ${bits} 2>/dev/null)
        result_order=$(cat data/${name}.order.txt)
        assert_matches "${JSON} -x ^0,\\\"order\\\"" "${result_order}" "${result}"
    }
    start_test
    test_order secp112r1 fp 112
    test_order secp128r1 fp 128
    test_order sect163k1 f2m 163
    test_order sect163r1 f2m 163
    test_order secp160k1 fp 160
}

if [ ! -f $ASSERT ]; then
	echo "$ASSERT not found. It is a git submodule from https://github.com/J08nY/assert.sh @679ed66 that should be at $(dirname $ASSERT)."
	exit 1
fi
if [ ! -f $JSON ]; then
	echo "$JSON not found. It is a git submodule from https://github.com/jimklimov/JSON.sh @679ed66 that should be at $(dirname $JSON)."
	exit 1
fi

. ${ASSERT} -v
start_suite
runs
json
exhaustive
ansix962
brainpool
nums
anomalous
supersingular
family
invalid
twist
cli
hex
cm
cm_orders
secg
end_suite ecgen
