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
const float s_rMouseWheel2Zoom = 0.1f;
const float s_rKbd2Rotate = 1.16f;
const float s_rKbd2Zoom = 1.16f;

D3DXMATRIX              identityMatrix;

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

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow)
    : cglApp(nW, nH, hInst, nCmdShow)
    , m_nPrevMouseX(-100)
    , m_nPrevMouseY(-100)
{
    for (int i = 0; i < MAX_KEYS; i++)
        m_keysPressed[i] = false;

    m_nClearColor = 0xFF111100;

    m_d3ddev = m_pD3D->getDevice();
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_d3ddev->SetRenderState(D3DRS_AMBIENT, 0x00202020);
    
    m_pViewControl.reset(new ViewControl(m_d3ddev));
    D3DXMatrixIdentity(&identityMatrix);

    transformProjection();

    for (int t = 1; t <= 4; ++t)
    {
        m_cars.push_back(std::shared_ptr<CarObject>(new CarObject(m_d3ddev, t)));
    }
    
    m_objects.push_back(std::shared_ptr<GObject>(new Axis(m_d3ddev, 100.0f)));
    m_objects.push_back(std::shared_ptr<GObject>(new Ground(m_d3ddev)));
    
    for (auto cars : m_cars)
    {
        m_objects.push_back(cars);
    }

    prepareMaterial();
    prepareLights();

    m_directLightOn = m_pointLightOn = true;
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
                             s_rMouseMove2Rotate*(yPos-m_nPrevMouseY), 0);
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
            case 0x31:                      // press 1
                m_directLightOn ^= true;
                break;
            case 0x32:                      // press 2
                m_pointLightOn ^= true;
                break;
            case 0x33:                      // press 3
                for (auto cars : m_cars)
                {
                    cars->switchLights();
                }
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
        dx -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_RIGHT])     
        dx += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_UP])     
        dy -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_DOWN])     
        dy += s_rKbd2Rotate * m_timer.getDelta();
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

void myApp::transformProjection()
{
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj,
        D3DXToRadian(45),    // the horizontal field of view
        800.0f / 600.0f, // aspect ratio
        1.0f,   // the near view-plane
        2000.0f);    // the far view-plane
    m_d3ddev->SetTransform(D3DTS_PROJECTION, &matProj); // set the projection
}

void myApp::renderInternal()
{
    m_d3ddev->SetFVF(CUSTOMFVF);

    m_pViewControl->transformView();
    drawLights();
    for (auto pObject : m_objects)
    {
        pObject->render();
    }
}

void myApp::prepareMaterial()
{
    D3DMATERIAL9 material;    // create the material struct
    ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white
    m_d3ddev->SetMaterial(&material);    // set the globably-used material to &material
}

void myApp::prepareLights()
{
    ZeroMemory(&m_pointLight, sizeof(m_pointLight));
    m_pointLight.Type = D3DLIGHT_POINT;                         // make the light type 'point light'
    m_pointLight.Diffuse = D3DXCOLOR(0.1f, 1.0f, 0.5f, 1.0f);   // 0xFFFF8000;//D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
    m_pointLight.Position = D3DXVECTOR3(0.0f, -30.0f, 0.0f);
    m_pointLight.Range = 50.0f;                                 // a range of 100
    m_pointLight.Attenuation0 = 0.0f;                           // no constant inverse attenuation
    m_pointLight.Attenuation1 = 0.125f;                         // only .125 inverse attenuation
    m_pointLight.Attenuation2 = 0.0f;                           // no square inverse attenuation

    ZeroMemory(&m_directLight, sizeof(m_directLight));          // clear out the light struct for use
    m_directLight.Type = D3DLIGHT_DIRECTIONAL;                  // make the light type 'directional light'
    m_directLight.Diffuse = D3DXCOLOR(0.1f, 0.7f, 0.1f, 1.0f);  // set the light's color
    m_directLight.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);
}

void myApp::drawLights()
{
    m_d3ddev->SetLight(0, &m_pointLight);
    m_d3ddev->LightEnable(0, m_pointLightOn);

    m_d3ddev->SetLight(1, &m_directLight);
    m_d3ddev->LightEnable(1, m_directLightOn);
}