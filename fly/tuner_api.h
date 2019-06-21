#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <vector>


#include "fly/fly_platform.h"

// Data types and enums
#include "fly/fly_types.h"
#include "fly/enum/argument_access_type.h"
#include "fly/enum/argument_data_type.h"
#include "fly/enum/argument_memory_location.h"
#include "fly/enum/argument_upload_type.h"
#include "fly/enum/compute_api.h"
#include "fly/enum/global_size_type.h"
#include "fly/enum/logging_level.h"
#include "fly/enum/modifier_action.h"
#include "fly/enum/modifier_dimension.h"
#include "fly/enum/modifier_type.h"
#include "fly/enum/time_unit.h"


// Data holders
#include "fly/api/computation_result.h"
#include "fly/api/device_info.h"
#include "fly/api/dimension_vector.h"
#include "fly/api/output_descriptor.h"
#include "fly/api/platform_info.h"


// Stop conditions
#include "fly/api/stop_condition/configuration_duration.h"
#include "fly/api/stop_condition/configuration_count.h"
#include "fly/api/stop_condition/configuration_fraction.h"
#include "fly/api/stop_condition/tuning_duration.h"



// Support for 16-bit floating point data type
#include "fly/half.h"



namespace fly
{

using half_float::half;
class TunerCore;

/** @class Tuner
  * Class which serves as the main part of public API of Fly framework.
  */
class  Tuner
{
public:
   
   
    Tuner(const PlatformIndex platform, const DeviceIndex device, const ComputeAPI computeAPI);

  
    Tuner(const PlatformIndex platform, const DeviceIndex device, const ComputeAPI computeAPI, const uint32_t computeQueueCount);

    /** @fn ~Tuner()
      * Tuner destructor.
      */
    ~Tuner();

    /**
      * 从字符串内的源代码向tuner添加新的内核。要求指定内核名称和默认的全局和本地线程大小。
      * @param source Kernel 源代码。
      * @param kernelName 内核源代码中内核函数的名称
      * @param globalSize 基本内核全局大小的维度 (eg. grid size in CUDA).
      * @param localSize 基本内核本地大小的维度 (eg. block size in CUDA).
      * @return Id  KernelId
      */
    KernelId addKernel(const std::string& source, const std::string& kernelName, const DimensionVector& globalSize,
        const DimensionVector& localSize);

  
    KernelId addKernelFromFile(const std::string& filePath, const std::string& kernelName, const DimensionVector& globalSize,
        const DimensionVector& localSize);

    /** 
      * 通过提供相应的参数id为指定的内核设置内核参数。
      * @param id KernelId
      * @param argumentIds 指定内核使用的参数的id。id的顺序必须与指定的内核参数的顺序匹配
      */
    void setKernelArguments(const KernelId id, const std::vector<ArgumentId>& argumentIds);

    /** 为指定的内核添加常量。
    * @param id Id of kernel for which the constraint will be added.
    * @param parameterNames 参数的名称
    * @param constraintFunction 函数，如果提供的参数值组合有效，则返回true。返回false,否则.
    */
    void addConstraint(const KernelId id, const std::vector<std::string>& parameterNames,
        const std::function<bool(const std::vector<size_t>&)>& constraintFunction);


    /** 为指定的内核添加新的整数参数，提供参数名称和允许值的列表。
       当启动相应的内核时，参数将作为预处理器定义添加到内核源代码中。
      * @param id KernelId
      * @param parameterName  参数的名称
      * @param parameterValues int数组
      */
    void addParameter(const KernelId id, const std::string& parameterName, const std::vector<size_t>& parameterValues);

    
    void addParameterDouble(const KernelId id, const std::string& parameterName, const std::vector<double>& parameterValues);

   
    void addParameterPack(const KernelId id, const std::string& packName, const std::vector<std::string>& parameterNames);

    /** 为指定的内核设置线程修饰符函数。
      */
    void setThreadModifier(const KernelId id, const ModifierType modifierType, const ModifierDimension modifierDimension,
        const std::vector<std::string>& parameterNames, const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction);

 
    void setThreadModifier(const KernelId id, const ModifierType modifierType, const ModifierDimension modifierDimension,
        const std::string& parameterName, const ModifierAction modifierAction);

