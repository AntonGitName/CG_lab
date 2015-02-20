#pragma once
#include "GObject.h"
#include <vector>
#include "Transformation.h"
#include <iostream>

using std::vector;

class Snake : public GObject {
public:
    ~Snake();
    
    void setViewProj(D3DXMATRIX viewProjMatrix);
    void draw();
    void setup() {}

    struct SnakeBoneVertex {
        D3DXVECTOR3 pos;
        DWORD color;
        float u, v;
        float b1, b2;
        short i1, i2;
    };
private:
    
    friend class SnakeBuilder;

    void transformWorld();

    struct SnakeBone {
        SnakeBone(float radius1, float radius2, float height, DWORD color, D3DXVECTOR3 center, size_t i, size_t n, LPDIRECT3DDEVICE9 d3ddev);
        ~SnakeBone();
        void draw();

        D3DXVECTOR3             m_boneVector;
        float                   m_length;
        int                     m_triangleCount;
        LPDIRECT3DVERTEXBUFFER9 m_vbuffer = NULL;
        LPDIRECT3DDEVICE9       m_d3ddev;
    };
    typedef std::shared_ptr<SnakeBone> SPSnakeBone;

    Snake(LPDIRECT3DDEVICE9 d3ddev, vector<SPSnakeBone> bones, vector<D3DXMATRIX> matrices, vector<SPTranform> transformations);

    vector<SPSnakeBone>             m_bones;
    vector<D3DXMATRIX>              m_matrices;
    vector<SPTranform>              m_transformations;
    
    D3DXMATRIX                      m_viewProjMatrix;
    IDirect3DVertexDeclaration9    *m_vertexDeclaration;
    LPD3DXEFFECT                    m_effect;
    LPDIRECT3DTEXTURE9              m_pTexture;
    static const int                NUMBER_OF_MIPMAPS = 5;
    const LPCWSTR                   MIPMAP_FILENAME_FMT = L"Res\\snake_texture%d.jpg";
};

typedef std::shared_ptr<Snake> SPSnake;

class SnakeBuilder {
public:
    SnakeBuilder& addBone(float radius1, float radius2, float height, DWORD color, SPTranform transformation);
    SPSnake build(LPDIRECT3DDEVICE9 d3ddev);
private:
    struct arg_t {
        float radius1;
        float radius2;
        float height;
        DWORD color;
        SPTranform transformation;
    };

    vector<arg_t> m_bonesArgs;
};