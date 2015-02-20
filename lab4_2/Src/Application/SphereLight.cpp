#include "SphereLight.h"

SphereLight::SphereLight(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float radius, D3DXCOLOR color)
    : Sphere(d3ddev, center, radius, color, true)
{
    ZeroMemory(&m_light, sizeof(m_light));
    m_light.Type = D3DLIGHT_POINT;                         // make the light type 'point light'
    m_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f);   // 0xFFFF8000;//D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
    m_light.Position = center;
    m_light.Range = 7.5f;                                    // a range of 10
    m_light.Attenuation0 = 0.0f;                           // no constant inverse attenuation
    m_light.Attenuation1 = 0.225f;                         // only .125 inverse attenuation
    m_light.Attenuation2 = 0.0f;                           // no square inverse attenuation
}

void SphereLight::drawLight()
{
    D3DXVECTOR3 p = getCenter();
    D3DXVec3TransformCoord(&p, &p, getTransformMatrix());
    m_light.Position = { p.x, p.y, p.z };
    m_d3ddev->SetLight(0, &m_light);
    m_d3ddev->LightEnable(0, true);
}

void SphereLight::draw()
{
    drawLight();
    Sphere::draw();
}