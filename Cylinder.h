#pragma once
class Cylinder : public RenderObj {
private:
    float m_fRadius1, m_fRadius2, m_fLength;
    UINT  m_uSlices;
    UINT  m_uStacks;

public:
    Cylinder(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fRadius1 = 1.5f, float fRadius2 = 1.5f, float fLength = 5.0f, UINT  uSlices = 40, UINT uStacks = 40, bool bBlending = false);
    ~Cylinder();

    static LPD3DXMESH CreateMesh(LPDIRECT3DDEVICE9 pd3dDevice, float fRadius1, float fRadius2, float fLength, UINT  uSlices, UINT uStacks);
    void SetBlend(bool bStatus) {
        this->m_bBlending = bStatus;
    }
    LPDIRECT3DTEXTURE9 LoadTexture(LPDIRECT3DDEVICE9 pd3dDevice = NULL, LPCSTR pSrcFile = "radiation_box.tga") {
        if (!pd3dDevice) {
            pd3dDevice = m_pd3dDevice;
        }
        return RenderObj::LoadTexture(pd3dDevice, pSrcFile);
    }
    void Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId = 0);
    LPD3DXMESH GetMesh() {
        if (!this->m_pMesh) {
            return this->CreateMesh(this->m_pd3dDevice, this->m_fRadius1, this->m_fRadius2, this->m_fLength, this->m_uSlices, this->m_uStacks);
        }
        return this->m_pMesh;
    }
    void RestoreDeviceObjects(void) {
        InvalidateDeviceObjects();
        this->m_pMesh = this->CreateMesh(m_pd3dDevice, m_fRadius1, m_fRadius2, m_fLength, m_uSlices, m_uStacks);
        this->m_pTexture = this->LoadTexture();
    }
};