    void setLocalMemoryModifier(const KernelId id, const ArgumentId argumentId, const std::vector<std::string>& parameterNames,
        const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction);

   
    
    
    void setCompositionKernelThreadModifier(const KernelId compositionId, const KernelId kernelId, const ModifierType modifierType,
        const ModifierDimension modifierDimension, const std::vector<std::string>& parameterNames,
        const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction);

    void setCompositionKernelThreadModifier(const KernelId compositionId, const KernelId kernelId, const ModifierType modifierType,
        const ModifierDimension modifierDimension, const std::string& parameterName, const ModifierAction modifierAction);

  
    void setCompositionKernelLocalMemoryModifier(const KernelId compositionId, const KernelId kernelId, const ArgumentId argumentId,
        const std::vector<std::string>& parameterNames, const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction);

    void setCompositionKernelArguments(const KernelId compositionId, const KernelId kernelId, const std::vector<ArgumentId>& argumentIds);

    /** @fn template <typename T> ArgumentId addArgumentVector(const std::vector<T>& data, const ArgumentAccessType accessType)
      * Adds new vector argument to tuner. Makes copy of argument data, so the source data vector remains unaffected by tuner operations.
      * Argument data will be accessed from device memory during its usage by compute API.
      * @param data Argument data provided in std::vector. Provided data type must be trivially copyable. Bool data type is currently not supported.
      * @param accessType Access type of argument specifies whether argument is used for input or output. See ::ArgumentAccessType for more
      * information.
      * @return Id assigned to kernel argument by tuner. The id can be used in other API methods.
      */
    template <typename T> ArgumentId addArgumentVector(const std::vector<T>& data, const ArgumentAccessType accessType)
    {
        ArgumentDataType dataType = getMatchingArgumentDataType<T>();
        return addArgument(data.data(), data.size(), sizeof(T), dataType, ArgumentMemoryLocation::Device, accessType, ArgumentUploadType::Vector);
    }

    /** @fn template <typename T> ArgumentId addArgumentVector(std::vector<T>& data, const ArgumentAccessType accessType,
      * const ArgumentMemoryLocation memoryLocation, const bool copyData)
      * Adds new vector argument to tuner. Allows choice for argument memory location and whether argument data is copied to tuner.
      * @param data Argument data provided in std::vector. Provided data type must be trivially copyable. Bool data type is currently not supported.
      * @param accessType Access type of argument specifies whether argument is used for input or output. See ::ArgumentAccessType for more
      * information.
      * @param memoryLocation Memory location of argument specifies whether argument will be accessed from device or host memory during its usage
      * by compute API. See ::ArgumentMemoryLocation for more information.
      * @param copyData Flag which specifies whether the argument is copied inside tuner. If set to false, tuner will store reference of source data
      * vector and will access it directly during kernel launch operations. This results in lower memory overhead, but relies on a user to keep data
      * in source vector valid.
      * @return Id assigned to kernel argument by tuner. The id can be used in other API methods.
      */
    template <typename T> ArgumentId addArgumentVector(std::vector<T>& data, const ArgumentAccessType accessType,
        const ArgumentMemoryLocation memoryLocation, const bool copyData)
    {
        ArgumentDataType dataType = getMatchingArgumentDataType<T>();
        return addArgument(data.data(), data.size(), sizeof(T), dataType, memoryLocation, accessType, copyData);
    }

    /** @fn template <typename T> ArgumentId addArgumentScalar(const T& data)
      * Adds new scalar argument to tuner. All scalar arguments are read-only.
      * @param data Argument data provided as single scalar value. The data type must be trivially copyable. Bool data type is currently not
      * supported.
      * @return Id assigned to kernel argument by tuner. The id can be used in other API methods.
      */
    template <typename T> ArgumentId addArgumentScalar(const T& data)
    {
        ArgumentDataType dataType = getMatchingArgumentDataType<T>();
        return addArgument(&data, 1, sizeof(T), dataType, ArgumentMemoryLocation::Device, ArgumentAccessType::ReadOnly, ArgumentUploadType::Scalar);
    }

