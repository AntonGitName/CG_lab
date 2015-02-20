#pragma once
#include "TextureObject.h"

#include <string>
#include <random>

class SphereTest :
    public TextureObject
{
public:
    SphereTest(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float radius);
    ~SphereTest();

    void setup();
    void transformWorld();
private:
    const LPCWSTR TEXTURE_FILENAME = L"Resources\\texture_sun.jpg";
    float m_phi;
    float m_speed;
};

