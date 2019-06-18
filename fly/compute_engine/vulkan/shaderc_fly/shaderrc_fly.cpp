//
//  shaderrc.cpp
//  flygpgpu_lib
//
//  Created by qiruisun on 2019/6/7.
//  Copyright © 2019 detu. All rights reserved.
//

#ifdef FLY_PLATFORM_VULKAN

#include "shaderrc_fly.hpp"

#ifdef __ANDROID__


// Android specific include files.
#include <unordered_map>

// Header files.
#include "string.h"
#include "errno.h"
#include <android_native_app_glue.h>
#include "shaderc/shaderc.hpp"
// Static variable that keeps ANativeWindow and asset manager instances.
static android_app *Android_application = nullptr;
#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))


#include <MoltenVKGLSLToSPIRVConverter/GLSLToSPIRVConverter.h>

#else

#include "SPIRV/GlslangToSpv.h"

#endif



namespace fly {

    
#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
    
    void init_glslang() {}
    
    void finalize_glslang() {}
    
    bool GLSLtoSPV(const VkShaderStageFlagBits shader_type,std::string pshader, std::vector<unsigned int> &spirv) {
        MVKShaderStage shaderStage;
        switch (shader_type) {
            case VK_SHADER_STAGE_VERTEX_BIT:
                shaderStage = kMVKShaderStageVertex;
                break;
            case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
                shaderStage = kMVKShaderStageTessControl;
                break;
            case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
                shaderStage = kMVKShaderStageTessEval;
                break;
            case VK_SHADER_STAGE_GEOMETRY_BIT:
                shaderStage = kMVKShaderStageGeometry;
                break;
            case VK_SHADER_STAGE_FRAGMENT_BIT:
                shaderStage = kMVKShaderStageFragment;
                break;
            case VK_SHADER_STAGE_COMPUTE_BIT:
                shaderStage = kMVKShaderStageCompute;
                break;
            default:
                shaderStage = kMVKShaderStageAuto;
                break;
        }
        
        
        mvk::GLSLToSPIRVConverter glslConverter;
        glslConverter.setGLSL(pshader);
        bool wasConverted = glslConverter.convert(shaderStage, false, false);
        const std::string logstr =   glslConverter.getResultLog();
        std::cout << " glslConverter.convert  log : " << logstr << std::endl;
        
        if (wasConverted) {
            spirv = glslConverter.getSPIRV();
        }
        return wasConverted;
    }
    
#else  // not IOS OR macOS
    
#ifndef __ANDROID__
    void init_resources(TBuiltInResource &Resources) {
        Resources.maxLights = 32;
        Resources.maxClipPlanes = 6;
        Resources.maxTextureUnits = 32;
        Resources.maxTextureCoords = 32;
        Resources.maxVertexAttribs = 64;
        Resources.maxVertexUniformComponents = 4096;
        Resources.maxVaryingFloats = 64;
        Resources.maxVertexTextureImageUnits = 32;
        Resources.maxCombinedTextureImageUnits = 80;
        Resources.maxTextureImageUnits = 32;
        Resources.maxFragmentUniformComponents = 4096;
        Resources.maxDrawBuffers = 32;
        Resources.maxVertexUniformVectors = 128;
        Resources.maxVaryingVectors = 8;
        Resources.maxFragmentUniformVectors = 16;
        Resources.maxVertexOutputVectors = 16;
        Resources.maxFragmentInputVectors = 15;
        Resources.minProgramTexelOffset = -8;
        Resources.maxProgramTexelOffset = 7;
        Resources.maxClipDistances = 8;
        Resources.maxComputeWorkGroupCountX = 65535;
        Resources.maxComputeWorkGroupCountY = 65535;
        Resources.maxComputeWorkGroupCountZ = 65535;
        Resources.maxComputeWorkGroupSizeX = 1024;
        Resources.maxComputeWorkGroupSizeY = 1024;
        Resources.maxComputeWorkGroupSizeZ = 64;
        Resources.maxComputeUniformComponents = 1024;
        Resources.maxComputeTextureImageUnits = 16;
        Resources.maxComputeImageUniforms = 8;
        Resources.maxComputeAtomicCounters = 8;
        Resources.maxComputeAtomicCounterBuffers = 1;
        Resources.maxVaryingComponents = 60;
        Resources.maxVertexOutputComponents = 64;
        Resources.maxGeometryInputComponents = 64;
        Resources.maxGeometryOutputComponents = 128;
        Resources.maxFragmentInputComponents = 128;
        Resources.maxImageUnits = 8;
        Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
        Resources.maxCombinedShaderOutputResources = 8;
        Resources.maxImageSamples = 0;
        Resources.maxVertexImageUniforms = 0;
        Resources.maxTessControlImageUniforms = 0;
        Resources.maxTessEvaluationImageUniforms = 0;
        Resources.maxGeometryImageUniforms = 0;
        Resources.maxFragmentImageUniforms = 8;
        Resources.maxCombinedImageUniforms = 8;
        Resources.maxGeometryTextureImageUnits = 16;
        Resources.maxGeometryOutputVertices = 256;
        Resources.maxGeometryTotalOutputComponents = 1024;
        Resources.maxGeometryUniformComponents = 1024;
        Resources.maxGeometryVaryingComponents = 64;
        Resources.maxTessControlInputComponents = 128;
        Resources.maxTessControlOutputComponents = 128;
        Resources.maxTessControlTextureImageUnits = 16;
        Resources.maxTessControlUniformComponents = 1024;
        Resources.maxTessControlTotalOutputComponents = 4096;
        Resources.maxTessEvaluationInputComponents = 128;
        Resources.maxTessEvaluationOutputComponents = 128;
        Resources.maxTessEvaluationTextureImageUnits = 16;
        Resources.maxTessEvaluationUniformComponents = 1024;
        Resources.maxTessPatchComponents = 120;
        Resources.maxPatchVertices = 32;
        Resources.maxTessGenLevel = 64;
        Resources.maxViewports = 16;
        Resources.maxVertexAtomicCounters = 0;
        Resources.maxTessControlAtomicCounters = 0;
        Resources.maxTessEvaluationAtomicCounters = 0;
        Resources.maxGeometryAtomicCounters = 0;
        Resources.maxFragmentAtomicCounters = 8;
        Resources.maxCombinedAtomicCounters = 8;
        Resources.maxAtomicCounterBindings = 1;
        Resources.maxVertexAtomicCounterBuffers = 0;
        Resources.maxTessControlAtomicCounterBuffers = 0;
        Resources.maxTessEvaluationAtomicCounterBuffers = 0;
        Resources.maxGeometryAtomicCounterBuffers = 0;
        Resources.maxFragmentAtomicCounterBuffers = 1;
        Resources.maxCombinedAtomicCounterBuffers = 1;
        Resources.maxAtomicCounterBufferSize = 16384;
        Resources.maxTransformFeedbackBuffers = 4;
        Resources.maxTransformFeedbackInterleavedComponents = 64;
        Resources.maxCullDistances = 8;
        Resources.maxCombinedClipAndCullDistances = 8;
        Resources.maxSamples = 4;
        Resources.maxMeshOutputVerticesNV = 256;
        Resources.maxMeshOutputPrimitivesNV = 512;
        Resources.maxMeshWorkGroupSizeX_NV = 32;
        Resources.maxMeshWorkGroupSizeY_NV = 1;
        Resources.maxMeshWorkGroupSizeZ_NV = 1;
        Resources.maxTaskWorkGroupSizeX_NV = 32;
        Resources.maxTaskWorkGroupSizeY_NV = 1;
        Resources.maxTaskWorkGroupSizeZ_NV = 1;
        Resources.maxMeshViewCountNV = 4;
        Resources.limits.nonInductiveForLoops = 1;
        Resources.limits.whileLoops = 1;
        Resources.limits.doWhileLoops = 1;
        Resources.limits.generalUniformIndexing = 1;
        Resources.limits.generalAttributeMatrixVectorIndexing = 1;
        Resources.limits.generalVaryingIndexing = 1;
        Resources.limits.generalSamplerIndexing = 1;
        Resources.limits.generalVariableIndexing = 1;
        Resources.limits.generalConstantMatrixVectorIndexing = 1;
    }
    
    EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type) {
        switch (shader_type) {
            case VK_SHADER_STAGE_VERTEX_BIT:
                return EShLangVertex;
                
            case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
                return EShLangTessControl;
                
            case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
                return EShLangTessEvaluation;
                
            case VK_SHADER_STAGE_GEOMETRY_BIT:
                return EShLangGeometry;
                
            case VK_SHADER_STAGE_FRAGMENT_BIT:
                return EShLangFragment;
                
            case VK_SHADER_STAGE_COMPUTE_BIT:
                return EShLangCompute;
                
            default:
                return EShLangVertex;
        }
    }
#endif
    
    void init_glslang() {
#ifndef __ANDROID__
        glslang::InitializeProcess();
#endif
    }
    
    void finalize_glslang() {
#ifndef __ANDROID__
        glslang::FinalizeProcess();
#endif
    }
    
#ifdef __ANDROID__
    // Android specific helper functions for shaderc.
    struct shader_type_mapping {
        VkShaderStageFlagBits vkshader_type;
        shaderc_shader_kind shaderc_type;
    };
    static const shader_type_mapping shader_map_table[] = {
        {VK_SHADER_STAGE_VERTEX_BIT, shaderc_glsl_vertex_shader},
        {VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, shaderc_glsl_tess_control_shader},
        {VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, shaderc_glsl_tess_evaluation_shader},
        {VK_SHADER_STAGE_GEOMETRY_BIT, shaderc_glsl_geometry_shader},
        {VK_SHADER_STAGE_FRAGMENT_BIT, shaderc_glsl_fragment_shader},
        {VK_SHADER_STAGE_COMPUTE_BIT, shaderc_glsl_compute_shader},
    };
    shaderc_shader_kind MapShadercType(VkShaderStageFlagBits vkShader) {
        for (auto shader : shader_map_table) {
            if (shader.vkshader_type == vkShader) {
                return shader.shaderc_type;
            }
        }
        assert(false);
        return shaderc_glsl_infer_from_source;
    }
