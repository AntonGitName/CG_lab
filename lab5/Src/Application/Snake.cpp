#include "Snake.h"

Snake::Snake(LPDIRECT3DDEVICE9 d3ddev, vector<SPSnakeBone> bones, vector<D3DXMATRIX> matrices, vector<SPTranform> transformations)
    : GObject(d3ddev)
    , m_bones(bones)
    , m_matrices(matrices)
    , m_transformations(transformations)
    , m_viewProjMatrix()
{
    ID3DXBuffer *errors;
    HRESULT hr = D3DXCreateEffectFromFile(d3ddev, L"Res/skinning.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_effect, &errors);
    if (hr != D3D_OK || errors)
    {
        char *str = (char *)errors->GetBufferPointer();

        MessageBox(NULL, L"Cannot load effect", L"ERROR", MB_ICONERROR | MB_OK);
        PostQuitMessage(EXIT_FAILURE);
    }

    D3DVERTEXELEMENT9 decl[] = {
            { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
            { 0, 32, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
            D3DDECL_END()
    };
    m_d3ddev->CreateVertexDeclaration(decl, &m_vertexDeclaration);

    const UINT width = 960;
    const UINT height = 600;
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

void Snake::transformWorld()
{
    const size_t n = m_bones.size();
    D3DXVECTOR3 boneRotationCenter(0.0f, 0.0f, 0.0f);

    for (size_t i = 0; i < n; ++i)
    {
        m_transformations[i]->transform(&m_matrices[i], boneRotationCenter);

        if (i > 0)
        {
            m_matrices[i] *= m_matrices[i - 1];
        }

        boneRotationCenter += m_bones[i]->m_boneVector;
    }
}

void Snake::setViewProj(D3DXMATRIX viewProjMatrix)
{
    m_viewProjMatrix = viewProjMatrix;
}

void Snake::draw()
{
    m_d3ddev->SetVertexDeclaration(m_vertexDeclaration);
    m_effect->SetMatrix("u_view_projection_matrix", &m_viewProjMatrix);
    m_effect->SetMatrixArray("u_bones_matrix", m_matrices.data(), m_matrices.size());
    m_effect->SetTexture("ModelTexture", m_pTexture);
    m_effect->SetTechnique("skinning_technique");
    size_t num_passes = 0;
    m_effect->Begin(&num_passes, 0);
    for (size_t i = 0; i < num_passes; ++i)
    {
        m_effect->BeginPass(i);

        for (size_t j = 0; j < m_bones.size(); ++j)
        {
            m_bones[j]->draw();
        }

        m_effect->EndPass();
    }
    m_effect->End();
}

Snake::~Snake()
{
    m_vertexDeclaration->Release();
    m_effect->Release();
    m_pTexture->Release();
}

SnakeBuilder& SnakeBuilder::addBone(float radius1, float radius2, float height, DWORD color, SPTranform transformation)
{
    m_bonesArgs.push_back({ radius1, radius2, height, color, transformation });
    return *this;
}

SPSnake SnakeBuilder::build(LPDIRECT3DDEVICE9 d3ddev)
{
    size_t n = m_bonesArgs.size();
    std::vector<Snake::SPSnakeBone> bones;
    std::vector<D3DXMATRIX> matrices;
    std::vector<SPTranform> transformations;
    D3DXVECTOR3 boneRotationCenter(0.0f, 0.0f, 0.0f);
    for (size_t i = 0; i < n; ++i)
    {
        bones.push_back(std::make_shared<Snake::SnakeBone>(m_bonesArgs[i].radius1, m_bonesArgs[i].radius2, m_bonesArgs[i].height, m_bonesArgs[i].color, boneRotationCenter, i, n, d3ddev));
        transformations.push_back(m_bonesArgs[i].transformation);
        matrices.push_back(D3DXMATRIX());
        D3DXMatrixIdentity(&matrices.back());
        boneRotationCenter += bones.back()->m_boneVector;
    }
    m_bonesArgs.clear();
    Snake *snake = new Snake(d3ddev, bones, matrices, transformations);
    return std::shared_ptr<Snake>(snake);
}

namespace
{
    inline D3DXVECTOR3 getPoint(float u, float v, float r1, float r2)
    {
        const float r = r1 + u * (r2 - r1);
        return{ u, r * cos(v), r * sin(v) };
    }

    inline void setCoefs(Snake::SnakeBoneVertex &v, D3DXMATRIX *mat, float c, float k, size_t i, size_t n, D3DXCOLOR color, float r1, float r2)
    {
        v.pos = getPoint(c, k, r1, r2);
        v.v = k / (2.0f * D3DX_PI);
        v.u = c;
        v.color = color;
        v.b1 = v.b2 = 0.5f;
        v.i1 = v.i2 = i;
        if (c >= 0.5f)
        {
            v.b1 = 1.5f - c;
            v.b2 = 1 - v.b1;
            v.i1 = i;
            v.i2 = (i + 1 != n) ? i + 1 : i;
        }
        if (c < 0.5f)
        {
            v.b2 = 0.5f + c;
            v.b1 = 1 - v.b2;
            v.i1 = (i != 0) ? i - 1 : i;
            v.i2 = i;
        }
        D3DXVec3TransformCoord(&v.pos, &v.pos, mat);
    }

    void pushTriangle(vector<Snake::SnakeBoneVertex>  &vertices, const Snake::SnakeBoneVertex& v1, const Snake::SnakeBoneVertex& v2, const Snake::SnakeBoneVertex& v3)
    {
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
    }
}

Snake::SnakeBone::SnakeBone(float radius1, float radius2, float height, DWORD color, D3DXVECTOR3 center, size_t index, size_t num, LPDIRECT3DDEVICE9 d3ddev)
    : m_d3ddev(d3ddev)
    , m_boneVector(height, 0.0f, 0.0f)
{
    m_boneVector = { height, 0.0f, 0.0f };
    vector<Snake::SnakeBoneVertex> v;
    D3DXMATRIX tmp;
    D3DXMATRIX mat;
    const int n = 32;
    const float invi = 2.0f * D3DX_PI / (float)n;
    const float invj = 1.0f / (float)n;
    const bool r1NotNull = (radius1 != 0.0f);
    float r1;
    float r2;
    if (r1NotNull)
    {
        r2 = radius2 / radius1;
        r1 = 1.0f;
        D3DXMatrixScaling(&mat, height, radius1, radius1);
    }
    else
    {
        r1 = radius1 / radius2;
        r2 = 1.0f;
        D3DXMatrixScaling(&mat, height, radius2, radius2);
    }
    D3DXMatrixTranslation(&tmp, center.x, center.y, center.z);
    mat *= tmp;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            Snake::SnakeBoneVertex v1;
            setCoefs(v1, &mat, j * invj, i * invi, index, num, color, r1, r2);

            Snake::SnakeBoneVertex v2;
            setCoefs(v2, &mat, j * invj, (i + 1) * invi, index, num, color, r1, r2);

            Snake::SnakeBoneVertex v3;
            setCoefs(v3, &mat, (j + 1) * invj, i * invi, index, num, color, r1, r2);

            Snake::SnakeBoneVertex v4;
            setCoefs(v4, &mat, (j + 1) * invj, (i + 1) * invi, index, num, color, r1, r2);

            pushTriangle(v, v1, v2, v3);
            pushTriangle(v, v3, v2, v4);
        }
    }

    m_triangleCount = v.size();
    m_d3ddev->CreateVertexBuffer(sizeof(Snake::SnakeBoneVertex) * v.size(), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_vbuffer, NULL);
    VOID* pVoid;
    m_vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, v.data(), sizeof(Snake::SnakeBoneVertex) * v.size());
    m_vbuffer->Unlock();
}

Snake::SnakeBone::~SnakeBone()
{
    m_vbuffer->Release();
}

void Snake::SnakeBone::draw()
{
    m_d3ddev->SetStreamSource(0, m_vbuffer, 0, sizeof(Snake::SnakeBoneVertex));
    m_d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_triangleCount / 3);
}