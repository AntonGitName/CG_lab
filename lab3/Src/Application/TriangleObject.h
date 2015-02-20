#pragma once

#include "GObject.h"

#include <vector>

struct NORMALVERTEX {
    D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    DWORD c;
};
#define NORMALFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)

typedef std::vector<NORMALVERTEX> vertices;

class TriangleObject : public GObject
{

public:
    TriangleObject(LPDIRECT3DDEVICE9 d3ddev);
    TriangleObject(LPDIRECT3DDEVICE9 d3ddev, const vertices& v);
    ~TriangleObject();

    void draw();
    void setup();
    void setVertices(const vertices& v);

protected:
    
    virtual void transformWorld() {}

    int triangleCount;
    LPDIRECT3DVERTEXBUFFER9 m_vbuffer = NULL;
};

