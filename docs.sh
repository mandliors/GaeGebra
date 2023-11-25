#!/bin/bash

doxygen
cd docs
cd latex
make pdf
cp refman.pdf ../programmer_docs_long.pdf
