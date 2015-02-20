#include "GObject.h"

GObject::GObject(LPDIRECT3DDEVICE9 d3dDevice)
    : m_d3ddev(d3dDevice)
{
    m_pTransfrom.reset(new Transformation());
    D3DXMatrixIdentity(&m_matWorld);
    D3DXMatrixIdentity(&m_accumulatedMatWorld);
}

GObject::GObject(LPDIRECT3DDEVICE9 d3dDevice, SHPTRTranform pTransfrom)
    : m_d3ddev(d3dDevice)
{
    m_pTransfrom = pTransfrom;
    D3DXMatrixIdentity(&m_matWorld);
    D3DXMatrixIdentity(&m_accumulatedMatWorld);
}

void GObject::setTransformation(SHPTRTranform pTransfrom)
{
    m_pTransfrom = pTransfrom;
}
void GObject::setTransformation(Transformation *pTransfrom)
{
    m_pTransfrom.reset(pTransfrom);
}

void GObject::render(const D3DXMATRIX* preTransform)
{
    transformWorld(preTransform);
    setup();
    draw();
}

void GObject::setup()
{
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void GObject::transformWorld(const D3DXMATRIX* preTransform)
{
    m_pTransfrom->transform(&m_matWorld);
    m_accumulatedMatWorld = m_matWorld;
    if (preTransform != nullptr)
    {
        m_accumulatedMatWorld *= *preTransform;
    }
    m_d3ddev->SetTransform(D3DTS_WORLD, &m_accumulatedMatWorld);
}

const D3DXMATRIX* GObject::getTransformMatrix() const
{
    return &m_accumulatedMatWorld;
}