#ifndef FILEDIALOG_H
#define FILEDIALOG_H

// ------ Project wide settings ---------------------
#include "config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>	// The core
#include <shobjidl.h>   // IFileOpenDialog (file open popup)
#include <wrl/client.h> // Microsoft::WRL::ComPtr (smartpointer)

// ------ STL ---------------------------------------
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

class FileDialog
{
public:
    struct Filter
    {
        std::wstring name;
        std::wstring extension;
    };

    static std::optional<std::filesystem::path> Open(HWND ownerHandle, const std::vector<Filter>& filters = {})
    {
        return Show(ownerHandle, CLSID_FileOpenDialog, filters);
    }

    static std::optional<std::filesystem::path> Save(HWND ownerHandle, const std::vector<Filter>& filters = {}, const std::wstring& defaultExtension = L"")
    {
        return Show(ownerHandle, CLSID_FileSaveDialog, filters, defaultExtension);
    }

private:
    static std::optional<std::filesystem::path> Show(HWND ownerHandle, REFCLSID scope, const std::vector<Filter>& filters = {}, const std::wstring& defaultExtension = L"")
    {
        using Microsoft::WRL::ComPtr;
        ComPtr<IFileDialog> pDialog;

        if (FAILED(CoCreateInstance(scope, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog))))
        {
            return std::nullopt;
        }

        ConfigureDialog(pDialog.Get(), filters, defaultExtension);

        if (SUCCEEDED(pDialog->Show(ownerHandle)))
        {
            ComPtr<IShellItem> pItem;
            if (SUCCEEDED(pDialog->GetResult(&pItem)))
            {
                return ExtractPath(pItem.Get());
            }
        }

        return std::nullopt;
    }

    static void ConfigureDialog(IFileDialog* pDialog, const std::vector<Filter>& filters, const std::wstring& defaultExtension)
    {
        if (!filters.empty())
        {
            std::vector<COMDLG_FILTERSPEC> extensions;
            for (const auto& f : filters)
            {
                extensions.push_back({ f.name.c_str(), f.extension.c_str() });
            }
            pDialog->SetFileTypes(static_cast<UINT>(extensions.size()), extensions.data());
        }

        if (!defaultExtension.empty())
        {
            pDialog->SetDefaultExtension(defaultExtension.c_str());
        }
    }

    static std::optional<std::filesystem::path> ExtractPath(IShellItem* pItem)
    {
        PWSTR pszPath = nullptr; // Pointer String Zero-terminated FilePath
        if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath)))
        {   // R.1: Manage resources automatically using resource handles and RAII
            std::unique_ptr<wchar_t, decltype(&CoTaskMemFree)> pathWrapper(pszPath, CoTaskMemFree);
            return std::filesystem::path(pathWrapper.get());
        }
        return std::nullopt;
    }
};

#endif
