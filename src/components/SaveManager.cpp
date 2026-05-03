#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>

#include "SaveManager.h"

SaveManager::SaveManager(const std::string &dir)
    : baseDir(dir)
{
    // Constructor
}

SaveManager::~SaveManager()
{
    // Destructor
}

// Publics

SaveMeta SaveManager::loadMeta(int slot)
{
    std::ifstream file(slotFilename(slot));

    if (!file.is_open())
    {
        return SaveMeta::makeDefault();
    }

    SaveMeta meta;
    std::unordered_map<std::string, std::string> kv;
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') &&
            std::getline(iss, value))
        {
            kv[key] = value;
        }
    }

    if (kv.count("room"))
    {
        meta.occupied = true;
        meta.room = kv["room"];
    }

    if (kv.count("timestamp"))
    {
        meta.timestamp = kv["timestamp"];
    }

    return meta;
}

void SaveManager::saveSlot(int slot, const SaveData &data)
{
    std::string filename = slotFilename(slot);
    std::ofstream file(filename, std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "[SaveManager] Could not open save file for writing: "
                  << filename << "\n";
        return;
    }

    file << "timestamp=" << data.timestamp << "\n";
    file << "room=" << data.room << "\n";
    file << "spawn=" << data.spawn << "\n";
    file << "playerAbilities=" << join(enumToInts(data.playerAbilities)) << "\n";
    file << "destroyedTiles=" << serializeDestroyedTiles(data.destroyedTiles) << "\n";
    file << "revealedTiles=" << serializeRevealedTiles(data.revealedTiles) << "\n";

    file.close();
}

SaveData SaveManager::loadSlot(int slot)
{
    std::string filename = slotFilename(slot);
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return SaveData::makeDefault();
    }

    SaveData data;
    std::unordered_map<std::string, std::string> kv;
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') &&
            std::getline(iss, value))
        {
            kv[key] = value;
        }
    }

    if (kv.count("room"))
    {
        data.room = kv["room"];
    }

    if (kv.count("spawn"))
    {
        data.spawn = kv["spawn"];
    }

    if (kv.count("playerAbilities"))
    {
        auto ints = split<int>(kv["playerAbilities"]);
        data.playerAbilities = intsToEnum<PlayerAbility>(ints);
    }

    if (kv.count("destroyedTiles"))
    {
        data.destroyedTiles = deserializeDestroyedTiles(kv["destroyedTiles"]);
    }

    if (kv.count("revealedTiles"))
    {
        data.revealedTiles = deserializeRevealedTilesRLE(kv["revealedTiles"]);
    }

    return data;
}

// Privates

std::string SaveManager::slotFilename(int slot) const
{
    std::filesystem::create_directories(baseDir);
    return baseDir + "/save" + std::to_string(slot) + ".sav";
}

std::string SaveManager::serializeDestroyedTiles(
    const std::unordered_map<std::string, std::unordered_set<TileKey, TileKeyHash>> &destroyed)
{
    std::ostringstream oss;
    bool firstRoom = true;

    for (const auto &[roomName, tiles] : destroyed)
    {
        if (!firstRoom)
        {
            oss << "|";
        }

        firstRoom = false;
        oss << roomName << ":";
        bool firstTile = true;

        for (const auto &tile : tiles)
        {
            if (!firstTile)
            {
                oss << ";";
            }

            firstTile = false;
            oss << "(" << tile.x << "," << tile.y << ")";
        }
    }

    return oss.str();
}

std::unordered_map<std::string, std::unordered_set<TileKey, TileKeyHash>>
SaveManager::deserializeDestroyedTiles(const std::string &str)
{
    std::unordered_map<std::string, std::unordered_set<TileKey, TileKeyHash>> result;
    std::vector<std::string> roomBlocks = split<std::string>(str, '|');

    for (const auto &block : roomBlocks)
    {
        auto pos = block.find(':');
        if (pos == std::string::npos)
        {
            continue;
        }

        std::string roomName = block.substr(0, pos);
        std::string tilesStr = block.substr(pos + 1);
        std::vector<std::string> tileTokens = split<std::string>(tilesStr, ';');

        for (const auto &token : tileTokens)
        {
            if (token.size() < 5)
            {
                continue; // minimal "(x,y)"
            }
            if (token.front() != '(' || token.back() != ')')
            {
                continue;
            }

            std::string coords = token.substr(1, token.size() - 2);
            auto commaPos = coords.find(',');

            if (commaPos == std::string::npos)
            {
                continue;
            }

            int x = std::stoi(coords.substr(0, commaPos));
            int y = std::stoi(coords.substr(commaPos + 1));

            result[roomName].insert(TileKey{x, y});
        }
    }

    return result;
}

