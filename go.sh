#!/bin/bash

sudo sh load.sh

function check {
    echo $1 > /dev/first
    echo $2 > /dev/operator
    echo $3 > /dev/second
    echo "$1 $2 $3 = $4"
	result=$(cat /proc/result)
    if [ "$4" == "$result" ]; then
        echo "True!"
    else
        echo "False!"
	echo "Expected:"
	echo "$4"
	echo "Getted:"
	echo "$result"
    fi
}

check 1 + 9 "10"
check 1 - 9 "-8"
check 2 p 2 "4"
check 2 / 2 "1"
check 1 / 0 "NaN"
check 1 + 1 "3"

sudo sh unload.sh

