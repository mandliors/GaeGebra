#!/bin/bash

cd build
gnome-terminal -- bash -c 'make && ./coordinate_geometry'

# if make failes
if [ $? -ne 0 ]; then
  echo "Make failed."
  read -s -n 1
fi

# if executable fails
if [ $? -ne 0 ]; then
  echo "Executable failed."
  read -s -n 1
fi