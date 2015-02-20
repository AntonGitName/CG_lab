#include "TextureObject.h"

void TextureObject::draw()
{
    m_d3ddev->SetStreamSource(0, m_vbuffer, 0, sizeof(NORMAL_TEXTURE_VERTEX));
    m_d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangleCount);
}

void TextureObject::setup()
{
    m_d3ddev->SetFVF(NORMAL_TEXTURE_FVF);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
    m_d3ddev->SetTexture(0, m_pTexture);
}

TextureObject::TextureObject(LPDIRECT3DDEVICE9 d3ddev)
    : triangleCount(0)
    , GObject(d3ddev)
    , m_vbuffer(nullptr)
{
}

TextureObject::~TextureObject()
{
    m_vbuffer->Release();
    m_pTexture->Release();
}

void TextureObject::setVertices(const texVertices& v)
{
    triangleCount = v.size() / 3;
    // create a vertex buffer interface called v_buffer
    m_d3ddev->CreateVertexBuffer(sizeof(NORMAL_TEXTURE_VERTEX) * v.size(),
        D3DUSAGE_WRITEONLY,
        NORMAL_TEXTURE_FVF,
        D3DPOOL_DEFAULT,
        &m_vbuffer,
        NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    m_vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, v.data(), sizeof(NORMAL_TEXTURE_VERTEX) * v.size());
    m_vbuffer->Unlock();
}