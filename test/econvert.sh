#!/usr/bin/env bash

econvert="../econvert"

function runs() {
	${econvert} --help
	${econvert} --version
}

runs >/dev/null