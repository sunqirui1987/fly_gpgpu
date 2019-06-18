#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <fly/api/parameter_pair.h>
#include "shaderc_fly/shaderrc_fly.hpp"

namespace fly
{


class ShadercCompiler
{
public:
    static ShadercCompiler& getCompiler()
    {
        static ShadercCompiler instance;
        return instance;
    }

    std::vector<uint32_t> compile(const std::string& name, const std::string& source, const VkShaderStageFlagBits kind,
        const std::vector<size_t>& localSize, const std::vector<ParameterPair>& parameterPairs)
    {
        std::string marco_str = "";
        marco_str =  marco_str + "#define LOCAL_SIZE_X "+ std::to_string(localSize[0]) +"\n";
        marco_str =  marco_str + "#define LOCAL_SIZE_Y "+ std::to_string(localSize[1]) +"\n";
        marco_str =  marco_str + "#define LOCAL_SIZE_Z "+ std::to_string(localSize[2]) +"\n";
        
        
        std::string preprocessedSource = replaceAll(source, "//{DEFINE_LOCAL_SIZE}", marco_str);
        
        
        std::vector<uint32_t> compiledSource = compileShader(name, preprocessedSource, kind);
        return compiledSource;
    }

    ShadercCompiler(const ShadercCompiler&) = delete;
    ShadercCompiler(ShadercCompiler&&) = delete;
    void operator=(const ShadercCompiler&) = delete;
    void operator=(ShadercCompiler&&) = delete;

private:
   
    ShadercCompiler()
    {
       
    }



    std::vector<uint32_t> compileShader(const std::string& name, const std::string& source, const VkShaderStageFlagBits kind)
    {
        
        // Convert GLSL to SPIR-V
        init_glslang();
        std::vector<unsigned int> spv;

        bool  retVal = GLSLtoSPV(kind , source, spv);
        assert(retVal);
        finalize_glslang();
        
        return spv;

    }

};

} // namespace fly
