#include <fly/kernel/kernel_parameter.h>
#include <fly/utility/fly_utility.h>

namespace fly
{

KernelParameter::KernelParameter(const std::string& name, const std::vector<size_t>& values) :
    name(name),
    values(values),
    isDouble(false)
{
    for (const auto value : values)
    {
        valuesDouble.push_back(static_cast<double>(value));
    }
}

KernelParameter::KernelParameter(const std::string& name, const std::vector<double>& values) :
    name(name),
    valuesDouble(values),
    isDouble(true)
{
    for (const auto value : values)
    {
        this->values.push_back(static_cast<size_t>(value));
    }
}

const std::string& KernelParameter::getName() const
{
    return name;
}

const std::vector<size_t>& KernelParameter::getValues() const
{
    return values;
}

const std::vector<double>& KernelParameter::getValuesDouble() const
{
    return valuesDouble;
}

bool KernelParameter::hasValuesDouble() const
{
    return isDouble;
}

bool KernelParameter::operator==(const KernelParameter& other) const
{
    return name == other.name;
}

bool KernelParameter::operator!=(const KernelParameter& other) const
{
    return !(*this == other);
}

} // namespace fly
