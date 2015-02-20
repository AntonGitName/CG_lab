#pragma once
#include "TextureObject.h"

#include <string>

class SphereObject :
    public TextureObject
{
public:
    SphereObject(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float radius);
    ~SphereObject();

    void transformWorld();
private:
    static const int NUMBER_OF_MIPMAPS = 6;
    const LPCWSTR MIPMAP_FILENAME_FMT = L"Resources\\mipmap%d.png";
};

