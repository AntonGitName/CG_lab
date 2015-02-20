#include "Ground.h"


Ground::Ground(LPDIRECT3DDEVICE9 d3ddev)
    : TriangleObject(d3ddev)
{
    std::vector<CUSTOMVERTEX> vertices;
    const D3DVECTOR normal = { 0.0f, -1.0f, 0.0f };
    const int n = 50;
    const float min = -50.0f;
    const float max = +50.0f;
    const float step = (max - min) / n;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            CUSTOMVERTEX v1 = { min + i * step, 0, min + j * step, normal };
            CUSTOMVERTEX v2 = { min + i * step, 0, min + (j + 1) * step, normal };
            CUSTOMVERTEX v3 = { min + (i + 1) * step, 0, min + j * step, normal };
            CUSTOMVERTEX v4 = { min + (i + 1) * step, 0, min + (j + 1) * step, normal };

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            vertices.push_back(v2);
            vertices.push_back(v3);
            vertices.push_back(v4);
        }
    }
    setVertices(vertices);
}

void Ground::transformWorld()
{
    static D3DXMATRIX matWorld;
    D3DXMatrixIdentity(&matWorld);
    m_d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

void Ground::render()
{
    m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    GObject::render();
}