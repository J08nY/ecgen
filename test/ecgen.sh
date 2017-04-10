#!/usr/bin/env bash

ecgen="../ecgen"

function runs() {
	${ecgen} --help
	${ecgen} --version
}

runs >/dev/null