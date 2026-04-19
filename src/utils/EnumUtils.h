#ifndef ENUMUTILS_H
#define ENUMUTILS_H

#include <functional>

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
};

#endif // ENUMUTILS_H