    /** @fn template <typename T> ArgumentId addArgumentLocal(const size_t localMemoryElementsCount)
      * Adds new local memory (shared memory in CUDA) argument to tuner. All local memory arguments are read-only and cannot be initialized
      * from host memory. In case of CUDA API usage, local memory arguments cannot be directly set as kernel function arguments. Setting a local
      * memory argument to kernel in CUDA means that corresponding amount of memory will be allocated for kernel to use. In that case, all local
      * memory argument ids should be specified at the end of the vector when calling setKernelArguments() method.
      * @param localMemoryElementsCount Specifies how many elements of provided data type the argument contains.
      * @return Id assigned to kernel argument by tuner. The id can be used in other API methods.
      */
    template <typename T> ArgumentId addArgumentLocal(const size_t localMemoryElementsCount)
    {
        ArgumentDataType dataType = getMatchingArgumentDataType<T>();
        return addArgument(localMemoryElementsCount, sizeof(T), dataType);
    }

    /** @fn void persistArgument(const ArgumentId id, const bool flag)
      * Controls whether specified vector argument is persisted inside a compute API buffer or not. Persisted arguments remain inside buffers even
      * after the execution of kernel utilizing these arguments has finished. Persistence of kernel arguments is switched off by default. Persistent
      * arguments can be useful during online tuning and regular kernel running, when kernel output is computed over multiple kernel launches in
      * different configurations. Note that persistent arguments are never utilized by reference kernels.
      * @param id Id of a vector argument.
      * @param flag Specifies whether argument should be persisted or not. If true, specified vector argument is immediately persisted. If false,
      * compute API buffer for specified argument is immediately destroyed.
      */
    void persistArgument(const ArgumentId id, const bool flag);

    /** @fn void downloadPersistentArgument(const OutputDescriptor& output) const
      * Downloads specified persistent argument from compute API buffer into specified memory location.
      * @param output User-provided memory location for persistent kernel argument which should be retrieved. See OutputDescriptor for more information.
      */
    void downloadPersistentArgument(const OutputDescriptor& output) const;

    /** @fn ComputationResult runKernel(const KernelId id, const std::vector<ParameterPair>& configuration,
      * const std::vector<OutputDescriptor>& output)
      * Runs specified kernel using provided configuration.
      * @param id Id of kernel which will be run.
      * @param configuration Configuration under which the kernel will be launched. See ParameterPair for more information.
      * @param output User-provided memory locations for kernel arguments which should be retrieved. See OutputDescriptor for more information.
      * @return Object containing information about computation using specified kernel configuration. See ComputationResult for more information.
      */
    ComputationResult runKernel(const KernelId id, const std::vector<ParameterPair>& configuration, const std::vector<OutputDescriptor>& output);

   

    /** @fn void setCompositionKernelProfiling(const KernelId compositionId, const KernelId kernelId, const bool flag)
      * Toggles profiling of a specific kernel inside kernel composition. This is useful if only some kernels inside the composition need to be
      * profiled. By default, profiling is enabled for all kernels inside newly added kernel compositions. Note that this method has no effect if
      * kernel profiling is completely disabled. See setKernelProfiling() method for more information.
      * @param compositionId Id of composition which includes the specified kernel.
      * @param kernelId Id of kernel inside the composition for which the profiling flag will be set.
      * @param flag If true, kernel profiling is enabled for specified kernel inside the composition. It is disabled otherwise.
      */
    void setCompositionKernelProfiling(const KernelId compositionId, const KernelId kernelId, const bool flag);

    /** @fn void setKernelProfilingCounters(const std::vector<std::string>& counterNames)
      * Specifies profiling counters that will be collected during kernel profiling. Note that not all profiling counters are available on all
      * devices.
      *
      * For the list of CUDA CUPTI profiling counters, see: https://docs.nvidia.com/cuda/cupti/index.html#metrics-reference
      * @param counterNames Names of counters that will be collected during kernel profiling.
      */
    void setKernelProfilingCounters(const std::vector<std::string>& counterNames);

  

   

