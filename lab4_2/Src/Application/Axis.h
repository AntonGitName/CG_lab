#pragma once
#include "TriangleObject.hpp"

class Axis :
    public TriangleObject<COLORVERTEX, COLORFVF>
{
public:
    Axis(LPDIRECT3DDEVICE9 d3ddev, float length);

protected:
	void setup();
};

