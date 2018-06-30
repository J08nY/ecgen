#!/bin/bash
# Transform the JSON output of ecgen into CSV.
# For F_p curves:
#    p, a, b, G.x, G.y, G.order, G.cofactor
# For F_2^m curves:
#    m, e1, e2, e3, a, b, G.x, G.y, G.order, G.cofactor
#
# Where G is the first subgroup in "subgroups".
jq -r -M ".[0] | (if (.field | has(\"p\")) then [.field.p] else [.field.m, .field.\"e1\", .field.\"e2\", .field.\"e3\"] end + [.a, .b, .subgroups[0].x, .subgroups[0].y, .subgroups[0].order, .subgroups[0].cofactor]) | join(\",\")"