#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fly/tuning_runner/kernel_runner.h>
#include <fly/utility/fly_utility.h>
#include <fly/utility/logger.h>
#include <fly/utility/timer.h>

namespace fly
{

KernelRunner::KernelRunner(ArgumentManager* argumentManager, KernelManager* kernelManager, ComputeEngine* computeEngine) :
    argumentManager(argumentManager),
    kernelManager(kernelManager),
    computeEngine(computeEngine),
    timeUnit(TimeUnit::Milliseconds)
{}

KernelResult KernelRunner::runKernel(const KernelId id, const KernelRunMode mode, const KernelConfiguration& configuration,
    const std::vector<OutputDescriptor>& output)
{
    if (!kernelManager->isKernel(id))
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }

    const Kernel& kernel = kernelManager->getKernel(id);


    std::stringstream stream;
    stream << "Running kernel " << kernel.getName() << " with configuration: " << configuration;
    Logger::getLogger().log(LoggingLevel::Info, stream.str());

    KernelResult result;
    try
    {
        result = runKernelSimple(kernel, mode, configuration, output);
    }
    catch (const std::runtime_error& error)
    {
        computeEngine->synchronizeDevice();
        computeEngine->clearEvents();
        Logger::getLogger().log(LoggingLevel::Warning, std::string("Kernel run failed, reason: ") + error.what());
        result = KernelResult(kernel.getName(), configuration, error.what());
    }

    return result;
}

KernelResult KernelRunner::runKernel(const KernelId id, const KernelRunMode mode, const std::vector<ParameterPair>& configuration,
    const std::vector<OutputDescriptor>& output)
{
    const KernelConfiguration launchConfiguration = kernelManager->getKernelConfiguration(id, configuration);
    return runKernel(id, mode, launchConfiguration, output);
}


void KernelRunner::setTimeUnit(const TimeUnit unit)
{
    this->timeUnit = unit;
}



KernelArgument KernelRunner::downloadArgument(const ArgumentId id) const
{
    return computeEngine->downloadArgumentObject(id, nullptr);
}

void KernelRunner::clearBuffers(const ArgumentAccessType accessType)
{
    computeEngine->clearBuffers(accessType);
}

void KernelRunner::clearBuffers()
{
    computeEngine->clearBuffers();
}

void KernelRunner::setPersistentArgumentUsage(const bool flag)
{
    computeEngine->setPersistentBufferUsage(flag);
}

KernelResult KernelRunner::runKernelSimple(const Kernel& kernel, const KernelRunMode mode, const KernelConfiguration& configuration,
    const std::vector<OutputDescriptor>& output)
{
    KernelId kernelId = kernel.getId();
    const std::string& kernelName = kernel.getName();
    std::string source = kernelManager->getKernelSourceWithDefines(kernelId, configuration);
    
    KernelRuntimeData kernelData(kernelId, kernelName, source, kernel.getSource(), configuration.getGlobalSize(), configuration.getLocalSize(),
        configuration.getParameterPairs(), kernel.getArgumentIds(), configuration.getLocalMemoryModifiers());

    KernelResult result;
    result = computeEngine->runKernel(kernelData, argumentManager->getArguments(kernel.getArgumentIds()), output);

    result.setConfiguration(configuration);
    return result;
}





} // namespace fly
