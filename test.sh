#!/bin/bash

executable_name="./user"

function_index="$1"

output_file="out_${function_index}.csv"

method=$1  # ksort or l_sort

# Clear the output file at the start of the script
> "$output_file"

for n in $(seq 1000 500 20000); do
    echo "Running $method with $n elements"
    sudo dmesg -C
    sudo ./user $method $n
    
    # Capture the last sorting time from dmesg and append to the CSV
    duration=$(sudo dmesg | grep "took" | awk '{print $4}')
    echo $duration
    echo "$n, $duration" >> "$output_file"
done
