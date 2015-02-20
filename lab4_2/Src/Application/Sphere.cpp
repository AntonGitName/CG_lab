#include "Sphere.h"

namespace
{
    D3DXVECTOR3 getSpherePoint(float u, float v)
    {
        return{ cos(u) * sin(v), sin(u) * sin(v), cos(v) };
    }

    NORMALCOLORVERTEX getSphereVertex(D3DXVECTOR3 p, D3DXVECTOR3 center, float radius, D3DXCOLOR clr, bool inv)
    {
        D3DXMATRIX mat, tmp;
        D3DXVECTOR3 normal;
        D3DXMatrixScaling(&tmp, radius, radius, radius);
        mat = tmp;
        D3DXMatrixTranslation(&tmp, center.x, center.y, center.z);
        mat *= tmp;
        D3DXVec3TransformCoord(&p, &p, &mat);
        inv ? D3DXVec3Subtract(&normal, &center, &p) : D3DXVec3Subtract(&normal, &p, &center);
        D3DXVec3Normalize(&normal, &normal);
        return{ p, normal, clr };
    }

    void pushTriangle(TriangleObject<NORMALCOLORVERTEX, COLORFVF>::vertices  &vertices, const NORMALCOLORVERTEX& v1, const NORMALCOLORVERTEX& v2, const NORMALCOLORVERTEX& v3)
    {
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }
}

Sphere::Sphere(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float radius, D3DXCOLOR color, bool inverse)
    : TriangleObject(d3ddev), m_center(center)
{
    std::vector<NORMALCOLORVERTEX> vertices;

    const int n = 50;
    const float invi = 2.0f * D3DX_PI / (float)n;
    const float invj = D3DX_PI / (float)n;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            NORMALCOLORVERTEX v1 = getSphereVertex(getSpherePoint(i * invi, j * invj), center, radius, color, inverse);
            NORMALCOLORVERTEX v2 = getSphereVertex(getSpherePoint((i + 1) * invi, j * invj), center, radius, color, inverse);
            NORMALCOLORVERTEX v3 = getSphereVertex(getSpherePoint(i * invi, (j + 1) * invj), center, radius, color, inverse);
            NORMALCOLORVERTEX v4 = getSphereVertex(getSpherePoint((i + 1) * invi, (j + 1) * invj), center, radius, color, inverse);
            pushTriangle(vertices, v1, v2, v3);
            pushTriangle(vertices, v3, v2, v4);
        }
    }

    setVertices(vertices);
}

void Sphere::setup()
{
    TriangleObject::setup();
    //m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
}