std::string SaveManager::serializeRevealedTiles(
    const std::unordered_map<std::string, std::vector<std::vector<bool>>> &revealed)
{
    std::ostringstream oss;
    bool firstRoom = true;

    for (const auto &[roomName, grid] : revealed)
    {
        if (!firstRoom)
        {
            oss << "|";
        }

        firstRoom = false;

        int height = static_cast<int>(grid.size());
        int width = height > 0
                        ? static_cast<int>(grid[0].size())
                        : 0;

        oss << roomName << ":"
            << width << "x" << height << ":"
            << encodeRLE(grid);
    }

    return oss.str();
}

std::unordered_map<std::string, std::vector<std::vector<bool>>>
SaveManager::deserializeRevealedTilesRLE(const std::string &str)
{
    std::unordered_map<std::string, std::vector<std::vector<bool>>> result;
    auto blocks = split<std::string>(str, '|');

    for (auto &block : blocks)
    {
        auto p1 = block.find(':');
        auto p2 = block.find(':', p1 + 1);

        if (p1 == std::string::npos ||
            p2 == std::string::npos)
        {
            continue;
        }

        std::string roomName = block.substr(0, p1);

        // Parse width/height
        std::string dim = block.substr(p1 + 1, p2 - (p1 + 1));
        auto xPos = dim.find('x');
        int width = std::stoi(dim.substr(0, xPos));
        int height = std::stoi(dim.substr(xPos + 1));

        std::string rle = block.substr(p2 + 1);
        result[roomName] = decodeRLE(rle, width, height);
    }

    return result;
}

// Helpers

template <typename T>
std::string SaveManager::join(const std::vector<T> &values, char sep)
{
    std::ostringstream oss;

    for (size_t i = 0; i < values.size(); ++i)
    {
        oss << values[i];
        if (i + 1 < values.size())
        {
            oss << sep;
        }
    }

    return oss.str();
}

template <typename T>
std::vector<T> SaveManager::split(const std::string &s, char sep)
{
    std::vector<T> result;
    std::istringstream iss(s);
    std::string token;

    while (std::getline(iss, token, sep))
    {
        if (!token.empty())
        {
            std::istringstream valStream(token);
            T value;
            valStream >> value;
            result.push_back(value);
        }
    }

    return result;
}

template <typename Enum>
std::vector<int> SaveManager::enumToInts(const std::vector<Enum> &values)
{
    static_assert(std::is_enum_v<Enum>, "enumToInts requires an enum type");

    std::vector<int> out;
    out.reserve(values.size());

    for (auto v : values)
    {
        out.push_back(static_cast<int>(v));
    }

    return out;
}

template <typename Enum>
std::vector<Enum> SaveManager::intsToEnum(const std::vector<int> &ints)
{
    static_assert(std::is_enum_v<Enum>, "intsToEnum requires an enum type");

    std::vector<Enum> out;
    out.reserve(ints.size());

    for (int v : ints)
    {
        out.push_back(static_cast<Enum>(v));
    }

    return out;
}

std::string SaveManager::encodeRLE(const std::vector<std::vector<bool>> &grid)
{
    std::ostringstream oss;

    for (const auto &row : grid)
    {
        int count = 1;
        bool prev = row[0];

        for (int i = 1; i < row.size(); ++i)
        {
            if (row[i] == prev)
                count++;
            else
            {
                oss << count << "x" << (prev ? "1" : "0") << ",";
                prev = row[i];
                count = 1;
            }
        }

        oss << count << "x" << (prev ? "1" : "0") << ";"; // end of row
    }

    return oss.str();
}

std::vector<std::vector<bool>> SaveManager::decodeRLE(
    const std::string &str, int width, int height)
{
    std::vector<std::vector<bool>> grid(height, std::vector<bool>(width, false));

    int x = 0;
    int y = 0;

    auto rows = split<std::string>(str, ';');

    for (auto &rowStr : rows)
    {
        if (rowStr.empty())
        {
            continue;
        }

        auto tokens = split<std::string>(rowStr, ',');

        for (auto &tok : tokens)
        {
            auto pos = tok.find('x');
            int count = std::stoi(tok.substr(0, pos));
            bool val = tok[pos + 1] == '1';

            for (int i = 0; i < count; ++i)
            {
                if (x >= width)
                {
                    x = 0;
                    y++;
                    if (y >= height)
                    {
                        return grid;
                    }
                }

                grid[y][x] = val;
                x++;
            }
        }

        x = 0;
        y++;
        if (y >= height)
        {
            break;
        }
    }

    return grid;
}
