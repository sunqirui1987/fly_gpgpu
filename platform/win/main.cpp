//
//  main.cpp
//  flygpgpu
//
//  Created by qiruisun on 2019/5/7.
//  Copyright © 2019 detu. All rights reserved.
//

#include <stdio.h>
#include <thread>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "fly_demo/demo1/demo1.hpp"

int main(int argc, char** argv)
{

   
    int type = 0;
    if (argc >= 2)
    {
        type = std::stoul(std::string(argv[1]));
    }
    std::string kernel_file ;
    fly::ComputeAPI kernel_type;
    std::string kernel_function;
    kernel_file = "E:\\gitlab\\fly_gpgpu\\fly_demo\\demo1\\cuda_kernel.cu";
    kernel_type = fly::ComputeAPI::CUDA;
    kernel_function = "vectorAddition";

    demo1_running_kernel(0,0,kernel_file, kernel_function, kernel_type);
 

    getchar();
    
}
