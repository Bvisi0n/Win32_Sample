#ifndef SAFERELEASE_H
#define SAFERELEASE_H

// Project wide settings
#include "Config.h"

// TODO: transition to Microsoft::WRL::ComPtr (if time allows)

template <typename T>
concept ComObject = std::derived_from<T, IUnknown>;

template <ComObject T>
void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}

#endif
