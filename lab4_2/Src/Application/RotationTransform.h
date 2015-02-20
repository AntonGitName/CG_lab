#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "Transformation.h"

class RotationTransform : public Transformation {
public:

    RotationTransform(float phi, D3DXVECTOR3 rotPointA, float theta, D3DXVECTOR3 rotPointB)
        : m_phi(phi)
        , m_theta(theta)
        , m_rotPointA(rotPointA)
        , m_rotPointB(rotPointB)
        , m_t((float) rand())
        , m_speed(1.0f + (float)rand() / (float) RAND_MAX)
    {}

    virtual void transform(D3DXMATRIX* worldMatrix)
    {
        D3DXMATRIX tmp;
        m_t += 0.02f * m_speed;
        D3DXMatrixTranslation(&tmp, -m_rotPointA.x, -m_rotPointA.y, -m_rotPointA.z);
        *worldMatrix *= tmp;
        D3DXMatrixRotationY(&tmp, m_phi * cos(m_t));
        *worldMatrix *= tmp;
        D3DXMatrixTranslation(&tmp, m_rotPointA.x, m_rotPointA.y, m_rotPointA.z);
        *worldMatrix *= tmp;
        
        D3DXMatrixTranslation(&tmp, -m_rotPointB.x, -m_rotPointB.y, -m_rotPointB.z);
        *worldMatrix *= tmp;
        D3DXMatrixRotationX(&tmp, m_theta * cos(m_t));
        *worldMatrix *= tmp;
        D3DXMatrixRotationZ(&tmp, m_theta * sin(m_t));
        *worldMatrix *= tmp;
        D3DXMatrixTranslation(&tmp, m_rotPointB.x, m_rotPointB.y, m_rotPointB.z);
        *worldMatrix *= tmp;

    }

private:
    float           m_phi;
    float           m_t;
    float           m_theta;
    float           m_speed;
    D3DXVECTOR3     m_rotPointA;
    D3DXVECTOR3     m_rotPointB;
};