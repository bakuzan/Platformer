#ifndef ENUMUTILS_H
#define ENUMUTILS_H

#include <array>
#include <cstddef>
#include <functional>

template <typename T>
struct EnumStrings;

namespace EnumUtils
{
    template <typename T>
    void forEachEnum(std::function<void(T)> action)
    {
        for (int i = 0; i < static_cast<int>(T::COUNT); ++i)
        {
            action(static_cast<T>(i));
        }
    }

    template <typename T>
    constexpr const char *enumToString(T value)
    {
        return EnumStrings<T>::names.at(static_cast<size_t>(value));
    }

};

#endif // ENUMUTILS_H
