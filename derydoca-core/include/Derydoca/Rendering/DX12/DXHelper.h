#pragma once
#include <string>

#if defined(_DEBUG)
#define NAME_D3D_OBJECT(x) SetName((x).Get(), L#x)
#else
#define NAME_D3D_OBJECT(x)
#endif

#if defined(_DEBUG)

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        D_LOG_CRITICAL("DXFailure: {0:x}", hr);
        exit(-1);
    }
}

void SetName(ID3D12Object* pObject, LPCWSTR name)
{
    pObject->SetName(name);
}

std::string CharArrToStr(const wchar_t* arr, const size_t maxLength)
{
    std::string string = "";

    for (size_t i = 0; i < maxLength; i++)
    {
        wchar_t c = arr[i];
        if (c == 0)
        {
            break;
        }
        string += c;
    }

    return string;
}
#endif