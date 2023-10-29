#!/bin/bash

executable_name="coordinate_geometry"

clear
cd build
gnome-terminal -- bash -c "gdb $executable_name"
