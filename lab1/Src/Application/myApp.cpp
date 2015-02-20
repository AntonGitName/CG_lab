/**
  @file     myApp.cpp
  @brief    User's application class
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
  */

// *******************************************************************
// includes

#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <zmouse.h>

#include "Library/cglD3D.h"
#include "myApp.h"

// *******************************************************************
// defines

namespace
{
    const float s_rMouseMove2Rotate = 0.001f;
    const float s_rMouseWheel2Zoom = 0.001f;
    const float s_rKbd2Rotate = 1.16f;
    const float s_rKbd2Zoom = 1.16f;

    void setXBetweenY(float& x, const float y)
    {
        x = (x < 0) ? 0 : x;
        x -= ((int)(x / y) * y);
    }

    void setXBetweenAB(float& x, const float a, const float b)
    {
        x -= a;
        setXBetweenY(x, b - a);
        x += a;
    }
}

// *******************************************************************
// Methods

CUSTOMVERTEX myApp::getTorusVertex(float t1, float t2)
{
    const float R = 100;
    const float r = 70;
    float x, y, z;
    x = (R + r * cos(t1)) * cos(t2) + m_torusCenter.x;
    y = (R + r * cos(t1)) * sin(t2) + m_torusCenter.y;
    z = r * sin(t1) + m_torusCenter.z;
    return{ x, y, z, (DWORD)((0xffffffff / (2 * D3DX_PI) * t1 - 0xffffffff / (2 * D3DX_PI) * t2)), };
}

bool myApp::processInput(unsigned int nMsg, int wParam, long lParam)
{
    // Handle input
    switch (nMsg)
    {
        // Process mouse move message
    case WM_MOUSEMOVE:
    {
        // Get mouse x & y (to the upper-left corner of the client area)
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        if ((wParam & MK_LBUTTON) != 0 && m_nPrevMouseX >= 0) // 
        {
            rotateMouse(s_rMouseMove2Rotate*(xPos - m_nPrevMouseX),
                s_rMouseMove2Rotate*(yPos - m_nPrevMouseY));
        }

        m_nPrevMouseX = xPos;
        m_nPrevMouseY = yPos;
        break;
    }

        // Process mouse wheel message
    case WM_MOUSEWHEEL:
    {
        int zDelta = (int)((signed short)(HIWORD(wParam)));
        zoom(zDelta * s_rMouseWheel2Zoom);
        break;
    }

    case WM_KEYDOWN:
    {
        // Save flags for continuous controls
        if (wParam < MAX_KEYS)
            m_keysPressed[wParam] = true;
        break;
    }

    case WM_KEYUP:
    {
        // Save flags for continuous controls
        if (wParam < MAX_KEYS)
            m_keysPressed[wParam] = false;
        // Process switch controls
        switch (wParam)
        {
        case '1': /* do something */ break;
        case '2': /* do something */ break;
        case '3': /* do something */ break;
        }
        break;
    }
    }

    return cglApp::processInput(nMsg, wParam, lParam);
}

void myApp::rotateMouse(float dx, float dy)
{
    xRotateCamera += dx;
    yRotateCamera += dy;

    if (yRotateCamera > D3DX_PI / 2)
    {
        yRotateCamera = D3DX_PI / 2.0001f;
    }
    else if (yRotateCamera < -D3DX_PI / 2)
    {
        yRotateCamera = -D3DX_PI / 2.0001f;
    }
}

void myApp::rotateKeyboard(float dx, float dy)
{
    xRotateObject += dx;
    yRotateObject += dy;
}

void myApp::zoom(float dr)
{
    zoomFactor += dr;
    if (zoomFactor < 0)
    {
        zoomFactor -= dr;
    }
}

