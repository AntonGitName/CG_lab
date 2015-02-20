#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class ViewControl
{
public:
    ViewControl(LPDIRECT3DDEVICE9 d3ddev, float dist);
    ~ViewControl();

    enum CAMERA_CONTROL_MODE
    {
        CC_MODE_WASD,
        CC_MODE_SPHERE
    };

    void changeControlMode();
    void setControlMode(CAMERA_CONTROL_MODE mode);

    void updateMouse(float dx, float dy, float dr);
    void updateKeyboard(float dx, float dy, float dr);

    void transformView();
    void transformProj();

    const D3DXMATRIX* getViewMatrix() const;
    const D3DXMATRIX* getProjMatrix() const;

private:
    LPDIRECT3DDEVICE9       m_d3ddev;
    D3DXMATRIX              m_matView;
    D3DXMATRIX              m_matProj;
    CAMERA_CONTROL_MODE     m_ccmode;
    D3DXVECTOR3             m_cameraPositionXYZ;
    D3DXVECTOR3             m_cameraPositionSphere;
    D3DXVECTOR3             m_lookAtPosition;
    
    void updateZoom(float dr);
    void updateCameraXY(float dx, float dy);
    void updateCenterXY(float dx, float dy);
};

