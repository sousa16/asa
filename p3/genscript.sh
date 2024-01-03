#!/bin/bash

for i in {1..15}; do
  num_toys=$((i * 10))          # Number of different toys
  num_special_packages=$((i * 5))  # Number of special packages
  max_toys_per_day=$((i * 30))  # Maximum number of toys that can be produced per day

  
  echo "Generating test_$i.in with num_toys=$num_toys, num_special_packages=$num_special_packages, max_toys_per_day=$max_toys_per_day"
  ./gen_ubiquity.exe $num_toys $num_special_packages 0 $max_toys_per_day 75 49 > test_$i.in
done

