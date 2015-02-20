#pragma once
#include "TriangleObject.h"
class Ground :
    public TriangleObject
{
public:
    Ground(LPDIRECT3DDEVICE9 d3ddev);

    void setup();
private:
    void transformWorld();
};

