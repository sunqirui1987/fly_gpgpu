## Fly_GpGpu 通用并行计算

支持 WINDOW，LINUX，MAC，IOS，Android平台



使用方法：

```
 int type = 0;
    if (argc >= 2)
    {
        type = std::stoul(std::string(argv[1]));
    }
    std::string kernel_file ;
    fly::ComputeAPI kernel_type;
    std::string kernel_function;
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
            
        default:
            break;
    }
    
    
    demo1_running_kernel(0,0,kernel_file, kernel_function, kernel_type);
```



```c++
int demo1_running_kernel(fly::PlatformIndex platformIndex, fly::DeviceIndex deviceIndex,
                         std::string kernelFile,
                         std::string kernelFunction,
                         fly::ComputeAPI type)
{
 

    // 声明内核参数和数据变量。
    const size_t numberOfElements = 1024 * 1024 ;
    
    // 工作组数量和维度
    // sizes: local_size_x = LOCAL_SIZE_X, local_size_y = LOCAL_SIZE_Y, local_size_z = LOCAL_SIZE_Z
    const fly::DimensionVector workGroupDimensions(6);
    const fly::DimensionVector ndRangeDimensions(numberOfElements / workGroupDimensions.getSizeX());
    
    
    
    std::vector<float> a(numberOfElements);
    std::vector<float> b(numberOfElements);
    std::vector<float> result(numberOfElements, 0.0f);

    // 初始化数据
    for (size_t i = 0; i < numberOfElements; i++)
    {
        a.at(i) = static_cast<float>(i);
        b.at(i) = static_cast<float>(i + 1);
    }

    
    fly::Tuner tuner(platformIndex, deviceIndex,  type);

    fly::KernelId kernelId = tuner.addKernelFromFile(kernelFile, kernelFunction, ndRangeDimensions, workGroupDimensions);


    //传参
    fly::ArgumentId aId = tuner.addArgumentVector(a, fly::ArgumentAccessType::ReadOnly);
    fly::ArgumentId bId = tuner.addArgumentVector(b, fly::ArgumentAccessType::ReadOnly);
    fly::ArgumentId resultId = tuner.addArgumentVector(result, fly::ArgumentAccessType::WriteOnly);

    // 通过提供添加内核的id来设置参数。id的顺序需要匹配  绑定的顺序。
    tuner.setKernelArguments(kernelId, std::vector<fly::ArgumentId>{aId, bId, resultId});
    
    
    tuner.setAutomaticGlobalSizeCorrection(true);
    
    
    // 运行指定的内核。
    tuner.runKernel(kernelId, {}, std::vector<fly::OutputDescriptor>{fly::OutputDescriptor(resultId, result.data())});
    
    for (size_t i = 0; i < 10; i++)
    {
        std::cout << result.at(i) << " ";
    }
    std::cout << std::endl;

    return 0;
    
}

```



