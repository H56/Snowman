#include "stdafx.h"
#include "Camera.h"



Camera::Camera() {
    m_fMaxPitch = D3DXToRadian(89.0f);
    m_fMaxVelocity = 1.0f;
    m_bInvertY = false;
    m_bEnableYMovement = true;
    m_dvCameraPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_dvVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_dvLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    SetProjection(D3DX_PI / 3.0f, 1.3f, 0.1f, 1000.0f);

    UpdateViewMatrix();
}

Camera::Camera(int x, int y, int z) {
    m_fMaxPitch = D3DXToRadian(89.0f);
    m_fMaxVelocity = 1.0f;
    m_bInvertY = false;
    m_bEnableYMovement = true;
    m_dvCameraPosition = D3DXVECTOR3(x, y, z);
    m_dvVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_dvLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    SetProjection(D3DX_PI / 3.0f, 1.3f, 0.1f, 1000.0f);
}


Camera::~Camera() {
}

void Camera::SetProjection(float fFovy, float fAspect, float fNearViewPlane, float fFarViewPlane) {
    m_fFovy       = fFovy;
    m_fAspectRatio    = fAspect;
    m_fNearViewPlane = fNearViewPlane;
    m_fFarViewPlane  = fFarViewPlane;
    D3DXMatrixPerspectiveFovLH(&m_dmProjection, m_fFovy, m_fAspectRatio, m_fNearViewPlane, m_fFarViewPlane);
}

void Camera::SetTargetPosition(float x, float y, float z) {
    m_dvTargetPosition.x = x;
    m_dvTargetPosition.y = y;
    m_dvTargetPosition.z = z;

    D3DXVECTOR3 dvLook = m_dvTargetPosition - m_dvCameraPosition;
    D3DXVec3Normalize(&m_dvLook, &dvLook);
}

void Camera::SetCameraPosition(float x, float y, float z) {
    m_dvCameraPosition.x = x;
    m_dvCameraPosition.y = y;
    m_dvCameraPosition.z = z;
}

void Camera::UpdateViewMatrix() {
    if (D3DXVec3Length(&m_dvVelocity) > m_fMaxVelocity) {
        m_dvVelocity = m_fMaxVelocity * (*D3DXVec3Normalize(&m_dvVelocity, &m_dvVelocity));
    }
    m_dvCameraPosition += m_dvVelocity;
    m_dvVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_dvTargetPosition = m_dvCameraPosition + m_dvLook;

    D3DXVECTOR3 dvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&m_dmView, &m_dvCameraPosition, &m_dvTargetPosition, &dvUp);
    
    // right
    m_dvRight.x = m_dmView._11;
    m_dvRight.y = m_dmView._21;
    m_dvRight.z = m_dmView._31;
    
    // up
    m_dvUp.x = m_dmView._12;
    m_dvUp.y = m_dmView._22;
    m_dvUp.z = m_dmView._32;

    // look
    m_dvLook.x = m_dmView._13;
    m_dvLook.y = m_dmView._23;
    m_dvLook.z = m_dmView._33;

    float lengthXZ = sqrtf(m_dvLook.x * m_dvLook.x + m_dvLook.z * m_dvLook.z);
    m_fPitch = atan2f(m_dvLook.y, lengthXZ);
    m_fYaw   = atan2f(m_dvLook.x, m_dvLook.z);
}

void Camera::MoveAlongRightVector(float fUnits) {
    if (m_bEnableYMovement) {
        m_dvVelocity += m_dvRight * fUnits;
    } else {
        D3DXVECTOR3 move(m_dvRight.x, 0.0f, m_dvRight.z);
        D3DXVec3Normalize(&move, &move);
        move *= fUnits;
        m_dvVelocity += move;
    }
}
void Camera::MoveALongUpVector(float fUnits) {
    if (m_bEnableYMovement) {
        m_dvVelocity += m_dvUp * fUnits;
    } else {
        D3DXVECTOR3 move(m_dvUp.x, 0.0f, m_dvUp.z);
        D3DXVec3Normalize(&move, &move);
        move *= fUnits;
        m_dvVelocity += move;
    }
}
void Camera::MoveAlongLookVector(float fUnits) {
    if (m_bEnableYMovement) {
        m_dvVelocity += m_dvLook * fUnits;
    } else {
        D3DXVECTOR3 move(m_dvLook.x, 0.0f, m_dvLook.z);
        D3DXVec3Normalize(&move, &move);
        move *= fUnits;
        m_dvVelocity += move;
    }
}

