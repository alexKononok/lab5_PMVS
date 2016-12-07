#!/bin/bash

function test {
	echo "$1" > /proc/first
	echo "$2" > /proc/operand
	echo "$3" > /proc/second
	res=`sudo cat /dev/result`
	echo "$1 $2 $3 = ${res}"
	if [ "${res}" == "$4" ]; then
		echo "true"
	else 
		echo "false"
	fi
}

test 1 + 1 2
test 1 - 9 -8
test -1 + 9 8
test 8 / 2 4
test -9 / 3 -3
test 2 '*' 3 6
test -2 '*' 3 -6
test -2 '*' -2 4
