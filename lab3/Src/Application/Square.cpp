#include "Square.h"

Square::Square(LPDIRECT3DDEVICE9 d3ddev, const D3DXVECTOR3& vec, float size)
    : TextureObject(d3ddev)
{
    std::vector<NORMAL_TEXTURE_VERTEX> vertices;
    D3DXVECTOR3 normal = vec;
    D3DXVec3Normalize(&normal, &normal);
    const int n = 50;
    const float min = -size / 2;
    const float max = +size / 2;
    const float step = size / n;

    D3DXVECTOR3 e1(1.0f, -normal.x / normal.y, 0.0f);
    D3DXVECTOR3 e2(1.0f, normal.y / normal.x, -(normal.x * normal.x + normal.y * normal.y) / (normal.x * normal.z));

    D3DXVec3Normalize(&e1, &e1);
    D3DXVec3Normalize(&e2, &e2);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            D3DXVECTOR3 u1 = e1 * (float)(i - n / 2) * step + e2 * (float)(j - n / 2) * step + vec;
            D3DXVECTOR3 u2 = e1 * (float)(i + 1 - n / 2) * step + e2 * (float)(j - n / 2) * step + vec;
            D3DXVECTOR3 u3 = e1 * (float)(i - n / 2) * step + e2 * (float)(j + 1 - n / 2) * step + vec;
            D3DXVECTOR3 u4 = e1 * (float)(i + 1 - n / 2) * step + e2 * (float)(j + 1 - n / 2) * step + vec;
            
            NORMAL_TEXTURE_VERTEX v1 = { u1.x, u1.y, u1.z, normal, (float)i / n, (float)j / n };
            NORMAL_TEXTURE_VERTEX v2 = { u2.x, u2.y, u2.z, normal, (float)(i + 1) / n, (float)j / n };
            NORMAL_TEXTURE_VERTEX v3 = { u3.x, u3.y, u3.z, normal, (float)i / n, (float)(j + 1) / n };
            NORMAL_TEXTURE_VERTEX v4 = { u4.x, u4.y, u4.z, normal, (float)(i + 1) / n, (float)(j + 1) / n };

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