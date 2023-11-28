#!/bin/bash
for i in {1..15}
do
  ./gen $((i * 100)) $((i * 100)) 500 49 > test_$i.in
done