#pragma once

#ifdef FLY_PLATFORM_CUDA

#include <string>
#include <cuda.h>
#include <nvrtc.h>
#ifdef FLY_PROFILING
#include <cupti.h>
#endif // FLY_PROFILING

namespace fly
{

std::string getCUDAEnumName(const CUresult value);
std::string getNvrtcEnumName(const nvrtcResult value);
void checkCUDAError(const CUresult value);
void checkCUDAError(const CUresult value, const std::string& message);
void checkCUDAError(const nvrtcResult value, const std::string& message);
float getEventCommandDuration(const CUevent start, const CUevent end);

#ifdef FLY_PROFILING
std::string getCUPTIEnumName(const CUptiResult value);
void checkCUDAError(const CUptiResult value, const std::string& message);
#endif // FLY_PROFILING

} // namespace fly

#endif // FLY_PLATFORM_CUDA
