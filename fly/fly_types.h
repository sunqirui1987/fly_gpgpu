#pragma once

#include <stdio.h>
#include <memory>
#include <type_traits>
#include <cstdint>

namespace fly
{

/** @typedef ArgumentId
  * Data type for referencing kernel arguments in Fly.
  */
using ArgumentId = uint64_t;

/** @typedef DeviceIndex
  * Data type for referencing devices in Fly.
  */
using DeviceIndex = uint32_t;

/** @typedef KernelId
  * Data type for referencing kernels in Fly.
  */
using KernelId = uint64_t;

/** @typedef PlatformIndex
  * Data type for referencing platforms in Fly.
  */
using PlatformIndex = uint32_t;

/** @typedef QueueId
  * Data type for referencing compute API queues in Fly.
  */
using QueueId = uint32_t;

/** @typedef EventId
  * Data type for referencing compute API events in Fly.
  */
using EventId = uint64_t;
    
    
 
//create unique ptr
template<class _Tp, class... _Args>
inline std::unique_ptr<_Tp> MakeStdUnique(_Args&&... __args)
{
    return std::unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...));
}
    
    

} // namespace fly
