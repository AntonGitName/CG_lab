#include <d3d9.h>
#include <d3dx9.h>

#include "GObject.h"

#include <vector>

class Transformation {
public:
    virtual void transform(D3DXMATRIX *matrix, D3DXVECTOR3 center) = 0;
};

typedef std::shared_ptr<Transformation> SPTranform;

namespace {
    const float dt = 0.01f;
}

class CompositeTransformation : public Transformation
{
public:
    CompositeTransformation(std::vector<SPTranform> transformations)
        : m_transformations(transformations)
    {}

    virtual void transform(D3DXMATRIX *matrix, D3DXVECTOR3 center)
    {
        if (m_transformations.empty())
        {
            return;
        }
        D3DXMATRIX tmp;
        m_transformations[0]->transform(matrix, center);
        for (size_t i = 1; i < m_transformations.size(); ++i)
        {
            m_transformations[i]->transform(&tmp, center);
            *matrix *= tmp;
        }
    }
private:
    std::vector<SPTranform> m_transformations;
};

class CustomMove : public Transformation {
public:
    CustomMove(float a, float f)
        : ampl(a)
        , freq(f)
        , t(0.0f)
    {}

    virtual void transform(D3DXMATRIX *matrix, D3DXVECTOR3 center)
    {
        static D3DXMATRIX tmpM;
        D3DXMatrixTranslation(matrix, -center.x, -center.y, -center.z);
        D3DXMatrixTranslation(&tmpM, 0.0f, 0.0f, ampl * cos(t * freq));
        *matrix *= tmpM;
        D3DXMatrixTranslation(&tmpM, center.x, center.y, center.z);
        t += dt;
        *matrix *= tmpM;
    }
private:
    const float ampl;
    const float freq;
    float t;
};

class RotateY : public Transformation {
public:
    RotateY(float a)
        : angle(a)
    {}

    virtual void transform(D3DXMATRIX *matrix, D3DXVECTOR3 center)
    {
        static D3DXMATRIX tmpM;
        D3DXMatrixTranslation(matrix, -center.x, -center.y, -center.z);
        D3DXMatrixRotationY(&tmpM, angle);
        *matrix *= tmpM;
        D3DXMatrixTranslation(&tmpM, center.x, center.y, center.z);
        *matrix *= tmpM;
    }
private:
    const float angle;
};

class SinusMove : public Transformation {
public:
    SinusMove(float a, float f, float s)
        : ampl(a)
        , freq(f)
        , shft(s)
        , t(0.0f)
    {}

    virtual void transform(D3DXMATRIX *matrix, D3DXVECTOR3 center)
    {
        static D3DXMATRIX tmpM;
        D3DXMatrixTranslation(matrix, -center.x, -center.y, -center.z);
        D3DXMatrixRotationY(&tmpM, ampl * sin(t * freq + shft));
        *matrix *= tmpM;
        D3DXMatrixTranslation(&tmpM, center.x, center.y, center.z);
        t += dt;
        *matrix *= tmpM; 
    }
private:
    const float ampl;
    const float freq;
    const float shft;
    float t;
};

class TranslateSinusZMove : public Transformation {
public:
    TranslateSinusZMove(float a, float f, float f0)
        : A(a)
        , F(f)
        , F0(f0)
        , t(0.0f)
    {}

    virtual void transform(D3DXMATRIX *matrix, D3DXVECTOR3 center)
    {
        static D3DXMATRIX tmpM;
        D3DXMatrixTranslation(matrix, -center.x, -center.y, -center.z);
        D3DXMatrixTranslation(&tmpM, 0.0f, 0.0f, A * sin(F * t + F0));
        *matrix *= tmpM;
        D3DXMatrixTranslation(&tmpM, center.x, center.y, center.z);
        *matrix *= tmpM;
        t += dt;
    }
private:
    const float A;
    const float F;
    const float F0;
    float t;
};

class CircleMove : public Transformation {
public:
    CircleMove(float r, float s)
        : radius(r)
        , speed(s)
        , t(0.0f)
    {}

    virtual void transform(D3DXMATRIX *matrix, D3DXVECTOR3 center)
    {
        static D3DXMATRIX tmpM;
        const float x = fx(t);
        const float z = fz(t);
        const float dx = (fx(t + 2.0f * dt) - x);
        const float dz = (fz(t + 2.0f * dt) - z);

        D3DXMatrixTranslation(matrix, -center.x, -center.y, -center.z);
        D3DXMatrixRotationY(&tmpM, -atan2(dz, dx) + D3DX_PI);
        *matrix *= tmpM;
        D3DXMatrixTranslation(&tmpM, x, 0.0f, z);
        *matrix *= tmpM;
        D3DXMatrixTranslation(&tmpM, center.x, center.y, center.z);
        *matrix *= tmpM;
        
        t += dt;
    }
private:
    const float radius;
    const float speed;
    float t;

    float fx(float t)
    {
        return radius * cos(speed * t);
    }
    float fz(float t)
    {
        return radius * sin(speed * t);
    }
};