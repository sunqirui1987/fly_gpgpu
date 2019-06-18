#pragma once

#include <string>
#include <fly/compute_engine/opencl/opencl_common.h>

namespace fly
{

class OpenCLDevice
{
public:
    explicit OpenCLDevice(const cl_device_id id, const std::string& name) :
        id(id),
        name(name)
    {}

    cl_device_id getId() const
    {
        return id;
    }

    const std::string& getName() const
    {
        return name;
    }

private:
    cl_device_id id;
    std::string name;
};

} // namespace fly
