#pragma once

#include "GObject.h"

#include <d3d9.h>
#include <d3dx9.h>

#include <string>

class MeshWrapper : public GObject
{
public:
    MeshWrapper(LPCWSTR filename, LPDIRECT3DDEVICE9 d3ddev);
    virtual ~MeshWrapper();

    virtual void draw();
    virtual void setup();

protected:
    
    virtual void transformWorld() {}
    
    LPD3DXMESH          m_pMesh = NULL; // Our mesh object in sysmem
    D3DMATERIAL9*       m_pMeshMaterials = NULL; // Materials for our mesh
    LPDIRECT3DTEXTURE9* m_pMeshTextures = NULL; // Textures for our mesh
    DWORD               m_dwNumMaterials = 0L;   // Number of mesh materials
};

