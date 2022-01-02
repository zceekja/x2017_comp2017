#!/bin/bash

# Trigger all your test cases with this script

declare -i x=1

declare -i y=0

# Test valid parser
for i in tests/objdump/*x2017
do
    name=${i%.*}
    echo Test $x $name
    echo expected:
    cat $name.asm
    echo Observed:
    ./objdump_x2017 $name.x2017
    a=$(diff <(./objdump_x2017 $name.x2017) <(cat $name.asm))
    if [[ $a == "" ]]
    then
	    echo "-------------------------------------------------------"
	    echo "	Passed!, result matched!"
	    echo "-------------------------------------------------------"
        y=$(( y + 1 ))
    else
	    echo "-------------------------------------------------------"	
	    echo "	Failed!, Output not matched"
	    echo "-------------------------------------------------------"
    fi	
    x=$(( x + 1 ))
    sleep 1
done

# Test valid output program
for i in tests/vm/*x2017
do
    name=${i%.*}
    echo Test $x $name
    echo expected:
    cat $name.out
    echo Observed:
    ./vm_x2017 $name.x2017
    a=$(diff <(./vm_x2017 $name.x2017) <(cat $name.out))
    if [[ $a == "" ]]
    then
	    echo "-------------------------------------------------------"
	    echo "	Passed!, result matched!"
	    echo "-------------------------------------------------------"
        y=$(( y + 1 ))
    else
	    echo "-------------------------------------------------------"	
	    echo "	Failed!, Output not matched"
	    echo "-------------------------------------------------------"
    fi	
    x=$(( x + 1 ))
    sleep 1
done


# Test error programs
for i in tests/vm_error/*x2017
do
    name=${i%.*}
    echo Test $x $name
    echo expected:
    cat $name.out
    echo Observed:
    ./vm_x2017 $name.x2017 2> error.txt
    cat error.txt
    
    a=$(diff <(cat error.txt) <(cat $name.out))
    if [[ $a == "" ]]
    then
	    echo "-------------------------------------------------------"
	    echo "	Passed!, result matched!"
	    echo "-------------------------------------------------------"
        y=$(( y + 1 ))
    else
	    echo "-------------------------------------------------------"	
	    echo "	Failed!, Output not matched"
	    echo "-------------------------------------------------------"
    fi	
    x=$(( x + 1 ))
    sleep 1
done

#Test missing argument

echo Test $x missing argument
echo expected:
cat ./tests/no_argument.out
echo Observed:

./vm_x2017 2> error.txt
cat error.txt
a=$(diff <(cat error.txt) <(cat ./tests/no_argument.out))
if [[ $a == "" ]]
then
    echo "-------------------------------------------------------"
    echo "	Passed!, result matched!"
    echo "-------------------------------------------------------"
    y=$(( y + 1 ))
else
    echo "-------------------------------------------------------"	
    echo "	Failed!, Output not matched"
    echo "-------------------------------------------------------"
fi	
x=$(( x + 1 ))
sleep 1

#Test missing file
echo Test $x missing file
echo expected:
cat ./tests/missing_file.out
echo Observed:
./vm_x2017 notexist.x2017 2> error.txt
cat error.txt

a=$(diff <(cat error.txt) <(cat ./tests/missing_file.out))
if [[ $a == "" ]]
then
    echo "-------------------------------------------------------"
    echo "	Passed!, result matched!"
    echo "-------------------------------------------------------"
    y=$(( y + 1 ))
else
    echo "-------------------------------------------------------"	
    echo "	Failed!, Output not matched"
    echo "-------------------------------------------------------"
fi	
sleep 1

rm error.txt


echo "  End of the test, Summary PASS( $y/$x )"
echo "-------------------------------------------------------"