#include "MeshWrapper.h"


MeshWrapper::MeshWrapper(LPCWSTR filename, LPDIRECT3DDEVICE9 d3ddev)
{
    LPD3DXBUFFER pD3DXMtrlBuffer;
    m_pd3dDevice = d3ddev;
    D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM,
        m_pd3dDevice, NULL,
        &pD3DXMtrlBuffer, NULL, &m_dwNumMaterials,
        &m_pMesh);
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

    m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

    for (DWORD i = 0; i < m_dwNumMaterials; i++)
    {
        // Copy the material
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

        m_pMeshTextures[i] = NULL;
        if (d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
        {
            // Create the texture
            D3DXCreateTextureFromFileA(m_pd3dDevice,
                d3dxMaterials[i].pTextureFilename,
                &m_pMeshTextures[i]);
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();
}


MeshWrapper::~MeshWrapper()
{
    delete[] m_pMeshMaterials;

    for (DWORD i = 0; i < m_dwNumMaterials; i++)
    {
        if (m_pMeshTextures[i])
            m_pMeshTextures[i]->Release();
    }
    delete[] m_pMeshTextures;
    m_pMesh->Release();
}

void MeshWrapper::draw()
{
    for (DWORD i = 0; i < m_dwNumMaterials; i++)
    {
        // Set the material and texture for this subset
        m_pd3dDevice->SetMaterial(&m_pMeshMaterials[i]);
        m_pd3dDevice->SetTexture(0, m_pMeshTextures[i]);

        // Draw the mesh subset
        m_pMesh->DrawSubset(i);
    }
}

void MeshWrapper::render()
{
    m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    GObject::render();
}