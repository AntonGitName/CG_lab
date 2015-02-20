#pragma once
#include "TriangleObject.hpp"

struct TEXTURE_VERTEX {
    D3DXVECTOR3 pos;
    float u, v;
};
#define TEXTURE_FVF (D3DFVF_XYZ | D3DFVF_TEX1)

class Ground : public TriangleObject<TEXTURE_VERTEX, TEXTURE_FVF>
{
public:
    Ground(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float width);
    ~Ground();

private:
    void setup();

    IDirect3DTexture9      *m_pTexture;
    static const int        NUMBER_OF_MIPMAPS = 5;
    const LPCWSTR           MIPMAP_FILENAME_FMT = L"Res\\sand_texture%d.jpg";
};

