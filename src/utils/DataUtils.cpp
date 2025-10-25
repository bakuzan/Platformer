#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <filesystem>
#include <regex>

#include "DataUtils.h"

namespace DataUtils
{
    std::string trim(const std::string &str)
    {
        size_t start = str.find_first_not_of(" \t");
        size_t end = str.find_last_not_of(" \t");

        if (start == std::string::npos)
        {
            return ""; // String is all whitespace
        }

        return str.substr(start, end - start + 1);
    }

    bool isValidInputChar(char inputChar)
    {
        unsigned char uchar = static_cast<unsigned char>(inputChar);
        return std::isalnum(uchar) ||
               std::isspace(uchar);
    }

    std::string currentTimestamp()
    {
        using namespace std::chrono;

        // get current time
        auto now = system_clock::now();
        std::time_t t = system_clock::to_time_t(now);

        // convert to localtime
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t); // Windows secure version
#else
        localtime_r(&t, &tm); // POSIX thread-safe version
#endif

        // format
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
        return oss.str();
    }

    std::string displayNameFromRoomPath(const std::string &path)
    {
        // strip directory and extension
        std::filesystem::path p(path);
        std::string stem = p.stem().string();

        // replace "room_" with "Room "
        std::regex re("^room_(\\d+)$", std::regex::icase);
        std::smatch match;

        if (std::regex_match(stem, match, re) &&
            match.size() > 1)
        {
            return "Room " + match[1].str();
        }

        // fallback: just return stem
        return stem;
    }

};
