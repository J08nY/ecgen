#!/usr/bin/env bash

. ./common.sh

function runs() {
	start_test
	assert_raises "${econvert} --help"
	assert_raises "${econvert} --version"
}

. ${ASSERT} -v
runs
assert_end econvert
