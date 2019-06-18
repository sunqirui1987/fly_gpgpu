#pragma once

#include <memory>
#include <fly/compute_engine/vulkan/vulkan_compute_pipeline.h>
#include <fly/compute_engine/vulkan/vulkan_descriptor_set_layout.h>
#include <fly/compute_engine/vulkan/vulkan_shader_module.h>

namespace fly
{

struct VulkanPipelineCacheEntry
{
public:
    VulkanPipelineCacheEntry(std::unique_ptr<VulkanComputePipeline> pipeline, std::unique_ptr<VulkanDescriptorSetLayout> layout,
        std::unique_ptr<VulkanShaderModule> shader) :
        pipeline(std::move(pipeline)),
        layout(std::move(layout)),
        shader(std::move(shader))
    {}

    std::unique_ptr<VulkanComputePipeline> pipeline;
    std::unique_ptr<VulkanDescriptorSetLayout> layout;
    std::unique_ptr<VulkanShaderModule> shader;
};

} // namespace fly
