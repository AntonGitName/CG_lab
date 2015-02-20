#include "ViewControl.h"

namespace {
    D3DXVECTOR3 nullVector(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 lookupDirection(0.0f, -1.0f, 0.0f);
    D3DXVECTOR3 tmpVector(0.0f, 0.0f, 0.0f);
    D3DXMATRIX tmpMatrix;

    float radius;
    float phi;
    float theta;
    float x;
    float y;
    float z;

    D3DXVECTOR3 Sphere2XYZ(const D3DXVECTOR3& coord)
    {
        radius = coord.x;
        theta = coord.y;
        phi = coord.z;
        return{ radius * sin(theta) * cos(phi), radius * cos(theta), radius * sin(theta) * sin(phi) };
    }

    D3DXVECTOR3 XYZ2Sphere(const D3DXVECTOR3& coord)
    {
        x = coord.x;
        y = coord.z;
        z = coord.y;
        radius = D3DXVec3Length(&coord);
        return{ radius, acos(z / radius), atan2(y, x) };
    }
}

ViewControl::ViewControl(LPDIRECT3DDEVICE9 d3ddev, float dist)
    : m_d3ddev(d3ddev)
{
    m_cameraPositionSphere = { dist, 2.7f * D3DX_PI / 4, 1.2f * D3DX_PI / 4 };
    m_cameraPositionXYZ = Sphere2XYZ(m_cameraPositionSphere);
    m_lookAtPosition = nullVector;
    m_ccmode = CC_MODE_SPHERE;
    D3DXMatrixLookAtLH(&m_matView,
        &m_cameraPositionXYZ,
        &nullVector,
        &lookupDirection);
}

void ViewControl::changeControlMode()
{
    switch (m_ccmode)
    {
    case CC_MODE_WASD:
        setControlMode(CC_MODE_SPHERE);
        break;
    case CC_MODE_SPHERE:
        setControlMode(CC_MODE_WASD);
        break;
    default:
        break;
    }
}

void ViewControl::setControlMode(CAMERA_CONTROL_MODE mode)
{
    switch (mode)
    {
    case CC_MODE_WASD:
        m_ccmode = CC_MODE_WASD;
        m_cameraPositionXYZ = Sphere2XYZ(m_cameraPositionSphere);
        m_lookAtPosition = nullVector;
        break;
    case CC_MODE_SPHERE:
        m_ccmode = CC_MODE_SPHERE;
        m_lookAtPosition = nullVector;
        D3DXMatrixInverse(&tmpMatrix, NULL, &m_matView);
        m_cameraPositionXYZ = { tmpMatrix._41, tmpMatrix._42, tmpMatrix._43 };
        m_cameraPositionSphere = XYZ2Sphere(m_cameraPositionXYZ);
        break;
    default:
        break;
    }
}

void ViewControl::updateZoom(float dr)
{
    switch (m_ccmode)
    {
    case CC_MODE_WASD:
        D3DXMatrixTranslation(&tmpMatrix, 0.0f, 0.0f, -dr);
        m_matView *= tmpMatrix;
        break;
    case CC_MODE_SPHERE:
        m_cameraPositionSphere.x += ((m_cameraPositionSphere.x - dr) > 0) ? -dr : 0;
        m_cameraPositionXYZ = Sphere2XYZ(m_cameraPositionSphere);
        D3DXMatrixLookAtLH(&m_matView,
            &m_cameraPositionXYZ,
            &nullVector,
            &lookupDirection);
        break;
    default:
        break;
    }
}

void ViewControl::updateMouse(float dx, float dy, float dr)
{
    switch (m_ccmode)
    {
    case CC_MODE_WASD:
        updateCenterXY(dx, dy);
        break;
    case CC_MODE_SPHERE:
        updateCameraXY(dx, dy);
        break;
    default:
        break;
    }
    updateZoom(dr);
}

void ViewControl::updateKeyboard(float dx, float dy, float dr)
{
    switch (m_ccmode)
    {
    case CC_MODE_WASD:
        updateCameraXY(dx, dy);
        break;
    case CC_MODE_SPHERE:
        updateCameraXY(dy, dx);
        break;
    default:
        break;
    }
    updateZoom(dr);
}

void ViewControl::updateCenterXY(float dx, float dy)
{
    switch (m_ccmode)
    {
    case CC_MODE_WASD:
        D3DXMatrixRotationX(&tmpMatrix, dy);
        m_matView *= tmpMatrix;
        D3DXMatrixRotationY(&tmpMatrix, dx);
        m_matView *= tmpMatrix;
        break;

    case CC_MODE_SPHERE:
        break;

    default:
        break;
    }
}

void ViewControl::updateCameraXY(float dx, float dy)
{
    switch (m_ccmode)
    {
    case CC_MODE_WASD:
        dx *= 50.0f;
        dy *= 50.0f;
        D3DXMatrixTranslation(&tmpMatrix, 0.0f, 0.0f, -dx);
        m_matView *= tmpMatrix;
        D3DXMatrixTranslation(&tmpMatrix, dy, 0.0f, 0.0f);
        m_matView *= tmpMatrix;
        break;

    case CC_MODE_SPHERE:
        m_cameraPositionSphere.z += dx;
        m_cameraPositionSphere.y += dy;
        if (m_cameraPositionSphere.y > D3DX_PI)
        {
            m_cameraPositionSphere.y = D3DX_PI / 1.0001f;
        }
        else if (m_cameraPositionSphere.y < 0)
        {
            m_cameraPositionSphere.y = 0.0001f;
        }
        
        m_cameraPositionXYZ = Sphere2XYZ(m_cameraPositionSphere);
        D3DXMatrixLookAtLH(&m_matView,
            &m_cameraPositionXYZ,
            &m_lookAtPosition,
            &lookupDirection);

        break;

    default:
        break;
    }
}

const D3DXMATRIX* ViewControl::getViewMatrix() const
{
    return &m_matView;
}

const D3DXMATRIX* ViewControl::getProjMatrix() const
{
    return &m_matProj;
}

void ViewControl::transformProj()
{
    D3DXMatrixPerspectiveFovLH(&m_matProj,
        D3DXToRadian(45),    // the horizontal field of view
        800.0f / 600.0f, // aspect ratio
        1.0f,   // the near view-plane
        2000.0f);    // the far view-plane
    m_d3ddev->SetTransform(D3DTS_PROJECTION, &m_matProj); // set the projection
}

void ViewControl::transformView()
{
    m_d3ddev->SetTransform(D3DTS_VIEW, &m_matView);
}

ViewControl::~ViewControl()
{
}
