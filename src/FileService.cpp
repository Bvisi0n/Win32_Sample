// ------ Win32 and more ----------------------------
#include <d2d1.h>

// ------ STL ---------------------------------------
#include <format>
#include <fstream>
#include <regex>
#include <string>

// ------ Homebrew ----------------------------------
#include "FileService.h"

void FileService::Save(const std::filesystem::path& path, const std::vector<D2D1_ELLIPSE>& data)
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

void FileService::Load(const std::filesystem::path& path, std::vector<D2D1_ELLIPSE>& data)
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

std::vector<float> FileService::ExtractFloats(const std::wstring& line)
{
    // Wrapped in () because conditional logic, | means OR
    // [-+]? optional sign
    // [0-9]*\\.[0-9]+ digits & decimal part
    // [0-9]+ digits without decimal part
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

void FileService::SaveText(const std::filesystem::path& path, const std::vector<D2D1_ELLIPSE>& data)
{
    std::wofstream outFile(path);
    if (!outFile.is_open()) return;

    for (const auto& e : data)
    {
        outFile << std::format(L"{} {} {} {}\n", e.point.x, e.point.y, e.radiusX, e.radiusY);
    }
}

void FileService::LoadText(const std::filesystem::path& path, std::vector<D2D1_ELLIPSE>& data)
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
