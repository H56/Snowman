#include "stdafx.h"
#include "RenderObj.h"



RenderObj::RenderObj(float x, float y, float z) {
    Reset();
    AbsoluteTranslate(x, y, z);
}


RenderObj::~RenderObj() {
    InvalidateDeviceObjects();
}

void RenderObj::Reset() {
    D3DXMatrixIdentity(&m_d3dmTranslate);
    D3DXMatrixIdentity(&m_d3dmRotate);
    D3DXMatrixIdentity(&m_d3dmScale);
    D3DXMatrixIdentity(&m_d3dmTransform);
    D3DXMatrixIdentity(&m_d3dmFace);

    //AbsoluteRotate(0.0f, 0.0f, 0.0f);
    AbsoluteScale(1.0f, 1.0f, 1.0f);
    m_fRotationX = m_fRotationY = m_fRotationZ = 0.0f;
    m_d3dmFace._41 = 0.0f;
    m_d3dmFace._42 = 0.0f;
    m_d3dmFace._43 = -1.0f;
}

D3DXVECTOR3  RenderObj::ComputePositionInMySurface(float x, float y, float z) {
    return D3DXVECTOR3(x, y, z);
}

LPDIRECT3DTEXTURE9 RenderObj::LoadTexture(LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR pSrcFile) {
    if (!pd3dDevice) {
        pd3dDevice = m_pd3dDevice;
    }
    LPDIRECT3DTEXTURE9 pTexture = NULL;
    if (FAILED(D3DXCreateTextureFromFile(pd3dDevice, pSrcFile, &pTexture))) {
        return NULL;
    }
    pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    return pTexture;
}

void RenderObj::InvalidateDeviceObjects(void) {
    if (m_pVertexBuffer) {
        static int nNewRefCount = m_pVertexBuffer->Release();
        if (nNewRefCount > 0) {
            static char strError[255];
            sprintf_s(strError, "The vertex object failed to cleanup properly.\n"
                    "Release() returned a reference count of %d", nNewRefCount);
            MessageBox(NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION);
        }
        m_pVertexBuffer = NULL;
    }

    if (m_pTexture) {
        int nNewRefCount = m_pTexture->Release();
        if (nNewRefCount > 0) {
            static char strError[255];
            sprintf_s(strError, "The texture object failed to cleanup properly.\n"
                    "Release() returned a reference count of %d", nNewRefCount);
            MessageBox(NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION);
        }
        m_pTexture = NULL;
    }

    if (m_pMesh) {
        int nNewRefCount = m_pMesh->Release();
        if (nNewRefCount > 0) {
            static char strError[255];
            sprintf_s(strError, "The mesh object failed to cleanup properly.\n"
                    "Release() returned a reference count of %d", nNewRefCount);
            MessageBox(NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION);
        }
        m_pMesh = NULL;
    }
}


void RenderObj::AbsoluteTranslate(float x, float y, float z) {
    m_d3dmTranslate._41 = x;
    m_d3dmTranslate._42 = y;
    m_d3dmTranslate._43 = z;
}

void RenderObj::RelativeTranslate(float x, float y, float z) {
    m_d3dmTranslate._41 += x;
    m_d3dmTranslate._42 += y;
    m_d3dmTranslate._43 += z;
}

void RenderObj::AbsoluteRotate(float x, float y, float z) {
    m_fRotationX = x;
    m_fRotationY = y;
    m_fRotationZ = z;
    D3DXMatrixRotationYawPitchRoll(&m_d3dmRotate, m_fRotationY, m_fRotationX, m_fRotationZ);
}
void RenderObj::RelativeRotate(float x, float y, float z) {
    m_fRotationX += x;
    m_fRotationY += y;
    m_fRotationZ += z;
    D3DXMatrixRotationYawPitchRoll(&m_d3dmRotate, m_fRotationY, m_fRotationX, m_fRotationZ);
}

void RenderObj::AbsoluteScale(float x, float y, float z) {
    m_d3dmScale._11 = x;
    m_d3dmScale._22 = y;
    m_d3dmScale._33 = z;
}

void RenderObj::RelativeScale(float x, float y, float z) {
    m_d3dmScale._11 += x;
    m_d3dmScale._22 += y;
    m_d3dmScale._33 += z;
}

void RenderObj::MoveAlongVector(D3DXVECTOR3 direction, float step) {
    D3DXVec3Normalize(&direction, &direction);
    D3DXVECTOR3 dvShift = step * direction;
    RelativeTranslate(dvShift.x, dvShift.y, dvShift.z);
}

D3DXVECTOR3 RenderObj::Rotation(D3DXVECTOR3 target) {
#define LENGTH(x, y) (sqrt(((x) * (x) + (y) * (y))))
    float length = D3DXVec3Length(&target), preY = target.y;
    float angle = atan2f(target.y, target.z) + 3 * D3DX_PI, len = LENGTH(target.y, target.z);
    target.y = len * sinf(angle + m_fRotationX);
    target.z = len * cosf(angle + m_fRotationX);

    angle = atan2f(target.z, target.x) + 3 * D3DX_PI; len = LENGTH(target.z, target.x);
    target.z = len * sinf(angle + m_fRotationY);
    target.x = len * cosf(angle + m_fRotationY);

    angle = atan2f(target.y, target.x) + 3 * D3DX_PI; len = LENGTH(target.y, target.x);
    target.y = len * sinf(angle + m_fRotationZ);
    target.x = len * cosf(angle + m_fRotationZ);
    D3DXVec3Normalize(&target, &target);

    target *= length;    
    target.y = preY;

    return target;
}