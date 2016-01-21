#pragma once
class Camera {
public:
    Camera();
    Camera(int x, int y, int z);
    ~Camera();

    void UpdateViewMatrix();
    void MoveAlongRightVector(float fUnits);
    void MoveALongUpVector(float fUnits);
    void MoveAlongLookVector(float fUnits);
    void MoveRight(float fUnits);
    void MoveForward(float fUnits);
    void MoveUp(float fUnits);

    void Yaw(float radians);
    void Pitch(float radians);
    void Roll(float radians);
    void SpinY(float fSpinY);
    void Spin(float fSpinForward, float fSpinRight, float fSpinUp);

    void SetProjection(float fFovy, float fAspect, float fNearViewPlane, float fFarViewPlane);
    void SetTargetPosition(float x, float y, float z);
    void SetTargetPosition(D3DXVECTOR3 position) {
        SetTargetPosition(position.x, position.y, position.z);
    }
    void SetCameraPosition(float x, float y, float z);
    void SetCameraPosition(D3DXVECTOR3 position) {
        SetCameraPosition(position.x, position.y, position.z);
    }
    void SetFov(float fFovy) {
        SetProjection(fFovy, m_fAspectRatio, m_fNearViewPlane, m_fFarViewPlane);
    }
    void SetAspectRatio(float fAspect) {
        SetProjection(m_fFovy, fAspect, m_fNearViewPlane, m_fFarViewPlane);
    }
    void SetNearViewPlane(float fNearViewPlane) {
        SetProjection(m_fFovy, m_fAspectRatio, fNearViewPlane, m_fFarViewPlane);
    }
    void SetFarPlane(float fFarViewPlane) {
        SetProjection(m_fFovy, m_fAspectRatio, m_fNearViewPlane, fFarViewPlane);
    }
    void SetMaxVelocity(float x, float y, float z) {
        m_dvVelocity = D3DXVECTOR3(x, y, z);
    }
    void SetEnableYMovement(bool bEnable) {
        m_bEnableYMovement = bEnable;
    }
    void SetLength(float length);

    D3DXMATRIX* GetViewMatrix() {
        return &m_dmView;
    }
    D3DXMATRIX* GetProjectionMatrix() {
        return &m_dmProjection;
    }
    float GetCameraY() {
        return m_dvCameraPosition.y;
    }

    D3DXVECTOR3 GetMoveVector(float fRight, float fForward);

    D3DXVECTOR3 GetMoveVector();

    D3DXVECTOR3 GetCameraPosition() {
        return m_dvCameraPosition;
    }

    D3DXVECTOR3 GetTargetPosition() {
        return m_dvTargetPosition;
    }


    D3DXVECTOR3 GetLookVector() {
        return m_dvLook;
    }


private:
    LPDIRECT3DDEVICE9 m_pd3dDevice;

    D3DXMATRIX  m_dmView;
    D3DXMATRIX  m_dmProjection;
    D3DXVECTOR3 m_dvRight;
    D3DXVECTOR3 m_dvUp;
    D3DXVECTOR3 m_dvLook;
    D3DXVECTOR3 m_dvTargetPosition;
    D3DXVECTOR3 m_dvCameraPosition;
    D3DXVECTOR3 m_dvVelocity;

    float m_fYaw;
    float m_fPitch;
    float m_fMaxPitch;
    float m_fMaxVelocity;
    float m_fFovy; // field of view in y direction
    float m_fAspectRatio;
    float m_fNearViewPlane;
    float m_fFarViewPlane;


    bool m_bInvertY;
    bool m_bEnableYMovement;
};

