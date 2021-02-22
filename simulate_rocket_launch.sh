#!/bin/bash
source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1
/bin/echo "##" $(whoami) is compiling your Rocket launch simulation program.
dpcpp lab/simulate_rocket_launch.cpp -o bin/simulate_rocket_launch -lm -std=c++11
if [ $? -eq 0 ]; then bin/simulate_rocket_launch; fi

