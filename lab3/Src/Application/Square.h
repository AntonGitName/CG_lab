#pragma once
#include "TextureObject.h"
class Square :
    public TextureObject
{
public:
    Square(LPDIRECT3DDEVICE9 d3ddev, const D3DXVECTOR3& vec, float size);
private:

    static const int NUMBER_OF_MIPMAPS = 6;
    const LPCWSTR MIPMAP_FILENAME_FMT = L"Resources\\mipmap%d.png";
};

