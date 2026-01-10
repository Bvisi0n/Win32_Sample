#ifndef FILESERVICE_H
#define FILESERVICE_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Win32 and more ----------------------------
#include <d2d1.h>	    // Direct2D

// ------ STL ---------------------------------------
#include <filesystem>
#include <format>
#include <fstream>
#include <regex>
#include <string>
#include <type_traits>
#include <vector>

template <typename T> // Consider gathering all concepts in TypeTraits.h (Single Source of Truth)
concept BinarySerializable = std::is_trivially_copyable_v<T>;

class FileService
{   //TODO Bug(threat): Missing some error handling
public:
    static void Save(const std::filesystem::path& path, const std::vector<D2D1_ELLIPSE>& data)
    {
        if (path.extension() == ".elip")
        {
            SaveText(path, data);
        }
        else
        {
            SaveBinary(path, data); // Default to binary
        }
    }
    
    static void Load(const std::filesystem::path& path, std::vector<D2D1_ELLIPSE>& data)
    {
        if (path.extension() == ".elip")
        {
            LoadText(path, data);
        }
        else
        {
            LoadBinary(path, data);
        }
    }

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
    static std::vector<float> ExtractFloats(const std::wstring& line)
    {
        static const std::wregex floatRegex(L"[-+]?([0-9]*\\.[0-9]+|[0-9]+)");

        std::vector<float> results;
        auto words_begin = std::wsregex_iterator(line.begin(), line.end(), floatRegex);
        auto words_end = std::wsregex_iterator();

        for (std::wsregex_iterator i = words_begin; i != words_end; ++i)
        {
            try {
                results.push_back(std::stof(i->str()));
            }
            catch (...) {
                continue;
            }
        }
        return results;
    }

    static void SaveText(const std::filesystem::path& path, const std::vector<D2D1_ELLIPSE>& data)
    {
        std::wofstream outFile(path);
        if (!outFile.is_open()) return;

        for (const auto& e : data)
        {
            outFile << std::format(L"{} {} {} {}\n", e.point.x, e.point.y, e.radiusX, e.radiusY);
        }
    }

    static void LoadText(const std::filesystem::path& path, std::vector<D2D1_ELLIPSE>& data)
    {
        std::wifstream inFile(path);
        if (!inFile.is_open()) return;

        data.clear();
        std::wstring line;
        while (std::getline(inFile, line))
        {
            std::vector<float> v = ExtractFloats(line);

            if (v.size() >= 4)
            {
                data.push_back(D2D1::Ellipse(D2D1::Point2F(v[0], v[1]), v[2], v[3]));
            }
        }
    }
};

#endif
