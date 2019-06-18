//
//  shaderrc.hpp
//  flygpgpu_lib
//
//  Created by qiruisun on 2019/5/16.
//  Copyright © 2019 detu. All rights reserved.
//
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>

#include <MoltenVK/mvk_vulkan.h>

namespace fly {
    void init_glslang();
    
    void finalize_glslang();
    
    bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, std::string pshader, std::vector<unsigned int> &spirv);

    
}
