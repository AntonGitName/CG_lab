#pragma once
#include "Sphere.h"

#include <string>

class SphereLight :
    public Sphere
{
public:
    SphereLight(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float radius, D3DXCOLOR color);

private:
    D3DLIGHT9 m_light;
    void drawLight();
    void draw();
};
