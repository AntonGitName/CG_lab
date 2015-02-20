#include "Ground.h"

Ground::Ground(LPDIRECT3DDEVICE9 d3ddev)
    : TriangleObject(d3ddev)
{
    vertices vertices;
    const int n = 50;
    const float min = -10.0f;
    const float max = +10.0f;
    const float step = (max - min) / n;
    D3DXVECTOR3 normal{0.0f, -1.0f, 0.0f};
    static const DWORD BLUE_AND_WHITE[] = { 0xFF000077, 0xFF770000 };

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            NORMALCOLORVERTEX v1 = { { min + i * step, 1.0f, min + j * step }, normal, BLUE_AND_WHITE[i + j & 1] };
            NORMALCOLORVERTEX v2 = { { min + i * step, 1.0f, min + (j + 1) * step }, normal, BLUE_AND_WHITE[i + j & 1] };
            NORMALCOLORVERTEX v3 = { { min + (i + 1) * step, 1.0f, min + j * step }, normal, BLUE_AND_WHITE[i + j & 1] };
            NORMALCOLORVERTEX v4 = { { min + (i + 1) * step, 1.0f, min + (j + 1) * step }, normal, BLUE_AND_WHITE[i + j & 1] };

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            vertices.push_back(v3);
            vertices.push_back(v2);
            vertices.push_back(v4);
        }
    }
    setVertices(vertices);
}