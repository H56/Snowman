#pragma once
class Snowman : public RenderObj {
    float m_fHeight;
    UINT  m_uStacks;
    UINT  m_uSlices;

    std::vector<std::vector<LPD3DXMESH>> m_vBodyMeshes;
    std::vector<LPDIRECT3DTEXTURE9>      m_vTexture;

public:
    Snowman(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fHeight);
    ~Snowman();

    void Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId = 0);
    std::vector<std::vector<LPD3DXMESH>> CreateMesh(LPDIRECT3DDEVICE9 pd3dDevice, float fHeight);
    std::vector<LPDIRECT3DTEXTURE9> LoadTexture(LPDIRECT3DDEVICE9 pd3dDevice = NULL, std::vector<LPCSTR> vpSrcFile = { "texture\\snow.bmp", "texture\\black.bmp", "texture\\red.bmp" });
    //void UpdateDistance(float step = 0.1f);
    //void UpdateSpinPosition(float fShiftSpinX, float fShiftSpinY);
    void RestoreDeviceObjects(void);
    void InvalidateDeviceObjects(void);
    void SetStacks(UINT uStacks) {
        m_uStacks = uStacks;
    }
    void SetSlices(UINT uSlices) {
        m_uSlices = uSlices;
    }
    void Move(D3DXVECTOR3 moveNormal, float right, float forward);
};

