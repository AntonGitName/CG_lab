#pragma once
#include "TriangleObject.hpp"

class Ground :
    public TriangleObject<NORMALCOLORVERTEX, NORMALCOLORFVF>
{
public:
    Ground(LPDIRECT3DDEVICE9 d3ddev);
};

