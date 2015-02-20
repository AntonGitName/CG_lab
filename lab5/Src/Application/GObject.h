#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <memory>

class GObject;

typedef GObject *PGObject;
typedef std::shared_ptr<GObject> SPGObject;
typedef std::shared_ptr<D3DXMATRIX> SPMatrix;

class GObject
{

public:
    GObject(LPDIRECT3DDEVICE9 pd3dDevice);
    virtual ~GObject() {}

    virtual void render();
    const D3DXMATRIX* getTransformMatrix() const;
    virtual void draw() = 0;

protected:
    virtual void transformWorld();
    virtual void setup();

    D3DXMATRIX                      m_matWorld;
    LPDIRECT3DDEVICE9               m_d3ddev;
};
