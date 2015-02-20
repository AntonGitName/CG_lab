#include "Ground.h"

Ground::Ground(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 center, float w)
    : TriangleObject(d3ddev)
{
    vertices vertices;
    const int n = 128;
    const float step = 2.0f * w / n;
    const float invN = 1.0f / (float)n;

    TEXTURE_VERTEX v[4];

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            v[0] = { center + D3DXVECTOR3(-w + (i + 0) * step, 0.0f, -w + (j + 0) * step), (i + 0) * invN, (j + 0) * invN };
            v[1] = { center + D3DXVECTOR3(-w + (i + 0) * step, 0.0f, -w + (j + 1) * step), (i + 0) * invN, (j + 1) * invN };
            v[2] = { center + D3DXVECTOR3(-w + (i + 1) * step, 0.0f, -w + (j + 0) * step), (i + 1) * invN, (j + 0) * invN };
            v[3] = { center + D3DXVECTOR3(-w + (i + 1) * step, 0.0f, -w + (j + 1) * step), (i + 1) * invN, (j + 1) * invN };

            addTriplet(vertices, v[0], v[1], v[2]);
            addTriplet(vertices, v[2], v[1], v[3]);
        }
    }
    setVertices(vertices);

    const UINT width = 512;
    const UINT height = 512;
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
}

void Ground::setup()
{
    TriangleObject::setup();
    m_d3ddev->SetTexture(0, m_pTexture);
}

Ground::~Ground()
{
    m_pTexture->Release();
}