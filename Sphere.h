#pragma once
class Sphere : public RenderObj {
private:
    float m_fRadius;
    UINT  m_uSlices;
    UINT  m_uStacks;

public:
    Sphere(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fRadius = 1.5f, UINT  uSlices = 40, UINT uStacks = 40, bool bBlending = false);
    ~Sphere();

    static LPD3DXMESH CreateMesh(LPDIRECT3DDEVICE9 pd3dDevice, float fRadius, UINT  uSlices, UINT uStacks);
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
            return this->CreateMesh(this->m_pd3dDevice, this->m_fRadius, this->m_uSlices, this->m_uStacks);
        }
        return this->m_pMesh;
    }
    void RestoreDeviceObjects(void) {
        InvalidateDeviceObjects();
        this->m_pMesh = this->CreateMesh(m_pd3dDevice, m_fRadius, m_uSlices, m_uStacks);
        this->m_pTexture = this->LoadTexture();
    }
};

