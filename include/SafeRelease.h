#ifndef SAFERELEASE_H
#define SAFERELEASE_H

#include "Config.h"

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

#endif
