#pragma once

#include "GObject.h"

#include <vector>


struct NORMAL_TEXTURE_VERTEX { FLOAT X, Y, Z; D3DVECTOR NORMAL; FLOAT U, V; };
#define NORMAL_TEXTURE_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

typedef std::vector<NORMAL_TEXTURE_VERTEX> texVertices;

class TextureObject : public GObject
{

public:
    TextureObject(LPDIRECT3DDEVICE9 d3ddev);
    ~TextureObject();

    void draw();
    void setup();
    void setVertices(const texVertices& v);

protected:

    int triangleCount;
    LPDIRECT3DVERTEXBUFFER9 m_vbuffer = NULL;
    IDirect3DTexture9 *m_pTexture;
};