void Camera::MoveRight(float fUnits) {
    D3DXVECTOR3 move = m_dvRight;
    move.y = 0.0f;
    D3DXVec3Normalize(&move, &move);
    m_dvVelocity += move * fUnits;
}
void Camera::MoveForward(float fUnits) {
    D3DXVECTOR3 move = m_dvLook;
    move.y = 0.0f;
    D3DXVec3Normalize(&move, &move);
    m_dvVelocity += move * fUnits;
}
void Camera::MoveUp(float fUnits) {
    m_dvVelocity.y += fUnits;
}

void Camera::Yaw(float radians) {
    if (radians == 0.0f) {
        return;
    }
    D3DXMATRIX rotation;
    D3DXMatrixRotationAxis(&rotation, &m_dvUp, radians);
    D3DXVec3TransformNormal(&m_dvRight, &m_dvRight, &rotation);
    D3DXVec3TransformNormal(&m_dvLook, &m_dvLook, &rotation);
}
void Camera::Pitch(float radians) {
    if (radians == 0.0f) {
        return;
    }
    radians = (m_bInvertY) ? -radians : radians;
    m_fPitch -= radians;
    if (m_fPitch > m_fMaxPitch) {
        radians += m_fMaxPitch;
    } else if (m_fPitch < -m_fMaxPitch) {
        radians -= m_fMaxPitch;
    }

    D3DXMATRIX rotation;
    D3DXMatrixRotationAxis(&rotation, &m_dvRight, radians);
    D3DXVec3TransformNormal(&m_dvUp, &m_dvUp, &rotation);
    D3DXVec3TransformNormal(&m_dvLook, &m_dvLook, &rotation);
}
void Camera::Roll(float radians) {
    if (radians == 0.0f) {
        return;
    }
    D3DXMATRIX rotation;
    D3DXMatrixRotationAxis(&rotation, &m_dvLook, radians);
    D3DXVec3TransformNormal(&m_dvUp, &m_dvUp, &rotation);
    D3DXVec3TransformNormal(&m_dvRight, &m_dvRight, &rotation);
}

void Camera::SpinY(float fSpinY) {
    D3DXVECTOR3 move = m_dvTargetPosition - m_dvCameraPosition;
    move.y = 0.0f;
    float length = D3DXVec3Length(&move);
    float angle = atan2(move.z, move.x);
    move.x -= length * cosf(angle + fSpinY);
    move.z -= length * sinf(angle + fSpinY);
    m_dvTargetPosition += move;
}

void Camera::Spin(float fSpinForward, float fSpinRight, float fSpinUp) {
    MoveAlongLookVector(fSpinForward);
    MoveAlongRightVector(fSpinRight);
    MoveALongUpVector(fSpinUp);
}



void Camera::SetLength(float length) {
    D3DXVECTOR3 moveVec(m_dvCameraPosition - m_dvTargetPosition);
    D3DXVec3Normalize(&moveVec, &moveVec);
    m_dvCameraPosition = length * moveVec + m_dvTargetPosition;
}

D3DXVECTOR3 Camera::GetMoveVector(float fRight, float fForward) {
    D3DXVECTOR3 move, direction = m_dvRight;
    D3DXVec3Normalize(&direction, &direction);
    move = fRight * direction;
    D3DXVec3Normalize(&direction, &m_dvLook);
    move += fForward * direction;
    return move;
}

D3DXVECTOR3 Camera::GetMoveVector() {
    D3DXVECTOR3 ret = m_dvVelocity;
    if (D3DXVec3Length(&ret) > m_fMaxVelocity) {
        ret = m_fMaxVelocity * (*D3DXVec3Normalize(&ret, &ret));
    }
    ret.y = 0.0f;
    return ret;
}
