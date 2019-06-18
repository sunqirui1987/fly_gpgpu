#pragma once

#ifdef FLY_PLATFORM_VULKAN

#include <string>
#include <vulkan/vulkan.h>

namespace fly
{

    std::string replaceAll(std::string str, const std::string& from, const std::string& to);
    std::string getVulkanEnumName(const VkResult value);
    void checkVulkanError(const VkResult value);
    void checkVulkanError(const VkResult value, const std::string& message);

} // namespace fly

#endif // FLY_PLATFORM_VULKAN
