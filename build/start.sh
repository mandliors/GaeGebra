#!/bin/bash
clear
project_name="$1"
gnome-terminal -- bash -c "make && ./$project_name"
#;exec bash
