#pragma once
#include "MeshWrapper.h"

const LPCWSTR FILENAME = L"Resources\\car00.x";

class CarObject :
    public MeshWrapper
{
public:
    CarObject(LPDIRECT3DDEVICE9 d3ddev, int carNum);
    ~CarObject();

    virtual void draw();
    void switchLights();

private:
    
    void transformWorld();
    void drawLights();

    int                         m_carNum;
    float                       m_phi;
    bool                        m_lightsOn;
    D3DXVECTOR3                 m_carCenter;
    D3DLIGHT9                   m_headLamp1;
    D3DLIGHT9                   m_headLamp2;
    
};

