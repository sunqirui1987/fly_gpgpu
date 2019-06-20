#include "fly/compute_engine/cuda/cuda_engine.h"
#include "fly/compute_engine/opencl/opencl_engine.h"
#include "fly/compute_engine/vulkan/vulkan_engine.h"
#include "fly/utility/fly_utility.h"
#include "fly/tuner_core.h"

namespace fly
{

TunerCore::TunerCore(const PlatformIndex platform, const DeviceIndex device, const ComputeAPI computeAPI, const uint32_t queueCount)
{
    if (queueCount == 0)
    {
        throw std::runtime_error("Number of compute queues must be greater than zero");
    }

    if (computeAPI == ComputeAPI::OpenCL)
    {
        #ifdef FLY_PLATFORM_OPENCL
        computeEngine = MakeStdUnique<OpenCLEngine>(platform, device, queueCount);
        #else
        throw std::runtime_error("Support for OpenCL API is not included in this version of Fly framework");
        #endif // FLY_PLATFORM_OPENCL
    }
    else if (computeAPI == ComputeAPI::CUDA)
    {
        #ifdef FLY_PLATFORM_CUDA
        computeEngine = MakeStdUnique<CUDAEngine>(device, queueCount);
        #else
        throw std::runtime_error("Support for CUDA API is not included in this version of Fly framework");
        #endif // FLY_PLATFORM_CUDA
    }
    else if (computeAPI == ComputeAPI::Vulkan)
    {
        #ifdef FLY_PLATFORM_VULKAN
        computeEngine = MakeStdUnique<VulkanEngine>(device, queueCount);
        #else
        throw std::runtime_error("Support for Vulkan API is not included in this version of Fly framework");
        #endif // FLY_PLATFORM_VULKAN
    }
    else
    {
        throw std::runtime_error("Specified compute API is not supported");
    }

    DeviceInfo info = computeEngine->getCurrentDeviceInfo();
    Logger::getLogger().log(LoggingLevel::Info, std::string("Initializing tuner for device ") + info.getName());

    kernelRunner = MakeStdUnique<KernelRunner>(&argumentManager, &kernelManager, computeEngine.get());
    
}

KernelId TunerCore::addKernel(const std::string& source, const std::string& kernelName, const DimensionVector& globalSize,
    const DimensionVector& localSize)
{
    return kernelManager.addKernel(source, kernelName, globalSize, localSize);
}

KernelId TunerCore::addKernelFromFile(const std::string& filePath, const std::string& kernelName, const DimensionVector& globalSize,
    const DimensionVector& localSize)
{
    return kernelManager.addKernelFromFile(filePath, kernelName, globalSize, localSize);
}



void TunerCore::addParameter(const KernelId id, const std::string& parameterName, const std::vector<size_t>& parameterValues)
{
    kernelManager.addParameter(id, parameterName, parameterValues);
}

void TunerCore::addParameter(const KernelId id, const std::string& parameterName, const std::vector<double>& parameterValues)
{
    kernelManager.addParameter(id, parameterName, parameterValues);
}

void TunerCore::addConstraint(const KernelId id, const std::vector<std::string>& parameterNames,
    const std::function<bool(const std::vector<size_t>&)>& constraintFunction)
{
    kernelManager.addConstraint(id, parameterNames, constraintFunction);
}

void TunerCore::addParameterPack(const KernelId id, const std::string& packName, const std::vector<std::string>& parameterNames)
{
    kernelManager.addParameterPack(id, packName, parameterNames);
}

void TunerCore::setThreadModifier(const KernelId id, const ModifierType modifierType, const ModifierDimension modifierDimension,
    const std::vector<std::string>& parameterNames, const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction)
{
    kernelManager.setThreadModifier(id, modifierType, modifierDimension, parameterNames, modifierFunction);
}

void TunerCore::setLocalMemoryModifier(const KernelId id, const ArgumentId argumentId, const std::vector<std::string>& parameterNames,
    const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction)
{
    kernelManager.setLocalMemoryModifier(id, argumentId, parameterNames, modifierFunction);
}

void TunerCore::setCompositionKernelThreadModifier(const KernelId compositionId, const KernelId kernelId, const ModifierType modifierType,
    const ModifierDimension modifierDimension, const std::vector<std::string>& parameterNames,
    const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction)
{
    kernelManager.setCompositionKernelThreadModifier(compositionId, kernelId, modifierType, modifierDimension, parameterNames, modifierFunction);
}

void TunerCore::setCompositionKernelLocalMemoryModifier(const KernelId compositionId, const KernelId kernelId, const ArgumentId argumentId,
    const std::vector<std::string>& parameterNames, const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction)
{
    kernelManager.setCompositionKernelLocalMemoryModifier(compositionId, kernelId, argumentId, parameterNames, modifierFunction);
}

void TunerCore::setKernelArguments(const KernelId id, const std::vector<ArgumentId>& argumentIds)
{
    for (const auto argumentId : argumentIds)
    {
        if (argumentId >= argumentManager.getArgumentCount())
        {
            throw std::runtime_error(std::string("Invalid kernel argument id: ") + std::to_string(id));
        }
    }

    if (!containsUnique(argumentIds))
    {
        throw std::runtime_error("Kernel argument ids assigned to single kernel must be unique");
    }

    kernelManager.setArguments(id, argumentIds);
}

void TunerCore::setCompositionKernelArguments(const KernelId compositionId, const KernelId kernelId, const std::vector<ArgumentId>& argumentIds)
{
    for (const auto id : argumentIds)
    {
        if (id >= argumentManager.getArgumentCount())
        {
            throw std::runtime_error(std::string("Invalid kernel argument id: ") + std::to_string(id));
        }
    }

    if (!containsUnique(argumentIds))
    {
        throw std::runtime_error("Kernel argument ids assigned to single kernel must be unique");
    }

    kernelManager.setCompositionKernelArguments(compositionId, kernelId, argumentIds);
}

std::string TunerCore::getKernelSource(const KernelId id, const std::vector<ParameterPair>& configuration) const
{
    if (!kernelManager.isKernel(id))
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }
    return kernelManager.getKernelSourceWithDefines(id, configuration);
}

