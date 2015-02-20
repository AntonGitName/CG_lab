#include "Cylinder.h"

namespace
{

    D3DXVECTOR3 getCylinderPoint(float u, float v)
    {
        return{ cos(u), sin(u), v };
    }

    void transformPoint(D3DXVECTOR3* p, const D3DXVECTOR3& center, const D3DXVECTOR3& orientation, float radius, float height)
    {
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
        D3DXVec3TransformCoord(p, p, &mat);
    }

    NORMALCOLORVERTEX getCylinderVertex(float u, float v, const D3DXVECTOR3& center, const D3DXVECTOR3& orientation, float radius, float height, D3DXCOLOR color)
    {
        D3DXVECTOR3 p = getCylinderPoint(u * 2 * D3DX_PI, v - 0.5f);
        D3DXVECTOR3 n = p;
        n.x = 0;
        n.y = 0;
        transformPoint(&p, center, orientation, radius, height);
        transformPoint(&n, center, orientation, radius, height);
        D3DXVec3Subtract(&n, &p, &n);
        D3DXVec3Normalize(&n, &n);
        return{ p, n, color };
    }

    void pushTriangle(TriangleObject<NORMALCOLORVERTEX, COLORFVF>::vertices  &vertices, const NORMALCOLORVERTEX& v1, const NORMALCOLORVERTEX& v2, const NORMALCOLORVERTEX& v3)
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
            NORMALCOLORVERTEX v1 = getCylinderVertex(i * step, j * step, center, orientation, radius, height, color);
            NORMALCOLORVERTEX v2 = getCylinderVertex(i * step, (j + 1) * step, center, orientation, radius, height, color);
            NORMALCOLORVERTEX v3 = getCylinderVertex((i + 1) * step, j * step, center, orientation, radius, height, color);
            NORMALCOLORVERTEX v4 = getCylinderVertex((i + 1) * step, (j + 1) * step, center, orientation, radius, height, color);

            pushTriangle(vertices, v1, v2, v3);
            pushTriangle(vertices, v3, v2, v4);
        }
    }    
    setVertices(vertices);
} 