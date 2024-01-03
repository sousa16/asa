#!/bin/bash

# Create a file to store the results
result_file="results.txt"
touch $result_file

# Run p3_time.py with each test file and append the result to the results file
for i in {1..15}
do
  input_file="test_$i.in"
  output=$(python3 p3_time.py < $input_file)
  echo "Test $i: $output" >> $result_file
done

# Display a message indicating that the tests have been completed
echo "Tests completed. Results are stored in $result_file"
