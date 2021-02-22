
//==============================================================
// Copyright © 2021 Intel Corporation
// Author:Prilvesh Krishna
// Email:prilcool@hotmail.com    
// Linkedin:https://www.linkedin.com/in/prilvesh-k-4349ba54/
// Date:21/02/2020        
// SPDX-License-Identifier: MIT
// =============================================================


// Step 1 Include necessary headers and name spaces 

#include <CL/sycl.hpp>
#include <array>
#include <iostream>
#include <cmath>
#include<math.h>
#include<iomanip>
#include<limits>
#include <chrono>

using namespace sycl;


// declare functions and parameters 
double pow(double base_unit, double power_unit);
double sq_root( double n);
void find_max_altitude_and_splashdown_time(cl::sycl::accessor<double, 1, cl::sycl::access::mode::write>max_flight_p,cl::sycl::accessor<double, 1, cl::sycl::access::mode::write> splashdown_p,
                                           cl::sycl::accessor<double, 1, cl::sycl::access::mode::write> maximum_altitude_time_p,int x,double a,double b,double c);

int main()
{
    
    //declare and intilize variables 
    const int N=1;
    double rocket_max_altitude[N];
    double rocket_splashdown_time[N];
    double maximum_altitude_time[N];
    
    for(int i=0;i<N;i++)
    {
       rocket_max_altitude[i]=i;
    }
    
    
    // using gpu as processing device  Device: Intel(R) Graphics Gen9 [0x3e96] since it supports doubles sometimes this device is not available so theres a long wait .
    //gpu_selector device_selector;
    
    //or you can use cpu  Device: Intel(R) Xeon(R) E-2176G CPU @ 3.70GHz this device is often avilable.
     cpu_selector device_selector;
      
    //Lets say cern & spacex does a rocket launch and from test runs previously have provided a quadratic equation that measusers height in meters above sea level as a function of time.
    // but since the last flight the engineering team has made some improvements and provided you with a adjusted equation h(t)=-4.9^2+310t+332
    // Now its time for the live launch of the second test run so your job is to run a simulation with the new equation on:
    // a) How high will the rocket reach maximum rocket altitude? 5235.06 meters
    // b) What is the time it will take to reach max height ?  31.6327 seconds 
    // c) What time will the rocket splash down into the ocean ? 64.3187 seconds

    //h(t)=-4.9^2+310t+332  rocket 1
    //h(t)=−4.9^2+160t+290  rocket 2

    // set up work group 
    queue q(device_selector);
    {
        // declare buffers
        buffer<double ,1> a_buff(rocket_max_altitude,range<1> (N));
        buffer<double ,1> ac_buff(rocket_splashdown_time,range<1> (N));
        buffer<double ,1> ad_buff(maximum_altitude_time,range<1> (N));
       
        q.submit([&](handler &h)
        {
            //declare accessors
            auto aa=a_buff.get_access<access::mode::write>(h);
            auto ac=ac_buff.get_access<access::mode::write>(h);
            auto ad=ad_buff.get_access<access::mode::write>(h);
            
            // setup to process work parallely if you have multiple quadratic equations to process in our case we only have one equation to process. 
            h.parallel_for(range<1>{N},[=](id<1> item)
            {
                int index=item;
             // feed a b c values quadratic equation values into our function and call it .
        
               find_max_altitude_and_splashdown_time(aa,ac,ad,index,-4.9,310,332);
           
            // below is  alternate equation for testing      
            // find_max_altitude_and_splashdown_time(aa,ac,ad,index,-4.9,160,290);
               
            });
      });
    }
    
    
    //out put results onto screen 
    std::cout<<"Simulated Result for the next rocket launch :"<<std::endl;
    for(int i=0;i<N;i++)
    {
        std::cout<<"The rocket reaches maximum height at "<<maximum_altitude_time[i]<<" seconds ,"<<" Maximum height rocket travels is "
        <<rocket_max_altitude[i]<<" meters "<<" and rocket will splash down at  time  "<<rocket_splashdown_time[i] <<" seconds"<<std::fixed << std::setprecision(6)<< std::endl;
    }
    std::cout<<"\n"<<std::endl;
}


// define our function and formula to do Quadratic calulations and solve for time at which rocket reaches max height , maximum height rocket can fly and  time till rocket splashes into ocean

void find_max_altitude_and_splashdown_time(cl::sycl::accessor<double, 1, cl::sycl::access::mode::write> max_flight_p,
                                           cl::sycl::accessor<double, 1, cl::sycl::access::mode::write> splashdown_p,cl::sycl::accessor<double, 1, cl::sycl::access::mode::write>maximum_altitude_time_p,
                                           int x,double a,double b,double c){
    
    //solve using quadratic formula it can have two possible solutions
    //note pow and sqrt were giving issues when using gpu so i had to make my own functions .
    
      double t_plus_sqra=sq_root(double (pow(b,2)-4*(a)*(c)));
      double t_minus_sqra=sq_root(double (pow(b,2)-4*(a)*(c)));
    
    
    double tplus=((-b)+t_plus_sqra)/(2*(a)); //-1.053427183
    double tminus=((-b)-t_minus_sqra)/(2*(a));//64.3187
         
    
    // time cannot be negative so we choose which ever solution has a time of more than 0 seconds
    double real_positive_time=0;
    if(tplus>0){
        real_positive_time=tplus;
    }else{
        
        real_positive_time=tminus; 
    }
    
    
    double max_height_time=-(b)/(2*(a));//31.63 time it takes to reach max height
    
    double max_flight=-4.9*pow(max_height_time,2)+b*(max_height_time)+c; // maximum height rocket can fly
    
    max_flight_p[x]= max_flight; //maximum height rocket can fly
    splashdown_p[x]=real_positive_time; // time till rocket splashes into ocean
   maximum_altitude_time_p[x]=max_height_time; //time at which rocket reaches max height 
}

// we create a custom pow function because GPU gives pow() not found.
double pow(double base_unit, double power_unit){
    double result=base_unit;
    int i=1;
    while(i<power_unit){
        result=result*base_unit;
        i++;
    }
    
  return result;  
}

// we create a custom sqrt function because GPU gives sqrt() not found but if u change to cpu it works ok strange.
//adapted from user someone https://stackoverflow.com/questions/17969168/compile-error-using-sqrt-in-c
double sq_root(double n)
{
    double count=0;
    int i = 0;
    double sum=0;
    for(i=1;sum<=n;i+=2)
    {
        sum+=i;
        count++;
    }
    return (count);
}

