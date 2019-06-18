/** @file compute_api.h
  * Definition of enum for compute APIs supported by Fly library.
  */
#pragma once

namespace fly
{

/** @enum ComputeAPI
  * Enum for compute API used by Fly library. It is utilized during tuner creation.
  */
enum class ComputeAPI
{
    /** Tuner will use OpenCL as compute API.
      */
    OpenCL,

    /** Tuner will use CUDA as compute API.
      */
    CUDA,

    /** Tuner will use Vulkan as compute API.
    */
    Vulkan
};

} // namespace fly
