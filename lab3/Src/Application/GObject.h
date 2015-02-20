#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class GObject
{

public:
    
    GObject(LPDIRECT3DDEVICE9 pd3dDevice);
    virtual ~GObject() {}

    virtual void draw() = 0;
    virtual void setup();
    void render();

protected:
    virtual void transformWorld();

    LPDIRECT3DDEVICE9   m_d3ddev = NULL; // Our rendering device
};

