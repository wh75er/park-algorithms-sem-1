#!/bin/sh

volume=5
input="${volume} ${volume}\n"

for i in $(seq 1 $volume); do 
  input="${input} 9999"
  #echo -n "$i ";
done

for i in $(seq 1 $volume); do 
  input="${input} 10000"
#  echo -n "$i ";
done

input="${input}\n"

echo "Starting..."
echo -e $input | ./a.out
echo "Finished! Exiting..."
