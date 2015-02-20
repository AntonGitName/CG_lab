#include "Cylinder.h"

namespace
{

    D3DXVECTOR3 getCylinderPoint(float u, float v)
    {
        return{ cos(u), sin(u), v };
    }

    COLORVERTEX getCylinderNormalPoint(float u, float v, D3DXCOLOR clr)
    {

        return{ getCylinderPoint(u * D3DX_PI * 2.0f, v - 0.5f), clr };
    }

    void pushTriangle(vertices &vertices, const COLORVERTEX& v1, const COLORVERTEX& v2, const COLORVERTEX& v3)
    {
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }
}

Cylinder::Cylinder(LPDIRECT3DDEVICE9 d3ddev, const D3DXVECTOR3& center, const D3DXVECTOR3& orientation, float radius, float height, D3DXCOLOR color)
    : TriangleObject(d3ddev)
    , m_center(center)
{
    vertices vertices;
    const int n = 50;
    const float min = -50.0f;
    const float max = +50.0f;
    const float step = 1.0f / n;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            COLORVERTEX v1 = getCylinderNormalPoint(i * step, j * step, color);
            COLORVERTEX v2 = getCylinderNormalPoint(i * step, (j + 1) * step, color);
            COLORVERTEX v3 = getCylinderNormalPoint((i + 1) * step, j * step, color);
            COLORVERTEX v4 = getCylinderNormalPoint((i + 1) * step, (j + 1) * step, color);
            pushTriangle(vertices, v1, v2, v3);
            pushTriangle(vertices, v3, v2, v4);
        }
    }
    D3DXMATRIX mat, tmp;
    D3DXMatrixScaling(&tmp, radius, radius, height);
    mat = tmp;
    D3DXMatrixRotationX(&tmp, atan2(orientation.z, orientation.y));
    mat *= tmp;
    D3DXMatrixRotationY(&tmp, atan2(orientation.x, orientation.z));
    mat *= tmp;
    D3DXMatrixRotationZ(&tmp, atan2(orientation.y, orientation.x));
    mat *= tmp;
    D3DXMatrixTranslation(&tmp, center.x, center.y, center.z);
    mat *= tmp;
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        D3DXVec3TransformCoord(&vertices[i].p, &vertices[i].p, &mat);
    }
    
    setVertices(vertices);
} 