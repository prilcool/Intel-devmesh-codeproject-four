# Intel-devmesh-codeproject-four
simulate rocket launch program article can be found on 
https://devmesh.intel.com/projects/rocket-projection-using-quadratics


# SCENARIO
Lets say cern & spacex does a rocket launch and from test runs previously have provided a quadratic equation that measurers height in meters above sea level as a function of time.

but since the last flight the engineering team has made some improvements and provided you with a adjusted equation h(t)=-4.9^2+310t+332

Now its time for the live launch of the second test run so your job is to run a simulation with the new equation on:

a) How high will the rocket reach maximum rocket altitude?

b) What is the time it will take to reach max height ?

c) What time will the rocket splash down into the ocean ?

Use Quadratics to simulate a rocket launch with a known equation from previous test launch , and determine maximum rocket altitude, the time it will take to reach max height,,The time it will take the rocket splash down into the ocean .

## Usage instructions :
Copy the entire structure including all files to  Intel dev cloud .

Ensure that the  Python 3.7 (Intel OneApi)  kernal is running

Ensure that you are using the q file ,simulate_rocket_launch.sh and Makefile that is provided with this sourcecode.

Ensure that file exist in  lab/simulate_rocket_launch.cpp


Run the following  jupyter notebook  Interest-on-account-audit-onemillion_records.ipynb

## Than Run  the following command
     ! chmod 755 q; chmod 755 simulate_rocket_launch.sh;if [ -x "$(command -v qsub)" ]; then ./q simulate_rocket_launch.sh; else ./simulate_rocket_launch.sh; fi 
     
## Results
    u60146 is compiling your Rocket launch simulation program.
   Simulated Result for the next rocket launch :
   The rocket reaches maximum height at 31.6327 seconds , Maximum height rocket travels is 5235.06 meters  and rocket will splash down at  time  64.3878 seconds


## Cross architecture compatibility GPU & CPU
     This code can Run on both on CPU and GPU of below specs
     
     if you select a GPU device than Device: Intel(R) Graphics Gen9 [0x3e96] will process in 1 second.
     queue q(gpu_selector{});
    
    if you select a CPU device than Device: Intel(R) Xeon(R) E-2176G CPU @ 3.70GHz will process in 3 seconds.
     queue q(=cpu_selector{});
