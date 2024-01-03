#!/bin/bash

# Create a file to store the results
result_file="results.txt"
touch $result_file

# Run p1.exe with each test file and append the result to the results file
for i in {1..2}
do
  input_file="test_$i.in"
  output=$(./p2.exe < $input_file)
  echo "Test $i: $output" >> $result_file
done

# Display a message indicating that the tests have been completed
echo "Tests completed. Results are stored in $result_file"
