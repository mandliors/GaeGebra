#!/bin/bash

doxygen
cd docs
cd latex
make pdf
cp refman.pdf ../docs.pdf