#include "Axis.h"


Axis::Axis(LPDIRECT3DDEVICE9 d3ddev, float length)
    : TriangleObject(d3ddev)
{
    std::vector<CUSTOMVERTEX> vertices;
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
        const D3DVECTOR normal = { 0.0f, 1.0f, 0.0f };
        vertices.push_back({ 0.f, 0.f, -length, normal });
        vertices.push_back({ 0.f, 0.f, -length, normal });
        vertices.push_back({ 0.f, 0.f, length, normal });

        vertices.push_back({ 0.f, -length, 0.f, normal });
        vertices.push_back({ 0.f, -length, 0.f, normal });
        vertices.push_back({ 0.f, length, 0.f, normal });

        vertices.push_back({ -length, 0.f, 0.f, normal });
        vertices.push_back({ -length, 0.f, 0.f, normal });
        vertices.push_back({ length, 0.f, 0.f, normal });
    }

    setVertices(vertices);
}

void Axis::render()
{
    m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    GObject::render();
}

void Axis::transformWorld()
{
    static D3DXMATRIX matWorld;
    D3DXMatrixIdentity(&matWorld);
    m_d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}