#endif
    
    //
    // Compile a given string containing GLSL into SPV for use by VK
    // Return value of false means an error was encountered.
    //
    bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, std::string pshader, std::vector<unsigned int> &spirv) {
#ifndef __ANDROID__
        EShLanguage stage = FindLanguage(shader_type);
        glslang::TShader shader(stage);
        glslang::TProgram program;
        const char *shaderStrings[1];
        TBuiltInResource Resources = {};
        init_resources(Resources);
        
        // Enable SPIR-V and Vulkan rules when parsing GLSL
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
        
        shaderStrings[0] = pshader;
        shader.setStrings(shaderStrings, 1);
        
        if (!shader.parse(&Resources, 100, false, messages)) {
            puts(shader.getInfoLog());
            puts(shader.getInfoDebugLog());
            return false;  // something didn't work
        }
        
        program.addShader(&shader);
        
        //
        // Program-level processing...
        //
        
        if (!program.link(messages)) {
            puts(shader.getInfoLog());
            puts(shader.getInfoDebugLog());
            fflush(stdout);
            return false;
        }
        
        glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
#else
        // On Android, use shaderc instead.
        shaderc::Compiler compiler;
        shaderc::SpvCompilationResult module =
        compiler.CompileGlslToSpv(pshader, strlen(pshader), MapShadercType(shader_type), "shader");
        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            LOGE("Error: Id=%d, Msg=%s", module.GetCompilationStatus(), module.GetErrorMessage().c_str());
            return false;
        }
        spirv.assign(module.cbegin(), module.cend());
#endif
        return true;
    }
    
#endif  // IOS or macOS
    
}

#endif