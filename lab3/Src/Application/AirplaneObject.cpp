#include "AirplaneObject.h"


AirplaneObject::AirplaneObject(LPDIRECT3DDEVICE9 d3ddev, int m_carNum)
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

    m_phi = (m_carNum - 1) * D3DX_PI / 4;

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    r -= 0.5f;
    m_phi += r * D3DX_PI / 8;

    m_objectCenter = { 0.0f, -1.5f, 0.0f };
}

void AirplaneObject::switchLights()
{
    m_lightsOn ^= true;
}

void AirplaneObject::drawLights()
{
    m_d3ddev->SetLight(m_carNum * 2, &m_headLamp1);
    m_d3ddev->LightEnable(m_carNum * 2, m_lightsOn);
}

void AirplaneObject::draw()
{
    MeshWrapper::draw();
    drawLights();
}

void AirplaneObject::transformWorld()
{
    static D3DXMATRIX matWorld;
    static D3DXMATRIX tmpMatrix;
    static D3DXVECTOR3 tmpV;

    m_phi += 0.01f;
    if (m_phi > 2 * D3DX_PI)
    {
        m_phi = 0;
    }

    float theta = m_phi * 3;

    tmpV = m_objectCenter;
    m_objectCenter = { (m_outerRadius + m_innerRadius * cos(theta))* cos(m_phi),
                        -5.0f + m_innerRadius * (-1.0f + sin(theta)),
                       (m_outerRadius + m_innerRadius * cos(theta))* sin(m_phi) };
    tmpV.x -= m_objectCenter.x;
    tmpV.y -= m_objectCenter.y;
    tmpV.z -= m_objectCenter.z;
    D3DXVec3Normalize(&tmpV, &tmpV);
    float kx = -tmpV.x;
    float ky = -tmpV.y;
    float kz = -tmpV.z;
    m_headLamp1.Direction = D3DXVECTOR3(kx, ky, kz);
    m_headLamp1.Position = { m_objectCenter.x + 2 * tmpV.x, m_objectCenter.y + 2 * tmpV.y, m_objectCenter.z + 2 * tmpV.z };

    D3DXMatrixIdentity(&matWorld);
    
    float f = atan(abs(ky / kz));
    D3DXMatrixRotationX(&tmpMatrix, f + D3DX_PI);
    matWorld *= tmpMatrix;
    
    f = atan(abs(kx / kz));
    D3DXMatrixRotationY(&tmpMatrix, f);
    matWorld *= tmpMatrix;
    
    f = atan(abs(ky / kx));
    D3DXMatrixRotationZ(&tmpMatrix, f);
    matWorld *= tmpMatrix;

    D3DXMatrixTranslation(&tmpMatrix, m_objectCenter.x, m_objectCenter.y, m_objectCenter.z);
    matWorld *= tmpMatrix;

    m_d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

AirplaneObject::~AirplaneObject()
{
}
