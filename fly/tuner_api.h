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




// Support for 16-bit floating point data type
#include "fly/half.h"



namespace fly
{

    using half_float::half;
    class TunerCore;


    class  Tuner
    {
    public:


        Tuner(const PlatformIndex platform, const DeviceIndex device, const ComputeAPI computeAPI);


        Tuner(const PlatformIndex platform, const DeviceIndex device, const ComputeAPI computeAPI, const uint32_t computeQueueCount);

     
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


        template <typename T> ArgumentId addArgumentVector(const std::vector<T>& data, const ArgumentAccessType accessType)
        {
            ArgumentDataType dataType = getMatchingArgumentDataType<T>();
            return addArgument(data.data(), data.size(), sizeof(T), dataType, ArgumentMemoryLocation::Device, accessType, ArgumentUploadType::Vector);
        }


        template <typename T> ArgumentId addArgumentVector(std::vector<T>& data, const ArgumentAccessType accessType,
            const ArgumentMemoryLocation memoryLocation, const bool copyData)
        {
            ArgumentDataType dataType = getMatchingArgumentDataType<T>();
            return addArgument(data.data(), data.size(), sizeof(T), dataType, memoryLocation, accessType, copyData);
        }

        template <typename T> ArgumentId addArgumentScalar(const T& data)
        {
            ArgumentDataType dataType = getMatchingArgumentDataType<T>();
            return addArgument(&data, 1, sizeof(T), dataType, ArgumentMemoryLocation::Device, ArgumentAccessType::ReadOnly, ArgumentUploadType::Scalar);
        }


        template <typename T> ArgumentId addArgumentLocal(const size_t localMemoryElementsCount)
        {
            ArgumentDataType dataType = getMatchingArgumentDataType<T>();
            return addArgument(localMemoryElementsCount, sizeof(T), dataType);
        }


        void persistArgument(const ArgumentId id, const bool flag);


        void downloadPersistentArgument(const OutputDescriptor& output) const;


        ComputationResult runKernel(const KernelId id, const std::vector<ParameterPair>& configuration, const std::vector<OutputDescriptor>& output);





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
