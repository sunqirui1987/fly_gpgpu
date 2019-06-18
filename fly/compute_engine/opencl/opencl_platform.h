#pragma once

#include <string>
#include <fly/compute_engine/opencl/opencl_common.h>
namespace fly
{

class OpenCLPlatform
{
public:
    explicit OpenCLPlatform(const cl_platform_id id, const std::string& name) :
        id(id),
        name(name)
    {}

    cl_platform_id getId() const
    {
        return id;
    }

    const std::string& getName() const
    {
        return name;
    }

private:
    cl_platform_id id;
    std::string name;
};

} // namespace fly