void TunerCore::setCompositionKernelProfiling(const KernelId compositionId, const KernelId kernelId, const bool flag)
{
    kernelManager.setCompositionKernelProfiling(compositionId, kernelId, flag);
}

ArgumentId TunerCore::addArgument(void* data, const size_t numberOfElements, const size_t elementSizeInBytes, const ArgumentDataType dataType,
    const ArgumentMemoryLocation memoryLocation, const ArgumentAccessType accessType, const ArgumentUploadType uploadType, const bool copyData)
{
    return argumentManager.addArgument(data, numberOfElements, elementSizeInBytes, dataType, memoryLocation, accessType, uploadType, copyData);
}

ArgumentId TunerCore::addArgument(const void* data, const size_t numberOfElements, const size_t elementSizeInBytes, const ArgumentDataType dataType,
    const ArgumentMemoryLocation memoryLocation, const ArgumentAccessType accessType, const ArgumentUploadType uploadType)
{
    return argumentManager.addArgument(data, numberOfElements, elementSizeInBytes, dataType, memoryLocation, accessType, uploadType);
}

ComputationResult TunerCore::runKernel(const KernelId id, const std::vector<ParameterPair>& configuration,
    const std::vector<OutputDescriptor>& output)
{
    KernelResult result;

    result = kernelRunner->runKernel(id, KernelRunMode::Running, configuration, output);

    kernelRunner->clearBuffers();

    if (result.isValid())
    {
        if (kernelManager.isComposition(id))
        {
            return ComputationResult(result.getKernelName(), result.getConfiguration().getParameterPairs(), result.getComputationDuration(),
                result.getCompositionProfilingData());
        }
        else
        {
            return ComputationResult(result.getKernelName(), result.getConfiguration().getParameterPairs(), result.getComputationDuration(),
                result.getProfilingData());
        }
    }
    else
    {
        return ComputationResult(result.getKernelName(), result.getConfiguration().getParameterPairs(), result.getErrorMessage());
    }
}


void TunerCore::setPrintingTimeUnit(const TimeUnit unit)
{
    kernelRunner->setTimeUnit(unit);
    resultPrinter.setTimeUnit(unit);
}

void TunerCore::setInvalidResultPrinting(const bool flag)
{
    resultPrinter.setInvalidResultPrinting(flag);
}

void TunerCore::printResult(const KernelId id, std::ostream& outputTarget, const PrintFormat format) const
{
    resultPrinter.printResult(id, outputTarget, format);
}

void TunerCore::printResult(const KernelId id, const std::string& filePath, const PrintFormat format) const
{
    std::ofstream outputFile(filePath);

    if (!outputFile.is_open())
    {
        throw std::runtime_error(std::string("Unable to open file: ") + filePath);
    }

    resultPrinter.printResult(id, outputFile, format);
}

void TunerCore::setCompilerOptions(const std::string& options)
{
    computeEngine->setCompilerOptions(options);
}

void TunerCore::setGlobalSizeType(const GlobalSizeType type)
{
    computeEngine->setGlobalSizeType(type);
}

void TunerCore::setAutomaticGlobalSizeCorrection(const bool flag)
{
    computeEngine->setAutomaticGlobalSizeCorrection(flag);
}

void TunerCore::setKernelCacheCapacity(const size_t capacity)
{
    if (capacity == 0)
    {
        computeEngine->setKernelCacheUsage(false);
    }
    else
    {
        computeEngine->setKernelCacheUsage(true);
    }
    computeEngine->setKernelCacheCapacity(capacity);
}

void TunerCore::persistArgument(const ArgumentId id, const bool flag)
{
    argumentManager.setPersistentFlag(id, flag);
    KernelArgument& argument = argumentManager.getArgument(id);
    computeEngine->persistArgument(argument, flag);
}

void TunerCore::downloadPersistentArgument(const OutputDescriptor& output) const
{
    computeEngine->downloadArgument(output.getArgumentId(), output.getOutputDestination(), output.getOutputSizeInBytes());
}

void TunerCore::printComputeAPIInfo(std::ostream& outputTarget) const
{
    computeEngine->printComputeAPIInfo(outputTarget);
}

std::vector<PlatformInfo> TunerCore::getPlatformInfo() const
{
    return computeEngine->getPlatformInfo();
}

std::vector<DeviceInfo> TunerCore::getDeviceInfo(const PlatformIndex platform) const
{
    return computeEngine->getDeviceInfo(platform);
}

DeviceInfo TunerCore::getCurrentDeviceInfo() const
{
    return computeEngine->getCurrentDeviceInfo();
}

void TunerCore::setKernelProfilingCounters(const std::vector<std::string>& counterNames)
{
    computeEngine->setKernelProfilingCounters(counterNames);
}

void TunerCore::setLoggingLevel(const LoggingLevel level)
{
    Logger::getLogger().setLoggingLevel(level);
}

void TunerCore::setLoggingTarget(std::ostream& outputTarget)
{
    Logger::getLogger().setLoggingTarget(outputTarget);
}

void TunerCore::setLoggingTarget(const std::string& filePath)
{
    Logger::getLogger().setLoggingTarget(filePath);
}

void TunerCore::log(const LoggingLevel level, const std::string& message)
{
    Logger::getLogger().log(level, message);
}

} // namespace fly
