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
    D3DXVECTOR3 getCenter() const {
        return{ 0.f, 0.f, 0.f };
    }

protected:
    void draw();
    void setup();

    int triangleCount;
    LPDIRECT3DVERTEXBUFFER9 m_vbuffer = NULL;
};

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
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, true);
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
    // create a vertex buffer interface called v_buffer
    m_d3ddev->CreateVertexBuffer(sizeof(CUSTOMVERTEX) * v.size(),
        D3DUSAGE_WRITEONLY,
        FVFFLAG,
        D3DPOOL_DEFAULT,
        &m_vbuffer,
        NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    m_vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, v.data(), sizeof(CUSTOMVERTEX) * v.size());
    m_vbuffer->Unlock();
}
