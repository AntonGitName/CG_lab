#pragma once
#include "TriangleObject.h"

class Axis :
    public TriangleObject
{
public:
    Axis(LPDIRECT3DDEVICE9 d3ddev, float length);
    
    void setup();

protected:
    virtual void transformWorld();
};

