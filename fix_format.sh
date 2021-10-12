#!/bin/bash

# run c++ fix format 
find interpolator/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i --verbose

# run python fix format
black interpolator/
