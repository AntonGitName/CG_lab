#include "CarObject.h"


CarObject::CarObject(LPDIRECT3DDEVICE9 d3ddev, int m_carNum)
    : MeshWrapper(FILENAME, d3ddev)
    , m_carNum(m_carNum)
    , m_lightsOn(true)
{
    ZeroMemory(&m_headLamp1, sizeof(m_headLamp1));
    m_headLamp1.Type = D3DLIGHT_SPOT;    // make the light type 'spot light'
    m_headLamp1.Diffuse = D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f);
    m_headLamp1.Range = 50.0f;    // a range of 100
    m_headLamp1.Attenuation0 = 0.0f;    // no constant inverse attenuation
    m_headLamp1.Attenuation1 = 0.125f;    // only .125 inverse attenuation
    m_headLamp1.Attenuation2 = 0.0f;    // no square inverse attenuation
    m_headLamp1.Phi = D3DXToRadian(40.0f);    // set the outer cone to 30 degrees
    m_headLamp1.Theta = D3DXToRadian(20.0f);    // set the inner cone to 10 degrees
    m_headLamp1.Falloff = 1.0f;    // use the typical falloff

    ZeroMemory(&m_headLamp2, sizeof(m_headLamp2));
    m_headLamp2.Type = D3DLIGHT_SPOT;    // make the light type 'spot light'
    m_headLamp2.Diffuse = D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f);
    m_headLamp2.Range = 50.0f;    // a range of 100
    m_headLamp2.Attenuation0 = 0.0f;    // no constant inverse attenuation
    m_headLamp2.Attenuation1 = 0.125f;    // only .125 inverse attenuation
    m_headLamp2.Attenuation2 = 0.0f;    // no square inverse attenuation
    m_headLamp2.Phi = D3DXToRadian(40.0f);    // set the outer cone to 30 degrees
    m_headLamp2.Theta = D3DXToRadian(20.0f);    // set the inner cone to 10 degrees
    m_headLamp2.Falloff = 1.0f;    // use the typical falloff

    m_phi = (m_carNum - 1) * D3DX_PI / 2;

    m_carCenter = { 0.0f, -2.5f, 0.0f };
}

void CarObject::switchLights()
{
    m_lightsOn ^= true;
}

void CarObject::drawLights()
{
    m_pd3dDevice->SetLight(m_carNum * 2, &m_headLamp1);
    m_pd3dDevice->LightEnable(m_carNum * 2, m_lightsOn);
    m_pd3dDevice->SetLight(m_carNum * 2 + 1, &m_headLamp2);
    m_pd3dDevice->LightEnable(m_carNum * 2 + 1, m_lightsOn);
}

void CarObject::draw()
{
    MeshWrapper::draw();
    drawLights();
}

void CarObject::transformWorld()
{
    static D3DXMATRIX matWorld;
    static D3DXMATRIX tmpMatrix;
    static D3DXVECTOR3 tmpV;

    m_phi += 0.003f;
    if (m_phi > 2 * D3DX_PI)
    {
        m_phi = 0;
    }
    tmpV = m_carCenter;
    m_carCenter = { -30.0f * sin(3 * m_phi), m_carCenter.y, -30.0f * cos(7 * m_phi) };
    tmpV.x -= m_carCenter.x;
    tmpV.y = 0;
    tmpV.z -= m_carCenter.z;
    D3DXVec3Normalize(&tmpV, &tmpV);
    float kx = -tmpV.x;
    float ky = -tmpV.z;
    m_headLamp1.Direction = D3DXVECTOR3(kx, 0.0f, ky);
    m_headLamp1.Position = { +ky + 7.0f * kx + m_carCenter.x, -1.3f, -kx + 7.0f * ky + m_carCenter.z };
    m_headLamp2.Direction = D3DXVECTOR3(kx, 0.0f, ky);
    m_headLamp2.Position = { -ky + 7.0f * kx + m_carCenter.x, -1.3f, +kx + 7.0f * ky + m_carCenter.z };

    D3DXMatrixRotationY(&matWorld, -atan2(ky, kx) + D3DX_PI);

    D3DXMatrixTranslation(&tmpMatrix, m_carCenter.x, m_carCenter.y, m_carCenter.z);
    matWorld *= tmpMatrix;
    m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

CarObject::~CarObject()
{
}
