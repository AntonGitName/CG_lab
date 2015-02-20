#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <memory>

#include "Transformation.h"

class GObject;
typedef GObject *PGObject;

typedef std::shared_ptr<Transformation> SHPTRTranform;
typedef std::shared_ptr<GObject> SPGObject;

class GObject
{

public:
    GObject(LPDIRECT3DDEVICE9 pd3dDevice);
    GObject(LPDIRECT3DDEVICE9 pd3dDevice, SHPTRTranform pTransfrom);
    virtual ~GObject() {}

    virtual void render(const D3DXMATRIX* preTransform = nullptr);
    const D3DXMATRIX* getTransformMatrix() const;
    void setTransformation(std::shared_ptr<Transformation> pTransfrom);
    void setTransformation(Transformation *pTransfrom);
    virtual D3DXVECTOR3 getCenter() const = 0;

protected:
    virtual void transformWorld(const D3DXMATRIX* preTransform = nullptr);
	virtual void draw() = 0;
	virtual void setup();

    D3DXMATRIX                      m_matWorld;
    D3DXMATRIX                      m_accumulatedMatWorld;
    SHPTRTranform                   m_pTransfrom;

    LPDIRECT3DDEVICE9               m_d3ddev; // Our rendering device
};

