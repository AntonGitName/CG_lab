#include "GObject.h"

GObject::GObject(LPDIRECT3DDEVICE9 d3dDevice)
    : m_d3ddev(d3dDevice)
{
    D3DXMatrixIdentity(&m_matWorld);
}

void GObject::render()
{
    transformWorld();
    setup();
    draw();
}

void GObject::setup()
{
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void GObject::transformWorld()
{
    m_d3ddev->SetTransform(D3DTS_WORLD, &m_matWorld);
}

const D3DXMATRIX* GObject::getTransformMatrix() const
{
    return &m_matWorld;
}