#pragma once

#include <fstream>
#include <memory>
#include <vector>
#include "fly/api/computation_result.h"
#include "fly/compute_engine/compute_engine.h"
#include "fly/enum/compute_api.h"
#include "fly/kernel/kernel_manager.h"
#include "fly/kernel_argument/argument_manager.h"
#include "fly/tuning_runner/kernel_runner.h"
#include "fly/utility/logger.h"
#include "fly/utility/result_loader.h"
#include "fly/utility/result_printer.h"
#include "fly/fly_types.h"

namespace fly
{

class TunerCore
{
public:
    // Constructor
    explicit TunerCore(const PlatformIndex platform, const DeviceIndex device, const ComputeAPI computeAPI, const uint32_t queueCount);

    // Kernel manager methods
    KernelId addKernel(const std::string& source, const std::string& kernelName, const DimensionVector& globalSize,
        const DimensionVector& localSize);
    KernelId addKernelFromFile(const std::string& filePath, const std::string& kernelName, const DimensionVector& globalSize,
        const DimensionVector& localSize);
  
    void addParameter(const KernelId id, const std::string& parameterName, const std::vector<size_t>& parameterValues);
    void addParameter(const KernelId id, const std::string& parameterName, const std::vector<double>& parameterValues);
    void addConstraint(const KernelId id, const std::vector<std::string>& parameterNames,
        const std::function<bool(const std::vector<size_t>&)>& constraintFunction);
    void addParameterPack(const KernelId id, const std::string& packName, const std::vector<std::string>& parameterNames);
    void setThreadModifier(const KernelId id, const ModifierType modifierType, const ModifierDimension modifierDimension,
        const std::vector<std::string>& parameterNames, const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction);
    void setLocalMemoryModifier(const KernelId id, const ArgumentId argumentId, const std::vector<std::string>& parameterNames,
        const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction);
    void setCompositionKernelThreadModifier(const KernelId compositionId, const KernelId kernelId, const ModifierType modifierType,
        const ModifierDimension modifierDimension, const std::vector<std::string>& parameterNames,
        const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction);
    void setCompositionKernelLocalMemoryModifier(const KernelId compositionId, const KernelId kernelId, const ArgumentId argumentId,
        const std::vector<std::string>& parameterNames, const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction);
    void setKernelArguments(const KernelId id, const std::vector<ArgumentId>& argumentIds);
    void setCompositionKernelArguments(const KernelId compositionId, const KernelId kernelId, const std::vector<ArgumentId>& argumentIds);
    std::string getKernelSource(const KernelId id, const std::vector<ParameterPair>& configuration) const;
    void setCompositionKernelProfiling(const KernelId compositionId, const KernelId kernelId, const bool flag);

    // Argument manager methods
    ArgumentId addArgument(void* data, const size_t numberOfElements, const size_t elementSizeInBytes, const ArgumentDataType dataType,
        const ArgumentMemoryLocation memoryLocation, const ArgumentAccessType accessType, const ArgumentUploadType uploadType,
        const bool copyData);
    ArgumentId addArgument(const void* data, const size_t numberOfElements, const size_t elementSizeInBytes, const ArgumentDataType dataType,
        const ArgumentMemoryLocation memoryLocation, const ArgumentAccessType accessType, const ArgumentUploadType uploadType);

    // Kernel runner methods
    ComputationResult runKernel(const KernelId id, const std::vector<ParameterPair>& configuration, const std::vector<OutputDescriptor>& output);


    // Result printer methods
    void setPrintingTimeUnit(const TimeUnit unit);
    void setInvalidResultPrinting(const bool flag);
    void printResult(const KernelId id, std::ostream& outputTarget, const PrintFormat format) const;
    void printResult(const KernelId id, const std::string& filePath, const PrintFormat format) const;

    // Compute engine methods
    void setCompilerOptions(const std::string& options);
    void setGlobalSizeType(const GlobalSizeType type);
    void setAutomaticGlobalSizeCorrection(const bool flag);
    void setKernelCacheCapacity(const size_t capacity);
    void persistArgument(const ArgumentId id, const bool flag);
    void downloadPersistentArgument(const OutputDescriptor& output) const;
    void printComputeAPIInfo(std::ostream& outputTarget) const;
    std::vector<PlatformInfo> getPlatformInfo() const;
    std::vector<DeviceInfo> getDeviceInfo(const PlatformIndex platform) const;
    DeviceInfo getCurrentDeviceInfo() const;
    void setKernelProfilingCounters(const std::vector<std::string>& counterNames);

    // Logger methods
    static void setLoggingLevel(const LoggingLevel level);
    static void setLoggingTarget(std::ostream& outputTarget);
    static void setLoggingTarget(const std::string& filePath);
    static void log(const LoggingLevel level, const std::string& message);

private:
    // Attributes
    ArgumentManager argumentManager;
    KernelManager kernelManager;
    ResultPrinter resultPrinter;
    std::unique_ptr<ComputeEngine> computeEngine;
    std::unique_ptr<KernelRunner> kernelRunner;
};

} // namespace fly
