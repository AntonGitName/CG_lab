/**
@file         myApp.cpp
@brief        User's application class
@date         Created on 10/09/2005
@project    D3DBase
@author     Bvs
*/

#include "myApp.h"

// *******************************************************************
// defines

namespace
{

    const float s_rMouseMove2Rotate = 0.001f;
    const float s_rMouseWheel2Zoom = 0.03f;
    const float s_rKbd2Rotate = 1.0f;
    const float s_rKbd2Zoom = 30.0f;

    int s_mipmap[3] = { D3DTEXF_POINT, D3DTEXF_LINEAR, D3DTEXF_NONE };
    int s_minmag[2] = { D3DTEXF_POINT, D3DTEXF_LINEAR };
}

// *******************************************************************
// Methods

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow)
    : cglApp(nW, nH, hInst, nCmdShow)
    , m_nPrevMouseX(-100)
    , m_nPrevMouseY(-100)
    , m_mipmapIndex(1)
    , m_minIndex(1)
    , m_magIndex(1)
{
    for (int i = 0; i < MAX_KEYS; i++)
        m_keysPressed[i] = false;

    m_nClearColor = 0xFF111100;

    m_d3ddev = m_pD3D->getDevice();

    m_pViewControl = std::make_shared<ViewControl>(m_d3ddev, 50.f);

    m_objects.push_back(std::make_shared<Axis>(m_d3ddev, 50.0f));
    m_objects.push_back(std::make_shared<Ground>(m_d3ddev, D3DXVECTOR3(0.f, 0.2f, 0.f), 10.f));

    createSnake();

    m_d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, s_mipmap[m_mipmapIndex]);
    m_d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, s_minmag[m_minIndex]);
    m_d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, s_minmag[m_magIndex]);
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
            m_pViewControl->updateMouse(s_rMouseMove2Rotate*(xPos - m_nPrevMouseX),
                s_rMouseMove2Rotate*(yPos - m_nPrevMouseY), 0);
        }

        m_nPrevMouseX = xPos;
        m_nPrevMouseY = yPos;
        break;
    }

        // Process mouse wheel message
    case WM_MOUSEWHEEL:
    {
        int zDelta = (int)((signed short)(HIWORD(wParam)));
        m_pViewControl->updateMouse(0, 0, zDelta * s_rMouseWheel2Zoom);
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
        case VK_SPACE:
            m_pViewControl->changeControlMode();
            break;
        case 'M':
            m_mipmapIndex = (m_mipmapIndex + 1) % 3;
            m_pD3D->getDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, s_mipmap[m_mipmapIndex]);
            break;
        case 'F':
            m_minIndex = (m_minIndex + 1) % 2;
            m_pD3D->getDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, s_minmag[m_minIndex]);
            break;
        case 'G':
            m_magIndex = (m_magIndex + 1) % 2;
            m_pD3D->getDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, s_minmag[m_magIndex]);
            break;
        }
        break;
    }
    }

    return cglApp::processInput(nMsg, wParam, lParam);
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
        dy += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_RIGHT])
        dy -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_UP])
        dx += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_DOWN])
        dx -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[0x57])                        // W
        dx += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[0x53])                        // S
        dx -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[0x41])                        // A
        dy += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[0x44])                        // D
        dy -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_SUBTRACT])
        dr -= s_rKbd2Zoom * m_timer.getDelta();
    if (m_keysPressed[VK_ADD])
        dr += s_rKbd2Zoom * m_timer.getDelta();

    m_pViewControl->updateKeyboard(dx, dy, dr);
}



void myApp::renderInternal()
{
    m_pViewControl->transformProj();
    m_pViewControl->transformView();
    
    m_snake->setViewProj(*m_pViewControl->getViewMatrix() * *m_pViewControl->getProjMatrix());

    for (auto pObject : m_objects)
    {
        pObject->render();
    }
    
}

namespace
{
    DWORD WHITE     = 0xFFFFFFFF;
    DWORD BLACK     = 0xFF000000;
    DWORD YELLOW    = 0xFFFFFF00;
    DWORD RED       = 0xFFFF0000;
    DWORD BLUE      = 0xFF0000FF;
    DWORD GREEN     = 0xFF00FF00;
    DWORD PINK      = 0xFFFF00FF;
    DWORD ORANGE    = 0xFFFF7777;
    
    const int COLORS_COUNT = 8;
    DWORD COLORS[] = { WHITE, GREEN, BLACK, YELLOW, RED, BLUE, PINK, ORANGE };
}

namespace {
    float snakeRadius1(float r, size_t i, size_t n)
    {
        const float x = (float)i / (float) n;
        return 4.0f * r * x * (x - 1);
    }

    float snakeRadius2(float r, size_t i, size_t n)
    {
        const float x = (float)(n - i) / (float)n;
        return r * x * x * exp(-x) * (1.0f - exp(30.0f * (x - 1.0f)));
    }
}

void myApp::createSnake()
{
    const int n = 32;
    const float aTranslate = 0.1f;
    const float fTranslate = 5.5f;
    const float radius = 0.5f;
    const float length = 0.2f;
    const float angleShift = - D3DX_PI / 6;

    std::vector<SPTranform> tmp;
    tmp.push_back(std::make_shared<TranslateSinusZMove>(aTranslate, fTranslate, 0.0f));
    tmp.push_back(std::make_shared<CircleMove>(6.0f, 0.25f));
    SnakeBuilder builder;
    SPTranform headMove(new CompositeTransformation(tmp));

    builder.addBone(snakeRadius2(radius, 0, n), snakeRadius2(radius, 1, n), length, WHITE, headMove);
    for (int i = 1; i < n; ++i)
    {
        std::vector<SPTranform> tmp;
        tmp.push_back(std::make_shared<RotateY>(0.018f));
        tmp.push_back(std::make_shared<TranslateSinusZMove>(aTranslate, fTranslate, i * angleShift));
        builder.addBone(snakeRadius2(radius, i, n), snakeRadius2(radius, i + 1, n), length, COLORS[i % 2], std::make_shared<CompositeTransformation>(tmp));
    }

    m_objects.push_back(m_snake = builder.build(m_d3ddev));
}