#!/bin/bash

# UDP socket + conditional mutex tester
# Usage:
#   ./tester.sh [period]
#
# Where:
#   period: Period to send messages (default: 1 (second))

period=1
if [ -n "$1" ]; then
    period="$1"
fi

counter=1
while true
do
    echo $counter | nc -u localhost 1234 -w0
    counter=$(expr $counter + 1)
    sleep $period
done