void myApp::transformView()
{
    float phi = xRotateCamera;
    float theta = yRotateCamera;
    
    float radius = 1000.0f * zoomFactor;
    
    float x = radius * cos(theta) * cos(phi);
    float z = radius * cos(theta) * sin(phi);
    float y = radius * sin(theta);

    D3DXMatrixLookAtLH(&matView,
        &D3DXVECTOR3(x, y, z),
        &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
    m_d3ddev->SetTransform(D3DTS_VIEW, &matView);
}

void myApp::transformProjection()
{
    m_d3ddev->SetTransform(D3DTS_PROJECTION, &matProj); // set the projection
}

void myApp::transformWorld()
{
    // tell Direct3D about our matrix
    D3DXMATRIX tmpMatrix;
    D3DXMatrixTranslation(&matWorld, -m_torusCenter.x, -m_torusCenter.y, -m_torusCenter.z);
    D3DXMatrixRotationX(&tmpMatrix, xRotateObject);
    matWorld *= tmpMatrix;
    D3DXMatrixRotationY(&tmpMatrix, yRotateObject);
    matWorld *= tmpMatrix;
    D3DXMatrixTranslation(&tmpMatrix, m_torusCenter.x, m_torusCenter.y, m_torusCenter.z);
    matWorld *= tmpMatrix;
    m_d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

void myApp::transformWorldIdentity()
{
    // tell Direct3D about our matrix
    D3DXMatrixIdentity(&matWorld);
    m_d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

void myApp::renderInternal()
{
    // select which vertex format we are using
    m_d3ddev->SetFVF(CUSTOMFVF);

    // select the vertex buffer to display
    m_d3ddev->SetStreamSource(0, m_vbuffer, 0, sizeof(CUSTOMVERTEX));

    transformWorldIdentity();
    transformView();
    transformProjection();

    // copy the vertex buffer to the back buffer (lines)
    m_d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, m_trianNum , m_lines);

    transformWorld();

    // copy the vertex buffer to the back buffer (torus)
    m_d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_trianNum / 3);

}

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow)
    : cglApp(nW, nH, hInst, nCmdShow)
    , m_nPrevMouseX(-100)
    , m_nPrevMouseY(-100)
{
    for (int i = 0; i < MAX_KEYS; i++)
    {
        m_keysPressed[i] = false;
    }

    init_graphics();

    m_nClearColor = 0xFF000000;
}

void myApp::init_graphics() {

    m_d3ddev = m_pD3D->getDevice();
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    createTorus();
    
    // prepare projection matrix
    D3DXMatrixPerspectiveFovLH(&matProj,
        D3DXToRadian(45),    // the horizontal field of view
        800.0f / 600.0f, // aspect ratio
        1.0f,   // the near view-plane
        2000.0f);    // the far view-plane
}

void myApp::update()
{
    // Call predecessor update
    cglApp::update();

    // Process keyboard
    float dx = 0.0f;
    float dy = 0.0f;
    float dr = 0.0f;
    if (m_keysPressed[VK_LEFT])
        dx -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_RIGHT])
        dx += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_UP])
        dy -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_DOWN])
        dy += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_SUBTRACT])
        dr -= s_rKbd2Zoom * m_timer.getDelta();
    if (m_keysPressed[VK_ADD])
        dr += s_rKbd2Zoom * m_timer.getDelta();

    rotateKeyboard(dx, dy);
    zoom(dr);
}



void myApp::createTorus() {
    
    CUSTOMVERTEX vertices[m_trianNum + m_lines * 3];
    int k = 0;
    float k1 = 2 * D3DX_PI / n;
    float k2 = 2 * D3DX_PI / m;
    float t11, t12, t21, t22;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            t11 = i * k1;
            t12 = (i + 1) * k1;
            t21 = j * k2;
            t22 = (j + 1) * k2;
            vertices[k++] = getTorusVertex(t11, t21);
            vertices[k++] = getTorusVertex(t12, t21);
            vertices[k++] = getTorusVertex(t11, t22);
            vertices[k++] = getTorusVertex(t12, t21);
            vertices[k++] = getTorusVertex(t11, t22);
            vertices[k++] = getTorusVertex(t12, t22);
        }
    }
    
    // add Lines
    vertices[k++] = { 0.f, 0.f, -500.f, 0xFFFF0000};
    vertices[k++] = { 0.f, 0.f, -500.f, 0xFFFF0000 };
    vertices[k++] = { 0.f, 0.f, 500.f, 0xFFFF0000 };

    vertices[k++] = { 0.f, -500.f, 0.f, 0xFF00FF00 };
    vertices[k++] = { 0.f, -500.f, 0.f, 0xFF00FF00 };
    vertices[k++] = { 0.f, 500.f, 0.f, 0xFF00FF00 };

    vertices[k++] = { -500.f, 0.f, 0.f, 0xFF0000FF };
    vertices[k++] = { -500.f, 0.f, 0.f, 0xFF0000FF };
    vertices[k++] = { 500.f, 0.f, 0.f, 0xFF0000FF };

    // create a vertex buffer interface called v_buffer
    m_d3ddev->CreateVertexBuffer(sizeof(vertices),
        0,
        CUSTOMFVF,
        D3DPOOL_DEFAULT,
        &m_vbuffer,
        NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    m_vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    m_vbuffer->Unlock();
}

myApp::~myApp()
{
    m_vbuffer->Release();
}