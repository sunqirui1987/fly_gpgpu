/** @file enum_bitfield.h
  * Support for bitwise operations for strongly typed enums.
  */
#pragma once

#include <type_traits>

namespace fly
{

/** @struct EnableBitfieldOperators
  * Structure which enables bitwise operations support for specified enum.
  */
template <typename EnumType>
struct EnableBitfieldOperators
{
    /** Bitwise operations are disabled by default.
      */
    static const bool enable = false;
};

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, EnumType>::type
operator&(EnumType lhs, EnumType rhs)
{
    using Underlying = typename std::underlying_type<EnumType>::type;
    return static_cast<EnumType>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, EnumType>::type
operator|(EnumType lhs, EnumType rhs)
{
    using Underlying = typename std::underlying_type<EnumType>::type;
    return static_cast<EnumType>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, EnumType>::type
operator^(EnumType lhs, EnumType rhs)
{
    using Underlying = typename std::underlying_type<EnumType>::type;
    return static_cast<EnumType>(static_cast<Underlying>(lhs) ^ static_cast<Underlying>(rhs));
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, EnumType>::type
operator~(EnumType lhs)
{
    using Underlying = typename std::underlying_type<EnumType>::type;
    return static_cast<EnumType>(~static_cast<Underlying>(lhs));
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, EnumType&>::type
operator&=(EnumType& lhs, EnumType rhs)
{
    using Underlying = typename std::underlying_type<EnumType>::type;
    lhs = static_cast<EnumType>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
    return lhs;
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, EnumType &>::type
operator|=(EnumType& lhs, EnumType rhs)
{
    using Underlying = typename std::underlying_type<EnumType>::type;
    lhs = static_cast<EnumType>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
    return lhs;
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, EnumType &>::type
operator^=(EnumType& lhs, EnumType rhs)
{
    using Underlying = typename std::underlying_type<EnumType>::type;
    lhs = static_cast<EnumType>(static_cast<Underlying>(lhs) ^ static_cast<Underlying>(rhs));
    return lhs;
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, void>::type
setFlag(EnumType& bitfield, EnumType flag)
{
    bitfield |= flag;
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, void>::type
clearFlag(EnumType& bitfield, EnumType flag)
{
    bitfield &= ~flag;
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, void>::type
flipFlag(EnumType& bitfield, EnumType flag)
{
    bitfield ^= flag;
}

template <typename EnumType>
typename std::enable_if<EnableBitfieldOperators<EnumType>::enable, bool>::type
hasFlag(EnumType bitfield, EnumType flag)
{
    return static_cast<bool>(bitfield & flag);
}

} // namespace fly
