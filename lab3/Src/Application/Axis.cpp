#include "Axis.h"


Axis::Axis(LPDIRECT3DDEVICE9 d3ddev, float length)
    : TriangleObject(d3ddev)
{
    std::vector<NORMALVERTEX> vertices;
    std::vector<D3DVECTOR> normals = {
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, -1.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { -1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, -1.0f }
    };

    for (auto& normal : normals)
    {
        vertices.push_back({ { 0.f, 0.f, -length }, normal, 0xFF666666 });
        vertices.push_back({ { 0.f, 0.f, -length }, normal, 0xFF666666 });
        vertices.push_back({ { 0.f, 0.f, length }, normal, 0xFF666666 });

        vertices.push_back({ { 0.f, -length, 0.f }, normal, 0xFF666666 });
        vertices.push_back({ { 0.f, -length, 0.f }, normal, 0xFF666666 });
        vertices.push_back({ { 0.f, length, 0.f }, normal, 0xFF666666 });

        vertices.push_back({ { -length, 0.f, 0.f }, normal, 0xFF666666 });
        vertices.push_back({ { -length, 0.f, 0.f }, normal, 0xFF666666 });
        vertices.push_back({ { length, 0.f, 0.f }, normal, 0xFF666666 });
    }

    setVertices(vertices);
}

void Axis::setup()
{
    TriangleObject::setup();
    m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
}

void Axis::transformWorld()
{
    static D3DXMATRIX matWorld;
    D3DXMatrixIdentity(&matWorld);
    m_d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}
