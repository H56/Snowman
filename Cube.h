#pragma once
#include "RenderObj.h"

class Cube : public RenderObj {
private:
    float m_fWidth;
    float m_fHeight;
    float m_fDepth;

    void GetTexturePosition(D3DXVECTOR3 position, float &tu, float &tv) {
        if (position.z < 0) {
            tu = position.x < 0 ? 0 : 1;
            tv = position.y < 0 ? 0 : 1;
        } else {
            tu = position.x < 0 ? 1 : 0;
            tv = position.y < 0 ? 0 : 1;
        }
    }

public:
    Cube(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f, bool bBlending = false);
    ~Cube();

    void RestoreDeviceObjects(void) {
        InvalidateDeviceObjects();
        this->m_pMesh = this->CreateMesh(m_fWidth, m_fHeight, m_fDepth);
        this->m_pTexture = this->LoadTexture(m_pd3dDevice, "texture\\radiation_box.tga");
    }
    void Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId = 0);
    LPD3DXMESH CreateMesh(float fWidth, float fHeight, float fDepth);
    LPD3DXMESH GetMesh() {
        if (!this->m_pMesh) {
            return this->CreateMesh(this->m_fWidth, this->m_fHeight, this->m_fDepth);
        }
        return this->m_pMesh;
    }
    float GetWidth() {
        return m_fWidth;
    }
    float GetHeight() {
        return m_fHeight;
    }
    float GetDepth() {
        return m_fDepth;
    }
    bool IsOnMe(D3DXVECTOR3 position);
};

