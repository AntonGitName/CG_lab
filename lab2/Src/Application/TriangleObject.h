#pragma once

#include "GObject.h"

#include <vector>

struct CUSTOMVERTEX { FLOAT X, Y, Z; D3DVECTOR NORMAL; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL)

typedef std::vector<CUSTOMVERTEX> vertices;

class TriangleObject : public GObject
{

public:
    TriangleObject(LPDIRECT3DDEVICE9 d3ddev);
    TriangleObject(LPDIRECT3DDEVICE9 d3ddev, const vertices& v);
    ~TriangleObject();

    void draw();
    void setVertices(const vertices& v);
    void render();

protected:
    
    virtual void transformWorld() {}

    int triangleCount;
    LPDIRECT3DDEVICE9 m_d3ddev;
    LPDIRECT3DVERTEXBUFFER9 m_vbuffer = NULL;
};

