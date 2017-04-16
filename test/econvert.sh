#!/usr/bin/env bash

. lib/assert.sh/assert.sh -v
econvert="../econvert"

function runs() {
	assert_raises "${econvert} --help"
	assert_raises "${econvert} --version"
}

runs
assert_end econvert
