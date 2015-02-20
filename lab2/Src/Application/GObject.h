#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class GObject
{

public:
    virtual ~GObject() {}

    virtual void draw() = 0;
    virtual void render();

protected:
    virtual void transformWorld() = 0;
};