    /** @fn std::string getKernelSource(const KernelId id, const std::vector<ParameterPair>& configuration) const
      * Returns kernel source with preprocessor definitions for specified kernel based on provided configuration.
      * @param id Id of kernel for which the source is returned.
      * @param configuration Kernel configuration for which the source will be generated. See ParameterPair for more information.
      * @return Kernel source with preprocessor definitions for specified kernel based on provided configuration.
      */
    std::string getKernelSource(const KernelId id, const std::vector<ParameterPair>& configuration) const;

   
    
   
    void setCompilerOptions(const std::string& options);

    void setKernelCacheCapacity(const size_t capacity);

   
    void printComputeAPIInfo(std::ostream& outputTarget) const;

  
    std::vector<PlatformInfo> getPlatformInfo() const;

   
    std::vector<DeviceInfo> getDeviceInfo(const PlatformIndex platform) const;

    
    DeviceInfo getCurrentDeviceInfo() const;

   
    void setAutomaticGlobalSizeCorrection(const bool flag);

   
    void setGlobalSizeType(const GlobalSizeType type);

    static void setLoggingLevel(const LoggingLevel level);

  
    static void setLoggingTarget(std::ostream& outputTarget);

    static void setLoggingTarget(const std::string& filePath);

private:
    // Pointer to implementation class
    std::unique_ptr<TunerCore> tunerCore;

    // Helper methods
    ArgumentId addArgument(void* data, const size_t numberOfElements, const size_t elementSizeInBytes, const ArgumentDataType dataType,
        const ArgumentMemoryLocation memoryLocation, const ArgumentAccessType accessType, const bool copyData);
    ArgumentId addArgument(const void* data, const size_t numberOfElements, const size_t elementSizeInBytes, const ArgumentDataType dataType,
        const ArgumentMemoryLocation memoryLocation, const ArgumentAccessType accessType, const ArgumentUploadType uploadType);
    ArgumentId addArgument(const size_t localMemoryElementsCount, const size_t elementSizeInBytes, const ArgumentDataType dataType);

    template <typename T> ArgumentDataType getMatchingArgumentDataType() const
    {
        if (!std::is_trivially_copyable<T>() || typeid(T) == typeid(bool))
        {
            std::cerr << "Unsupported argument data type" << std::endl;
            throw std::runtime_error("Unsupported argument data type");
        }

        if (!std::is_arithmetic<T>())
        {
            return ArgumentDataType::Custom;
        }

        if (sizeof(T) == 1 && std::is_unsigned<T>())
        {
            return ArgumentDataType::UnsignedChar;
        }
        else if (sizeof(T) == 1)
        {
            return ArgumentDataType::Char;
        }
        else if (typeid(T) == typeid(half))
        {
            return ArgumentDataType::Half;
        }
        else if (sizeof(T) == 2 && std::is_unsigned<T>())
        {
            return ArgumentDataType::UnsignedShort;
        }
        else if (sizeof(T) == 2)
        {
            return ArgumentDataType::Short;
        }
        else if (typeid(T) == typeid(float))
        {
            return ArgumentDataType::Float;
        }
        else if (sizeof(T) == 4 && std::is_unsigned<T>())
        {
            return ArgumentDataType::UnsignedInt;
        }
        else if (sizeof(T) == 4)
        {
            return ArgumentDataType::Int;
        }
        else if (typeid(T) == typeid(double))
        {
            return ArgumentDataType::Double;
        }
        else if (sizeof(T) == 8 && std::is_unsigned<T>())
        {
            return ArgumentDataType::UnsignedLong;
        }
        else if (sizeof(T) == 8)
        {
            return ArgumentDataType::Long;
        }

        std::cerr << "Unsupported argument data type" << std::endl;
        throw std::runtime_error("Unsupported argument data type");
    }
};

} // namespace fly
