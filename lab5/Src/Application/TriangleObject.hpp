#pragma once

#include "GObject.h"

#include <vector>

template<class CUSTOMVERTEX, DWORD FVFFLAG>
class TriangleObject : public GObject
{
public:
    
    typedef std::vector<CUSTOMVERTEX> vertices;

    TriangleObject(LPDIRECT3DDEVICE9 d3ddev);
    TriangleObject(LPDIRECT3DDEVICE9 d3ddev, const vertices& v);
    ~TriangleObject();

    void setVertices(const vertices& v);

    static void addTriplet(vertices& vec, CUSTOMVERTEX v1, CUSTOMVERTEX v2, CUSTOMVERTEX v3);
protected:
    void draw();
    void setup();

    int triangleCount;
    LPDIRECT3DVERTEXBUFFER9 m_vbuffer = NULL;
};

template<class CUSTOMVERTEX, DWORD FVFFLAG>
void TriangleObject<CUSTOMVERTEX, FVFFLAG>::addTriplet(vertices& vec, CUSTOMVERTEX v1, CUSTOMVERTEX v2, CUSTOMVERTEX v3)
{
    vec.push_back(v1);
    vec.push_back(v2);
    vec.push_back(v3);
}

template<class CUSTOMVERTEX, DWORD FVFFLAG>
void TriangleObject<CUSTOMVERTEX, FVFFLAG>::draw()
{
    m_d3ddev->SetStreamSource(0, m_vbuffer, 0, sizeof(CUSTOMVERTEX));
    m_d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangleCount);
}

template<class CUSTOMVERTEX, DWORD FVFFLAG>
void TriangleObject<CUSTOMVERTEX, FVFFLAG>::setup()
{
    m_d3ddev->SetFVF(FVFFLAG);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

template<class CUSTOMVERTEX, DWORD FVFFLAG>
TriangleObject<CUSTOMVERTEX, FVFFLAG>::TriangleObject(LPDIRECT3DDEVICE9 d3ddev)
    : triangleCount(0)
    , GObject(d3ddev)
    , m_vbuffer(nullptr)
{
}

template<class CUSTOMVERTEX, DWORD FVFFLAG>
TriangleObject<CUSTOMVERTEX, FVFFLAG>::TriangleObject(LPDIRECT3DDEVICE9 d3ddev, const vertices& v)
    : triangleCount(0)
    , GObject(d3ddev)
    , m_vbuffer(nullptr)
{
    setVertices(v);
}

template<class CUSTOMVERTEX, DWORD FVFFLAG>
TriangleObject<CUSTOMVERTEX, FVFFLAG>::~TriangleObject()
{
    m_vbuffer->Release();
}

template<class CUSTOMVERTEX, DWORD FVFFLAG>
void TriangleObject<CUSTOMVERTEX, FVFFLAG>::setVertices(const vertices& v)
{
    triangleCount = v.size() / 3;
    m_d3ddev->CreateVertexBuffer(sizeof(CUSTOMVERTEX) * v.size(), D3DUSAGE_WRITEONLY, FVFFLAG, D3DPOOL_DEFAULT, &m_vbuffer, NULL);

    VOID* pVoid;
    m_vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, v.data(), sizeof(CUSTOMVERTEX) * v.size());
    m_vbuffer->Unlock();
}

struct COLORVERTEX {
    D3DXVECTOR3 p;
    DWORD c;
};
#define COLORFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct NORMALCOLORVERTEX {
    D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    DWORD c;
};
#define NORMALCOLORFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)