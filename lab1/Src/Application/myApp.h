#ifndef __MYLAPP_H__632619789336566350
#define __MYLAPP_H__632619789336566350

/**
  @file     cglapp.h
  @brief    Class cglApp definition
  @date     Created on 10/09/2005
  @project  D3DBase
  @author   Bvs
  */

// *******************************************************************
// includes


#include <d3d9.h>
#include <d3dx9.h>
#include "Library/cglApp.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// *******************************************************************
// defines & constants

struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// *******************************************************************
// classes 

// Application class
class myApp : public cglApp
{
public:
    // Constructor
    myApp(int nW, int nH, void* hInst, int nCmdShow);

    // Destructor
    virtual ~myApp();

    // This function performs input processing. Returns true if input is handled
    virtual bool processInput(unsigned int nMsg, int wParam, long lParam);

    virtual void renderInternal();

protected:
    virtual void update();

private:
    enum
    {
        MAX_KEYS = 128
    };

    int   m_nPrevMouseX;
    int   m_nPrevMouseY;
    bool  m_keysPressed[MAX_KEYS];

    void rotateMouse(float dx, float dy);
    void rotateKeyboard(float dx, float dy);
    void zoom(float dr);

    void init_graphics();

    void createTorus();
    
    void transformView();
    void transformWorld();
    void transformWorldIdentity();
    void transformProjection();

    CUSTOMVERTEX getTorusVertex(float t1, float t2);

    static const int n = 100;
    static const int m = 100;
    static const int m_trianNum = n * m * 6;
    static const int m_lines = 3;

    LPDIRECT3DDEVICE9 m_d3ddev;    // the pointer to the device class
    LPDIRECT3DVERTEXBUFFER9 m_vbuffer = NULL;

    D3DXMATRIX matWorld;
    D3DXMATRIX matView;
    D3DXMATRIX matProj;

    float zoomFactor = 1;
    float xRotateCamera = D3DX_PI / 4.0f;
    float yRotateCamera = D3DX_PI / 4.0f;
    float xRotateObject = D3DX_PI / 4.0f;
    float yRotateObject = D3DX_PI / 4.0f;

    D3DXVECTOR3 m_torusCenter = {47.0f, -37.0f, 59.0f};

};


// *******************************************************************
// functions 

#endif //__CGLAPP_H__632619789336566350
