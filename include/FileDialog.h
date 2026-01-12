#ifndef FILEDIALOG_H
#define FILEDIALOG_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <shobjidl.h>

// ------ STL ---------------------------------------
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

// ------ Forward Declarations ----------------------
struct HWND__;
typedef struct HWND__* HWND;

class FileDialog
{
public:
    struct Filter
    {
        std::wstring name;
        std::wstring extension;
    };

    static std::optional<std::filesystem::path> Open(HWND ownerHandle, const std::vector<Filter>& filters = {});
    static std::optional<std::filesystem::path> Save(HWND ownerHandle, const std::vector<Filter>& filters = {}, const std::wstring& defaultExtension = L"");

private:
    static std::optional<std::filesystem::path> Show(HWND ownerHandle, REFCLSID scope, const std::vector<Filter>& filters = {}, const std::wstring& defaultExtension = L"");
    static void ConfigureDialog(IFileDialog* pDialog, const std::vector<Filter>& filters, const std::wstring& defaultExtension);
    static std::optional<std::filesystem::path> ExtractPath(IShellItem* pItem);
};

#endif
