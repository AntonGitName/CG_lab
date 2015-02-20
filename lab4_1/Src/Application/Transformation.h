#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Transformation {
public:
    virtual void transform (D3DXMATRIX* worldMatrix)
    {
        D3DXMatrixIdentity(worldMatrix);
    }
};