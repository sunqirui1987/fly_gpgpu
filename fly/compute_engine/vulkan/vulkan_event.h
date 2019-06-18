#pragma once

#include <memory>
#include <fly/compute_engine/vulkan/vulkan_fence.h>
#include <fly/compute_engine/vulkan/vulkan_utility.h>
#include "fly/fly_types.h"

namespace fly
{

class VulkanEvent
{
public:
    explicit VulkanEvent(VkDevice device, const EventId id, const bool validFlag) :
        id(id),
        kernelName(""),
        fence(nullptr),
        validFlag(validFlag),
        overhead(0)
    {
        if (validFlag)
        {
            fence = MakeStdUnique<VulkanFence>(device);
        }
    }

    explicit VulkanEvent(VkDevice device, const EventId id, const std::string& kernelName, const uint64_t kernelLaunchOverhead) :
        id(id),
        kernelName(kernelName),
        fence(MakeStdUnique<VulkanFence>(device)),
        validFlag(true),
        overhead(kernelLaunchOverhead)
    {}

    EventId getId() const
    {
        return id;
    }

    const std::string& getKernelName() const
    {
        return kernelName;
    }

    const VulkanFence& getFence() const
    {
        if (!isValid())
        {
            throw std::runtime_error("Valid Vulkan fence cannot be accessed by invalid Vulkan events");
        }

        return *fence.get();
    }

    VulkanFence& getFence()
    {
        if (!isValid())
        {
            throw std::runtime_error("Valid Vulkan fence cannot be accessed by invalid Vulkan events");
        }

        return *fence.get();
    }

    bool isValid() const
    {
        return validFlag;
    }

    uint64_t getOverhead() const
    {
        return overhead;
    }

    void wait()
    {
        if (!isValid())
        {
            throw std::runtime_error("Valid Vulkan fence cannot be accessed by invalid Vulkan events");
        }

        fence->wait();
    }

private:
    EventId id;
    std::string kernelName;
    std::unique_ptr<VulkanFence> fence;
    bool validFlag;
    uint64_t overhead;
};

} // namespace fly
