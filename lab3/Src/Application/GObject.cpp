#include "GObject.h"

GObject::GObject(LPDIRECT3DDEVICE9 d3dDevice)
    : m_d3ddev(d3dDevice)
{
}

void GObject::render()
{
    setup();
    transformWorld();
    draw();
}

void GObject::setup()
{
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void GObject::transformWorld()
{
    static D3DXMATRIX matWorld;
    D3DXMatrixIdentity(&matWorld);
    m_d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}