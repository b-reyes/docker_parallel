#!/bin/sh

a=1
while [ "$a" -lt 5 ]    # this is loop1
do
    run_time=$(make run np="$a" N=1000000)
    $run_time >> out.txt
    a=`expr $a + 1`
done

T1=$(head -n 1 out.txt)

echo "Calculating the speedup:"
while read line; do    
    echo $T1/$line | bc -l 
done < out.txt

rm out.txt