//
//  opencl_common.h
//  flygpgpu_lib
//
//  Created by qiruisun on 2019/6/14.
//  Copyright © 2019 detu. All rights reserved.
//

#pragma once

#ifdef FLY_PLATFORM_OPENCL



#ifdef __APPLE__

#include <OpenCL/cl.h>
#include <OpenCL/cl_gl.h>
#include <OpenCL/cl_gl_ext.h>
#include <OpenCL/cl_ext.h>

#else

#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <CL/cl_gl_ext.h>
#include <CL/cl_ext.h>

#endif



#endif /* FLY_PLATFORM_OPENCL */
