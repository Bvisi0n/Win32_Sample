#ifndef FILESERVICE_H
#define FILESERVICE_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ STL ---------------------------------------
#include <filesystem>
#include <vector>
#include <type_traits>

// ------ Homebrew ----------------------------------
#include "Concepts.h"

// ------ Forward Declarations ----------------------
struct D2D1_ELLIPSE;

class FileService
{   //TODO Bug(threat): Missing some error handling
public:
    static void Save(const std::filesystem::path& path, const std::vector<D2D1_ELLIPSE>& data);
    static void Load(const std::filesystem::path& path, std::vector<D2D1_ELLIPSE>& data);

    template <Concepts::BinarySerializable T>
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

    template <Concepts::BinarySerializable T>
    static void LoadBinary(const std::filesystem::path& path, std::vector<T>& data)
    {
        std::ifstream inFile(path, std::ios::binary);
        if (!inFile.is_open()) return;

        size_t count = 0;
        inFile.read(reinterpret_cast<char*>(&count), sizeof(count));

        data.resize(count);
        if (count > 0)
        {
            inFile.read(reinterpret_cast<char*>(data.data()), count * sizeof(T));
        }
    }

private:
    static std::vector<float> ExtractFloats(const std::wstring& line);
    static void SaveText(const std::filesystem::path& path, const std::vector<D2D1_ELLIPSE>& data);
    static void LoadText(const std::filesystem::path& path, std::vector<D2D1_ELLIPSE>& data);
};

#endif
