#include "SphereObject.h"

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
}

SphereObject::SphereObject(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float radius)
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

    const UINT width = 256;
    const UINT height = 256;
    IDirect3DTexture9* pTexture = NULL;

    D3DXCreateTexture(m_d3ddev, width, height, NUMBER_OF_MIPMAPS, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture);
    D3DXCreateTexture(m_d3ddev, width, height, NUMBER_OF_MIPMAPS, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTexture);

    wchar_t filename[80];
    for (int i = 0; i < NUMBER_OF_MIPMAPS; ++i)
    {
        IDirect3DSurface9 *pSurf;
        pTexture->GetSurfaceLevel(i, &pSurf);
        swprintf(filename, 80, MIPMAP_FILENAME_FMT, i + 1);
        D3DXLoadSurfaceFromFile(pSurf, NULL, NULL, filename, NULL, D3DX_DEFAULT, 0, NULL);
        pSurf->Release();
    }

    m_d3ddev->UpdateTexture(pTexture, m_pTexture);
    pTexture->Release();

    setVertices(vertices);
}

void SphereObject::transformWorld()
{
    static float phi = 0;
    static D3DXMATRIX matWorld;

    D3DXMatrixRotationY(&matWorld, phi += 0.01f);
    m_d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}




SphereObject::~SphereObject()
{
}