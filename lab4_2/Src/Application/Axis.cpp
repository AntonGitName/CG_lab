#include "Axis.h"


Axis::Axis(LPDIRECT3DDEVICE9 d3ddev, float length)
    : TriangleObject(d3ddev)
{
    vertices vertices;
    const D3DXCOLOR GRAY = 0xFF666666;
    vertices.push_back({ { 0.f, 0.f, -length }, GRAY });
    vertices.push_back({ { 0.f, 0.f, -length }, GRAY });
    vertices.push_back({ { 0.f, 0.f, length }, GRAY });

    vertices.push_back({ { 0.f, -length, 0.f }, GRAY });
    vertices.push_back({ { 0.f, -length, 0.f }, GRAY });
    vertices.push_back({ { 0.f, length, 0.f }, GRAY });

    vertices.push_back({ { -length, 0.f, 0.f }, GRAY });
    vertices.push_back({ { -length, 0.f, 0.f }, GRAY });
    vertices.push_back({ { length, 0.f, 0.f }, GRAY });

    setVertices(vertices);
}

void Axis::setup()
{
    TriangleObject::setup();
    m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
}
