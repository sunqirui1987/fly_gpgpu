#ifdef FLY_PLATFORM_VULKAN

#include <stdexcept>
#include <fly/compute_engine/vulkan/vulkan_utility.h>

namespace fly
{

std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
    

std::string getVulkanEnumName(const VkResult value)
{
    switch (value)
    {
    case VK_SUCCESS:
        return std::string("VK_SUCCESS");
    case VK_NOT_READY:
        return std::string("VK_NOT_READY");
    case VK_TIMEOUT:
        return std::string("VK_TIMEOUT");
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return std::string("VK_ERROR_OUT_OF_HOST_MEMORY");
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return std::string("VK_ERROR_OUT_OF_DEVICE_MEMORY");
    case VK_ERROR_INITIALIZATION_FAILED:
        return std::string("VK_ERROR_INITIALIZATION_FAILED");
    case VK_ERROR_DEVICE_LOST:
        return std::string("VK_ERROR_DEVICE_LOST");
    case VK_ERROR_LAYER_NOT_PRESENT:
        return std::string("VK_ERROR_LAYER_NOT_PRESENT");
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return std::string("VK_ERROR_EXTENSION_NOT_PRESENT");
    case VK_ERROR_TOO_MANY_OBJECTS:
        return std::string("VK_ERROR_TOO_MANY_OBJECTS");
    default:
        return std::to_string(static_cast<int>(value));
    }
}

void checkVulkanError(const VkResult value)
{
    if (value != VK_SUCCESS)
    {
        throw std::runtime_error(std::string("Internal Vulkan error: ") + getVulkanEnumName(value));
    }
}

void checkVulkanError(const VkResult value, const std::string& message)
{
    if (value != VK_SUCCESS)
    {
        throw std::runtime_error(std::string("Internal Vulkan error: ") + getVulkanEnumName(value) + "\nAdditional info: " + message);
    }
}

} // namespace fly

#endif // FLY_PLATFORM_VULKAN
