#ifndef DATAUTILS_H
#define DATAUTILS_H

#include <string>

namespace DataUtils
{
    std::string trim(const std::string &str);
    bool isValidInputChar(char inputChar);

    std::string currentTimestamp();
    std::string displayNameFromRoomPath(const std::string &path);
};

#endif // DATAUTILS_H
