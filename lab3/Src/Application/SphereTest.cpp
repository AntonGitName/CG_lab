#include "SphereTest.h"

namespace
{
    D3DXVECTOR3 getSpherePoint(float u, float v, const D3DXVECTOR3& c, float r)
    {
        return{ c.x + r * cos(u) * sin(v), c.y + r * sin(u) * sin(v), c.z + r * cos(v) };
    }
    D3DXVECTOR3 getSphereNormal(const D3DXVECTOR3& c, const D3DXVECTOR3& p)
    {
        static D3DXVECTOR3 tmp;
        D3DXVec3Subtract(&tmp, &p, &c);
        D3DXVec3Normalize(&tmp, &tmp);
        return tmp;
    }

    NORMAL_TEXTURE_VERTEX getSphereTexturePoint(float u, float v, const D3DXVECTOR3& center, float radius)
    {
        static D3DXVECTOR3 point;
        static D3DXVECTOR3 normal;
        point = getSpherePoint(u * D3DX_PI * 2.0f, v * D3DX_PI, center, radius);
        normal = getSphereNormal(center, point);
        return{ point.x, point.y, point.z, normal, u, v };
    }

    std::uniform_real_distribution<float> dist(0, 2.0f * D3DX_PI);
    std::default_random_engine engine;
}

SphereTest::SphereTest(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float radius)
    : TextureObject(d3ddev)
{
    std::vector<NORMAL_TEXTURE_VERTEX> vertices;

    const int n = 50;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            NORMAL_TEXTURE_VERTEX v1 = getSphereTexturePoint((float)i / (float)n, (float)j / (float)n, center, radius);
            NORMAL_TEXTURE_VERTEX v2 = getSphereTexturePoint((float)i / (float)n, (float)(j + 1) / (float)n, center, radius);
            NORMAL_TEXTURE_VERTEX v3 = getSphereTexturePoint((float)(i + 1) / (float)n, (float)j / (float)n, center, radius);
            NORMAL_TEXTURE_VERTEX v4 = getSphereTexturePoint((float)(i + 1) / (float)n, (float)(j + 1) / (float)n, center, radius);

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            vertices.push_back(v3);
            vertices.push_back(v2);
            vertices.push_back(v4);
        }
    }

    D3DXCreateTextureFromFile(m_d3ddev, TEXTURE_FILENAME, &m_pTexture);

    setVertices(vertices);

    m_phi = dist(engine);
    m_speed = dist(engine) / 500 * (rand() & 1 ? 1 : -1);
}

void SphereTest::setup()
{
    TextureObject::setup();
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
}

void SphereTest::transformWorld()
{
    static D3DXMATRIX matWorld;

    D3DXMatrixRotationY(&matWorld, m_phi += m_speed);
    m_d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

SphereTest::~SphereTest()
{
}