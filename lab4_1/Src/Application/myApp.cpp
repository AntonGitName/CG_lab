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
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    m_d3ddev->SetRenderState(D3DRS_AMBIENT, 0xFF202020);

    m_pViewControl.reset(new ViewControl(m_d3ddev));

    m_objects.push_back(SPGObject(new Axis(m_d3ddev, 50.0f)));
    m_objects.push_back(SPGObject(new Ground(m_d3ddev)));

    createSkeleton();

    transformProjection();
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
    m_pViewControl->transformView();

    for (auto pObject : m_objects)
    {
        pObject->render();
    }
}

namespace
{

    D3DXCOLOR BLACK = 0xFF000000;
    D3DXCOLOR YELLOW = 0xFFFFFF00;
    D3DXCOLOR RED = 0xFFFF0000;
    D3DXCOLOR BLUE = 0xFF0000FF;
    D3DXCOLOR GREEN = 0xFF00FF00;
    D3DXCOLOR PINK = 0xFFFF00FF;
    D3DXCOLOR ORANGE = 0xFFFF7777;

}

void myApp::createSkeleton()
{
    const float ropeR = 0.05f;
    const float ropeH = 1.5f;
    const float bellR = 0.4f;
    const float bellH = 4.5f;
    const float angleA = 0.005f;
    const float angleB = 0.001f;

    D3DXVECTOR3 pos{ 0.f, -20.0f, 0.f };
    D3DXVECTOR3 posV = pos;
    posV.y -= ropeH / 2;
    D3DXMATRIX mat;
    D3DXMatrixRotationY(&mat, D3DX_PI / 2);

    srand((unsigned)time(NULL));

    SPGObject tmpObj(SPGObject(new Cylinder(m_d3ddev, pos, { 0.f, 0.f, 1.f }, ropeR, ropeH, BLACK)));
    tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, posV));
    SPSkeleton a(new Skeleton(tmpObj));

    posV.y = pos.y;
    posV.x = bellH / 2.f;
    pos.y += ropeH / 2.f + bellR;
    tmpObj.reset(new Cylinder(m_d3ddev, pos, { 0.f, 0.f, 0.f }, bellR, bellH, YELLOW));
    tmpObj->setTransformation(new RotationTransform(angleA, tmpObj->getCenter(), 0.f, pos));
    SPSkeleton b1(new Skeleton(tmpObj));

    tmpObj.reset(new Cylinder(m_d3ddev, pos, { 1.f, 0.f, 0.f }, bellR, bellH, YELLOW));
    SPSkeleton b2(new Skeleton(tmpObj));

    posV.y += ropeH + 2.f * bellR;
    pos = posV;
    pos.y -= ropeH / 2.f;

    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, ropeR, ropeH, BLACK));
    tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton d1(new Skeleton(tmpObj));

    D3DXVec3TransformCoord(&posV, &posV, &mat);
    D3DXVec3TransformCoord(&pos, &pos, &mat);
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, ropeR, ropeH, BLACK));
    tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton d2(new Skeleton(tmpObj));

    D3DXVec3TransformCoord(&posV, &posV, &mat);
    D3DXVec3TransformCoord(&pos, &pos, &mat);
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, ropeR, ropeH, BLACK));
    tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton d3(new Skeleton(tmpObj));

    D3DXVec3TransformCoord(&posV, &posV, &mat);
    D3DXVec3TransformCoord(&pos, &pos, &mat);
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, ropeR, ropeH, BLACK));
    tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton d4(new Skeleton(tmpObj));

    D3DXVec3TransformCoord(&posV, &posV, &mat);
    D3DXVec3TransformCoord(&pos, &pos, &mat);
    posV.y += (ropeH + bellH) / 2.f;
    pos.y += ropeH;
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, bellR, bellH, YELLOW));
    //tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton c1(new Skeleton(tmpObj));

    D3DXVec3TransformCoord(&posV, &posV, &mat);
    D3DXVec3TransformCoord(&pos, &pos, &mat);
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, bellR, bellH, BLUE));
    //tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton c2(new Skeleton(tmpObj));

    D3DXVec3TransformCoord(&posV, &posV, &mat);
    D3DXVec3TransformCoord(&pos, &pos, &mat);
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, bellR, bellH, PINK));
    //tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton c4(new Skeleton(tmpObj));

    D3DXVec3TransformCoord(&posV, &posV, &mat);
    D3DXVec3TransformCoord(&pos, &pos, &mat);
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, bellR, bellH, RED));
    //tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton c5(new Skeleton(tmpObj));

    posV.y += (ropeH + bellH) / 2.f;
    pos.y += bellH;
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, ropeR, ropeH, BLACK));
    tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton d5(new Skeleton(tmpObj));

    posV.y += (ropeH + bellH) / 2.f;
    pos.y += ropeH;
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, bellR, bellH, ORANGE));
    //tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton c6(new Skeleton(tmpObj));

    posV.y -= (ropeH + bellH) / 2.f;
    pos.y -= ropeH;
    D3DXVec3TransformCoord(&posV, &posV, &mat);
    D3DXVec3TransformCoord(&pos, &pos, &mat);
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, ropeR, ropeH, BLACK));
    tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton d6(new Skeleton(tmpObj));

    posV.y += (ropeH + bellH) / 2.f;
    pos.y += ropeH;
    tmpObj.reset(new Cylinder(m_d3ddev, posV, { 0.f, 0.f, 1.f }, bellR, bellH, GREEN));
    //tmpObj->setTransformation(new RotationTransform(0.0f, pos, angleB, pos));
    SPSkeleton c3(new Skeleton(tmpObj));

    a->addChild(b1);
    b1->addChild(b2);
    b1->addChild(d1);
    b1->addChild(d2);
    b1->addChild(d3);
    b1->addChild(d4);
    d1->addChild(c1);
    d2->addChild(c2);
    d3->addChild(c4);
    d4->addChild(c5);
    c1->addChild(d6);
    d6->addChild(c3);
    c5->addChild(d5);
    d5->addChild(c6);

    m_objects.push_back(a);
}