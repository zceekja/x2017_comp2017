#!/bin/bash

# Trigger all your test cases with this script

for i in tests/*in; do

	temp="${i%.in}"
	args=$(cat ${temp}.args)

	echo "running ${temp}"
	diff ${temp}.out <(eval ./timetable $args < ${temp}.in)
	sleep 0.5
	echo "${temp} ended"
	echo "-------------------"

done

echo "Finished!"