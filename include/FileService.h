#ifndef FILESERVICE_H
#define FILESERVICE_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ STL ---------------------------------------
#include <filesystem>
#include <fstream>
#include <vector>
#include <type_traits>

template <typename T> // Consider gathering all concepts in TypeTraits.h (Single Source of Truth)
concept BinarySerializable = std::is_trivially_copyable_v<T>;

class FileService
{   //TODO Bug(threat): Missing error handling
public:
    template <BinarySerializable T>
    static void SaveBinary(const std::filesystem::path& path, const std::vector<T>& data)
    {
        std::ofstream outFile(path, std::ios::binary);
        if (!outFile.is_open()) return;

        const size_t count = data.size();
        outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));

        if (count > 0)
        {
            outFile.write(reinterpret_cast<const char*>(data.data()), count * sizeof(T));
        }
    }

    template <BinarySerializable T>
    static void LoadBinary(const std::filesystem::path& path, std::vector<T>& outData)
    {
        std::ifstream inFile(path, std::ios::binary);
        if (!inFile.is_open()) return;

        size_t count = 0;
        inFile.read(reinterpret_cast<char*>(&count), sizeof(count));

        outData.resize(count);
        if (count > 0)
        {
            inFile.read(reinterpret_cast<char*>(outData.data()), count * sizeof(T));
        }
    }

};

#endif
