// ------ Win32 and more ----------------------------
#include <wrl/client.h>

// ------ STL ---------------------------------------
#include <memory>

// ------ Homebrew ----------------------------------
#include "FileDialog.h"

std::optional<std::filesystem::path> FileDialog::Open(HWND ownerHandle, const std::vector<Filter>& filters)
{
    return Show(ownerHandle, CLSID_FileOpenDialog, filters);
}

std::optional<std::filesystem::path> FileDialog::Save(HWND ownerHandle, const std::vector<Filter>& filters, const std::wstring& defaultExtension)
{
    return Show(ownerHandle, CLSID_FileSaveDialog, filters, defaultExtension);
}

std::optional<std::filesystem::path> FileDialog::Show(HWND ownerHandle, REFCLSID scope, const std::vector<Filter>& filters, const std::wstring& defaultExtension)
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

void FileDialog::ConfigureDialog(IFileDialog* pDialog, const std::vector<Filter>& filters, const std::wstring& defaultExtension)
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

std::optional<std::filesystem::path> FileDialog::ExtractPath(IShellItem* pItem)
{
    PWSTR pszPath = nullptr; // Pointer String Zero-terminated FilePath
    if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath)))
    {   // R.1: Manage resources automatically using resource handles and RAII
        std::unique_ptr<wchar_t, decltype(&CoTaskMemFree)> pathWrapper(pszPath, CoTaskMemFree);
        return std::filesystem::path(pathWrapper.get());
    }
    return std::nullopt;
}
