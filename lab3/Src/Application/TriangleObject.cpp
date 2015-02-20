#include "TriangleObject.h"

void TriangleObject::draw()
{
    m_d3ddev->SetStreamSource(0, m_vbuffer, 0, sizeof(NORMALVERTEX));
    m_d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangleCount);
    
}

void TriangleObject::setup()
{
    m_d3ddev->SetFVF(NORMALFVF);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, true);
}

TriangleObject::TriangleObject(LPDIRECT3DDEVICE9 d3ddev)
    : triangleCount(0)
    , GObject(d3ddev)
    , m_vbuffer(nullptr)
{
}
TriangleObject::TriangleObject(LPDIRECT3DDEVICE9 d3ddev, const vertices& v)
    : triangleCount(0)
    , GObject(d3ddev)
    , m_vbuffer(nullptr)
{
    setVertices(v);
}

TriangleObject::~TriangleObject()
{
    m_vbuffer->Release();
}

void TriangleObject::setVertices(const vertices& v)
{
    triangleCount = v.size() / 3;
    // create a vertex buffer interface called v_buffer
    m_d3ddev->CreateVertexBuffer(sizeof(NORMALVERTEX) * v.size(),
        D3DUSAGE_WRITEONLY,
        NORMALFVF,
        D3DPOOL_DEFAULT,
        &m_vbuffer,
        NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    m_vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, v.data(), sizeof(NORMALVERTEX) * v.size());
    m_vbuffer->Unlock();
}