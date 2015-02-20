#pragma once

#include "GObject.h"

#include <vector>

struct COLORVERTEX {
    D3DXVECTOR3 p;
    DWORD c;
};
#define COLORFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

typedef std::vector<COLORVERTEX> vertices;

class TriangleObject : public GObject
{

public:
    TriangleObject(LPDIRECT3DDEVICE9 d3ddev);
    TriangleObject(LPDIRECT3DDEVICE9 d3ddev, const vertices& v);
    ~TriangleObject();

    void setVertices(const vertices& v);
    D3DXVECTOR3 getCenter() const {
        return{ 0.f, 0.f, 0.f };
    }

protected:
	void draw();
	void setup();

    int triangleCount;
    LPDIRECT3DVERTEXBUFFER9 m_vbuffer = NULL;
};

