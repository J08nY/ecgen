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
	assert_raises "${econvert} --help"
	assert_raises "${econvert} --version"
}

. ${ASSERT} -v
runs
assert_end econvert
