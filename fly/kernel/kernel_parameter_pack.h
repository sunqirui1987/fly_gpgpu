#pragma once

#include <string>
#include <utility>
#include <vector>
#include "fly/fly_types.h"

namespace fly
{

class KernelParameterPack
{
public:
    KernelParameterPack();
    explicit KernelParameterPack(const std::string& name, const std::vector<std::string>& parameterNames);

    const std::string& getName() const;
    const std::vector<std::string>& getParameterNames() const;
    size_t getParameterCount() const;
    bool containsParameter(const std::string& parameterName) const;

    bool operator==(const KernelParameterPack& other) const;
    bool operator!=(const KernelParameterPack& other) const;

private:
    std::string name;
    std::vector<std::string> parameterNames;
};

} // namespace fly
