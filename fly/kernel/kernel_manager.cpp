#include <fstream>
#include <sstream>
#include <fly/kernel/kernel_manager.h>
#include <fly/utility/fly_utility.h>

namespace fly
{

KernelManager::KernelManager() :
    nextId(0)
{}

KernelId KernelManager::addKernel(const std::string& source, const std::string& kernelName, const DimensionVector& globalSize,
    const DimensionVector& localSize)
{
    kernels.emplace_back(nextId, source, kernelName, globalSize, localSize);
    return nextId++;
}

KernelId KernelManager::addKernelFromFile(const std::string& filePath, const std::string& kernelName, const DimensionVector& globalSize,
    const DimensionVector& localSize)
{
    std::string source = loadFileToString(filePath);
    return addKernel(source, kernelName, globalSize, localSize);
}


std::string KernelManager::getKernelSourceWithDefines(const KernelId id, const KernelConfiguration& configuration) const
{
    return getKernelSourceWithDefines(id, configuration.getParameterPairs());
}

std::string KernelManager::getKernelSourceWithDefines(const KernelId id, const std::vector<ParameterPair>& configuration) const
{
    std::string source = getKernel(id).getSource();

    for (const auto& parameterPair : configuration)
    {
        std::stringstream stream;
        if (!parameterPair.hasValueDouble())
        {
            stream << parameterPair.getValue();
        }
        else
        {
            stream << parameterPair.getValueDouble();
        }
        source = std::string("#define ") + parameterPair.getName() + std::string(" ") + stream.str() + std::string("\n") + source;
    }

    return source;
}

KernelConfiguration KernelManager::getKernelConfiguration(const KernelId id, const std::vector<ParameterPair>& parameterPairs) const
{
    if (!isKernel(id))
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }

    const Kernel& kernel = getKernel(id);
    DimensionVector global = kernel.getModifiedGlobalSize(parameterPairs);
    DimensionVector local = kernel.getModifiedLocalSize(parameterPairs);
    std::vector<LocalMemoryModifier> modifiers = kernel.getLocalMemoryModifiers(parameterPairs);

    return KernelConfiguration(global, local, parameterPairs, modifiers);
}


void KernelManager::addParameter(const KernelId id, const std::string& name, const std::vector<size_t>& values)
{
    if (values.empty())
    {
        throw std::runtime_error("Tuning parameter must have at least one valid value");
    }

    if (isKernel(id))
    {
        getKernel(id).addParameter(KernelParameter(name, values));
    }
    else
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }
}

void KernelManager::addParameter(const KernelId id, const std::string& name, const std::vector<double>& values)
{
    if (values.empty())
    {
        throw std::runtime_error("Tuning parameter must have at least one valid value");
    }

    if (isKernel(id))
    {
        getKernel(id).addParameter(KernelParameter(name, values));
    }
    else
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }
}

void KernelManager::addConstraint(const KernelId id, const std::vector<std::string>& parameterNames,
    const std::function<bool(const std::vector<size_t>&)>& constraintFunction)
{
    if (isKernel(id))
    {
        getKernel(id).addConstraint(KernelConstraint(parameterNames, constraintFunction));
    }
    else
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }
}

void KernelManager::addParameterPack(const KernelId id, const std::string& packName, const std::vector<std::string>& parameterNames)
{
    if (isKernel(id))
    {
        getKernel(id).addParameterPack(KernelParameterPack(packName, parameterNames));
    }
    else
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }
}

void KernelManager::setThreadModifier(const KernelId id, const ModifierType modifierType, const ModifierDimension modifierDimension,
    const std::vector<std::string>& parameterNames, const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction)
{
    if (isKernel(id))
    {
        getKernel(id).setThreadModifier(modifierType, modifierDimension, parameterNames, modifierFunction);
    }
    else
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }
}

void KernelManager::setLocalMemoryModifier(const KernelId id, const ArgumentId argumentId, const std::vector<std::string>& parameterNames,
    const std::function<size_t(const size_t, const std::vector<size_t>&)>& modifierFunction)
{
    if (isKernel(id))
    {
        getKernel(id).setLocalMemoryModifier(argumentId, parameterNames, modifierFunction);
    }
    else
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }
}

void KernelManager::setArguments(const KernelId id, const std::vector<ArgumentId>& argumentIds)
{
    if (isKernel(id))
    {
        getKernel(id).setArguments(argumentIds);
    }
    else
    {
        throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
    }
}


const Kernel& KernelManager::getKernel(const KernelId id) const
{
    for (const auto& kernel : kernels)
    {
        if (kernel.getId() == id)
        {
            return kernel;
        }
    }

    throw std::runtime_error(std::string("Invalid kernel id: ") + std::to_string(id));
}

Kernel& KernelManager::getKernel(const KernelId id)
{
    return const_cast<Kernel&>(static_cast<const KernelManager*>(this)->getKernel(id));
}


bool KernelManager::isKernel(const KernelId id) const
{
    for (const auto& kernel : kernels)
    {
        if (kernel.getId() == id)
        {
            return true;
        }
    }

    return false;
}

std::string KernelManager::loadFileToString(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(std::string("Unable to open file: ") + filePath);
    }

    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

} // namespace fly
