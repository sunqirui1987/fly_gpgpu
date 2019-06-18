//
//  demo1.h
//  flygpgpu
//
//  Created by qiruisun on 2019/5/17.
//  Copyright © 2019 detu. All rights reserved.
//
#pragma once
#include <string>
#include "fly/tuner_api.h"


int demo1_running_kernel(fly::PlatformIndex platformIndex, fly::DeviceIndex deviceIndex,
                         std::string kernelFile,
                         std::string kernelFunction,
                         fly::ComputeAPI type);
