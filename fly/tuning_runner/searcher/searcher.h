#pragma once

#include <fly/api/kernel_profiling_data.h>
#include <fly/kernel/kernel_configuration.h>

namespace fly
{

class Searcher
{
public:
    virtual ~Searcher() = default;
    virtual void calculateNextConfiguration(const bool successFlag, const KernelConfiguration& previousConfiguration,
        const double previousDuration, const KernelProfilingData& previousProfilingData,
        const std::map<KernelId, KernelProfilingData>& previousCompositionProfilingData) = 0;
    virtual KernelConfiguration getNextConfiguration() const = 0;
    virtual size_t getUnexploredConfigurationCount() const = 0;
};

} // namespace fly
