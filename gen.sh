#!/bin/bash

if [ "$#" -lt 3 ]; then
	echo "gen.sh [-l|-s|-p] [num_curves] [prime_size] [timeout]"
	exit
fi

option=$1
keys=$2
bits=$3
timeout=$4

time for (( i=1; i <= "$keys"; i++ )); do
p=$(openssl prime -generate -hex -bits "$bits");
a=$(openssl rand -hex $(($bits / 8)));
b=$(openssl rand -hex $(($bits / 8)));
params="$p\n$a\n$b\n";

if [ "$#" -eq 4 ]; then
	res=$(echo -e "$params" | timeout "$timeout" ecgen "$option");
else
	res=$(echo -e "$params" | ecgen "$option");
fi

if [ "$?" -ne 0 ]; then
    i=$((i - 1));
else
    r=($res);
    echo "${r[3]}" | tee -a "$2b.curves";
fi
done

