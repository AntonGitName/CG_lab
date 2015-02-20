#include "GObject.h"

void GObject::render()
{
    transformWorld();
    draw();
}
