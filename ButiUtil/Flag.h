#ifndef BUTI_FLAG_H
#define BUTI_FLAG_H
#include<cstdint>
namespace ButiEngine {

template<typename EnumType, typename BitsType = std::uint32_t>
class Flags
{
public:

    Flags() : bits(0)
    {
    }

    Flags(EnumType arg_bit)
        : bits(static_cast<BitsType>(arg_bit))
    {
    }

    Flags(const Flags<EnumType>& arg_other)
        : bits(arg_other.bits)
    {
    }

    explicit Flags(const BitsType arg_flags) : bits(arg_flags)
    {
    }


    bool hasFlag(const EnumType arg_value) const
    {
        BitsType bitValue = static_cast<BitsType>(arg_value);
        return (bits & bitValue) == bitValue && (bitValue != 0 || bits == bitValue);
    }

    Flags<EnumType>& set(const EnumType arg_value)
    {
        bits |= static_cast<BitsType>(arg_value);
        return *this;
    }

    Flags<EnumType>& set(const EnumType arg_value,const bool arg_bit)
    {
        if (arg_bit) {
            set(arg_value);
        }
        else {
            unset(arg_value);
        }
        return *this;
    }

    void unset(const EnumType arg_value)
    {
        bits &= ~static_cast<BitsType>(arg_value);
    }

    EnumType get() const
    {
        return static_cast<EnumType>(bits);
    }

    Flags<EnumType>& operator=(const Flags<EnumType>& arg_other)
    {
        bits = arg_other.bits;
        return *this;
    }

    Flags<EnumType>& operator|=(const Flags<EnumType>& arg_other)
    {
        bits |= arg_other.bits;
        return *this;
    }

    Flags<EnumType>& operator&=(const Flags<EnumType>& arg_other)
    {
        bits &= arg_other.bits;
        return *this;
    }

    Flags<EnumType>& operator^=(const Flags<EnumType>& arg_other)
    {
        bits ^= arg_other.bits;
        return *this;
    }

    Flags<EnumType> operator~() const
    {
        Flags<EnumType> result(*this);
        result.bits = static_cast<BitsType>(~bits);
        return result;
    }

    explicit operator BitsType() const
    {
        return bits;
    }

    operator EnumType() const
    {
        return static_cast<EnumType>(bits);
    }

    bool operator!() const
    {
        return !bits;
    }

    explicit operator bool() const
    {
        return !!bits;
    }
    template<class Archive>
    void serialize(Archive& archive) {
        archive(bits);
    }

public:
    BitsType bits;
};

template<typename T>
static bool CheckFlag(const T arg_lhs, const T arg_rhs)
{
    return Flags<T>(arg_lhs).hasFlag(arg_rhs);
}

}

#endif // !BUTI_FLAG_H
