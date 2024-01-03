#!/bin/bash

for i in {1..10}
do
  SubN=$((i * 10))    # Number of sub-networks
  M=$((i * 5))        # Maximal number of individuals per sub-network
  m=$((M / 2))         # Minimal number of individuals per sub-network
  V=$((SubN * M))      # Number of individuals
  E=$((V * (V - 1) / 2))  # Assuming a complete graph for simplicity

  if [ "$V" -ge "$SubN" ] && [ "$m" -le "$M" ] && [ "$V" -ge "$((SubN * m))" ] && [ "$V" -le "$((SubN * M))" ]; then
    echo "Generating test_$i.in with V=$V, E=$E, SubN=$SubN, m=$m, M=$M"
    ./gen_tuganet.exe $V $E $SubN $m $M $seed > test_$i.in
  else
    echo "Error: Constraints not satisfied for iteration $i."
    exit 1
  fi
done