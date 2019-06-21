#include <iostream>



#include "tuner_api.h"
#include "tuner_core.h"




namespace fly
{


Tuner::Tuner(const PlatformIndex platform, const DeviceIndex device, const ComputeAPI computeAPI) :
    tunerCore(MakeStdUnique<TunerCore>(platform, device, computeAPI, 1))
{}

Tuner::Tuner(const PlatformIndex platform, const DeviceIndex device, const ComputeAPI computeAPI, const uint32_t computeQueueCount) :
    tunerCore(MakeStdUnique<TunerCore>(platform, device, computeAPI, computeQueueCount))
{}

Tuner::~Tuner() = default;

KernelId Tuner::addKernel(const std::string& source, const std::string& kernelName, const DimensionVector& globalSize,
    const DimensionVector& localSize)
{
    return tunerCore->addKernel(source, kernelName, globalSize, localSize);
}

KernelId Tuner::addKernelFromFile(const std::string& filePath, const std::string& kernelName, const DimensionVector& globalSize,
    const DimensionVector& localSize)
{
    try
    {
        return tunerCore->addKernelFromFile(filePath, kernelName, globalSize, localSize);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::setKernelArguments(const KernelId id, const std::vector<ArgumentId>& argumentIds)
{
    try
    {
        tunerCore->setKernelArguments(id, argumentIds);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::addParameter(const KernelId id, const std::string& parameterName, const std::vector<size_t>& parameterValues)
{
    try
    {
        tunerCore->addParameter(id, parameterName, parameterValues);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::addParameterDouble(const KernelId id, const std::string& parameterName, const std::vector<double>& parameterValues)
{
    try
    {
        tunerCore->addParameter(id, parameterName, parameterValues);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::addParameterPack(const KernelId id, const std::string& packName, const std::vector<std::string>& parameterNames)
{
    try
    {
        tunerCore->addParameterPack(id, packName, parameterNames);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::setThreadModifier(const KernelId id, const ModifierType modifierType, const ModifierDimension modifierDimension,
    const std::vector<std::string>& parameterNames, const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction)
{
    try
    {
        tunerCore->setThreadModifier(id, modifierType, modifierDimension, parameterNames, modifierFunction);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::setThreadModifier(const KernelId id, const ModifierType modifierType, const ModifierDimension modifierDimension,
    const std::string& parameterName, const ModifierAction modifierAction)
{
    switch (modifierAction)
    {
    case ModifierAction::Add:
        setThreadModifier(id, modifierType, modifierDimension, std::vector<std::string>{parameterName},
            [](const size_t threadSize, const std::vector<size_t>& parameters) {return threadSize + parameters.at(0);});
        break;
    case ModifierAction::Subtract:
        setThreadModifier(id, modifierType, modifierDimension, std::vector<std::string>{parameterName},
            [](const size_t threadSize, const std::vector<size_t>& parameters) {return threadSize - parameters.at(0);});
        break;
    case ModifierAction::Multiply:
        setThreadModifier(id, modifierType, modifierDimension, std::vector<std::string>{parameterName},
            [](const size_t threadSize, const std::vector<size_t>& parameters) {return threadSize * parameters.at(0);});
        break;
    case ModifierAction::Divide:
        setThreadModifier(id, modifierType, modifierDimension, std::vector<std::string>{parameterName},
            [](const size_t threadSize, const std::vector<size_t>& parameters) {return threadSize / parameters.at(0);});
        break;
    default:
        throw std::runtime_error("Unknown modifier action");
    }
}

void Tuner::setLocalMemoryModifier(const KernelId id, const ArgumentId argumentId, const std::vector<std::string>& parameterNames,
    const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction)
{
    try
    {
        tunerCore->setLocalMemoryModifier(id, argumentId, parameterNames, modifierFunction);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::addConstraint(const KernelId id, const std::vector<std::string>& parameterNames,
    const std::function<bool(const std::vector<size_t>&)>& constraintFunction)
{
    try
    {
        tunerCore->addConstraint(id, parameterNames, constraintFunction);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}


void Tuner::persistArgument(const ArgumentId id, const bool flag)
{
    try
    {
        tunerCore->persistArgument(id, flag);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::downloadPersistentArgument(const OutputDescriptor& output) const
{
    try
    {
        tunerCore->downloadPersistentArgument(output);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}


ComputationResult Tuner::runKernel(const KernelId id, const std::vector<ParameterPair>& configuration, const std::vector<OutputDescriptor>& output)
{
    try
    {
        return tunerCore->runKernel(id, configuration, output);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}





std::string Tuner::getKernelSource(const KernelId id, const std::vector<ParameterPair>& configuration) const
{
    try
    {
        return tunerCore->getKernelSource(id, configuration);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::setCompilerOptions(const std::string& options)
{
    tunerCore->setCompilerOptions(options);
}

void Tuner::setKernelCacheCapacity(const size_t capacity)
{
    tunerCore->setKernelCacheCapacity(capacity);
}

void Tuner::printComputeAPIInfo(std::ostream& outputTarget) const
{
    try
    {
        tunerCore->printComputeAPIInfo(outputTarget);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
    }
}

std::vector<PlatformInfo> Tuner::getPlatformInfo() const
{
    try
    {
        return tunerCore->getPlatformInfo();
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

std::vector<DeviceInfo> Tuner::getDeviceInfo(const PlatformIndex platform) const
{
    try
    {
        return tunerCore->getDeviceInfo(platform);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

DeviceInfo Tuner::getCurrentDeviceInfo() const
{
    try
    {
        return tunerCore->getCurrentDeviceInfo();
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

void Tuner::setAutomaticGlobalSizeCorrection(const bool flag)
{
    tunerCore->setAutomaticGlobalSizeCorrection(flag);
}

void Tuner::setGlobalSizeType(const GlobalSizeType type)
{
    tunerCore->setGlobalSizeType(type);
}

void Tuner::setLoggingLevel(const LoggingLevel level)
{
    TunerCore::setLoggingLevel(level);
}

void Tuner::setLoggingTarget(std::ostream& outputTarget)
{
    TunerCore::setLoggingTarget(outputTarget);
}

void Tuner::setLoggingTarget(const std::string& filePath)
{
    TunerCore::setLoggingTarget(filePath);
}

ArgumentId Tuner::addArgument(void* vectorData, const size_t numberOfElements, const size_t elementSizeInBytes, const ArgumentDataType dataType,
    const ArgumentMemoryLocation memoryLocation, const ArgumentAccessType accessType, const bool copyData)
{
    try
    {
        return tunerCore->addArgument(vectorData, numberOfElements, elementSizeInBytes, dataType, memoryLocation, accessType,
            ArgumentUploadType::Vector, copyData);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

ArgumentId Tuner::addArgument(const void* data, const size_t numberOfElements, const size_t elementSizeInBytes, const ArgumentDataType dataType,
    const ArgumentMemoryLocation memoryLocation, const ArgumentAccessType accessType, const ArgumentUploadType uploadType)
{
    try
    {
        return tunerCore->addArgument(data, numberOfElements, elementSizeInBytes, dataType, memoryLocation, accessType, uploadType);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

ArgumentId Tuner::addArgument(const size_t localMemoryElementsCount, const size_t elementSizeInBytes, const ArgumentDataType dataType)
{
    try
    {
        return tunerCore->addArgument(nullptr, localMemoryElementsCount, elementSizeInBytes, dataType, ArgumentMemoryLocation::Device,
            ArgumentAccessType::ReadOnly, ArgumentUploadType::Local);
    }
    catch (const std::runtime_error& error)
    {
        TunerCore::log(LoggingLevel::Error, error.what());
        throw;
    }
}

} // namespace fly
