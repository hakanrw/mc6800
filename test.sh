#!/bin/sh

mv code.asm code.bak

for i in tests/*.asm; do
	cp "$i" code.asm
	make -s run | tail -n12 > test.log
	expected_output="${i%.asm}.p"
	if diff -q test.log "$expected_output" > /dev/null; then
		echo "$i: PASS"
	else
		echo "$i: FAIL"
		diff -c test.log "$expected_output"
	fi
done

rm test.log
mv code.bak code.asm
