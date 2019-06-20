#pragma once

#include <map>
#include <memory>
#include <utility>
#include <fly/compute_engine/compute_engine.h>
#include <fly/dto/kernel_result.h>
#include <fly/enum/kernel_run_mode.h>
#include <fly/enum/time_unit.h>
#include <fly/kernel/kernel_manager.h>
#include <fly/kernel_argument/argument_manager.h>


namespace fly
{

class KernelRunner
{
public:
    // Constructor
    explicit KernelRunner(ArgumentManager* argumentManager, KernelManager* kernelManager, ComputeEngine* computeEngine);

    // Core methods
    KernelResult runKernel(const KernelId id, const KernelRunMode mode, const KernelConfiguration& configuration,
        const std::vector<OutputDescriptor>& output);

    KernelResult runKernel(const KernelId id, const KernelRunMode mode, const std::vector<ParameterPair>& configuration,
        const std::vector<OutputDescriptor>& output);

    void setTimeUnit(const TimeUnit unit);
    void setKernelProfiling(const bool flag);
    bool getKernelProfiling();

  

    // Compute engine methods
    KernelArgument downloadArgument(const ArgumentId id) const;
    void clearBuffers(const ArgumentAccessType accessType);
    void clearBuffers();
    void setPersistentArgumentUsage(const bool flag);

private:
    // Attributes
    ArgumentManager* argumentManager;
    KernelManager* kernelManager;
    ComputeEngine* computeEngine;
  

    TimeUnit timeUnit;
    bool kernelProfilingFlag;

    // Helper methods
    KernelResult runKernelSimple(const Kernel& kernel, const KernelRunMode mode, const KernelConfiguration& configuration,
        const std::vector<OutputDescriptor>& output);
    KernelResult runSimpleKernelProfiling(const Kernel& kernel, const KernelRunMode mode, const KernelRuntimeData& kernelData,
        const std::vector<OutputDescriptor>& output);
    
};

} // namespace fly
