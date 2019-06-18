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
        type = std::atoi(argv[1]);
    }
    std::string kernel_file = "";
    fly::ComputeAPI kernel_type = fly::ComputeAPI::OpenCL;
    std::string kernel_function = "";
    switch (type) {
      
        case 0:
            kernel_file = "/Users/qiruisun/github/fly_gpgpu/fly_demo/demo1/opencl_kernel.cl";
            kernel_type = fly::ComputeAPI::OpenCL;
            kernel_function = "vectorAddition";
            break;
        case 1:
            kernel_file = "/Users/qiruisun/github/fly_gpgpu/fly_demo/demo1/vulkan_kernel.glsl";
            kernel_type = fly::ComputeAPI::Vulkan;
            kernel_function = "main";
            break;
        case 2:
            kernel_file = "/Users/qiruisun/github/fly_gpgpu/fly_demo/demo1/cuda_kernel.cu";
            kernel_type = fly::ComputeAPI::CUDA;
            kernel_function = "vectorAddition";
            break;
            
        default:
            break;
    }
    
    
    demo1_running_kernel(0,0,kernel_file, kernel_function, kernel_type);
 
    
}
