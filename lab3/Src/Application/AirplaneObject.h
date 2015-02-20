#pragma once
#include "MeshWrapper.h"

const LPCWSTR FILENAME = L"Resources\\airplane00.x";

class AirplaneObject :
    public MeshWrapper
{
public:
    AirplaneObject(LPDIRECT3DDEVICE9 d3ddev, int carNum);
    ~AirplaneObject();

    virtual void draw();
    void switchLights();

private:
    
    void transformWorld();
    void drawLights();

    int                         m_carNum;
    float                       m_phi;
    bool                        m_lightsOn;
    D3DXVECTOR3                 m_objectCenter;
    D3DLIGHT9                   m_headLamp1;
    D3DLIGHT9                   m_headLamp2;
  

    const float                 m_outerRadius = 30.0f;
    const float                 m_innerRadius = 10.0f;
};

