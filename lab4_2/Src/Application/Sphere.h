#pragma once
#include "TriangleObject.hpp"

#include <string>

class Sphere :
    public TriangleObject<NORMALCOLORVERTEX, NORMALCOLORFVF>
{
public:
    Sphere(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float radius, D3DXCOLOR color, bool inverse = false);
    D3DXVECTOR3 getCenter()
    {
        return m_center;
    }
private:
    D3DXVECTOR3 m_center;
    void setup();
};
