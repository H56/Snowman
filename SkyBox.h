#pragma once
#include "RenderObj.h"
class SkyBox : public RenderObj {
public:
    SkyBox(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fLength);
    ~SkyBox();

    void Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId = 0);
    void RestoreDeviceObjects(void);
    void InvalidateDeviceObjects(void);
    void LoadTexture(LPDIRECT3DDEVICE9 pd3dDevice = NULL, std::vector<LPCSTR> vpSrcFile = 
                     {"SkyboxTexture\\blizzard_ft.tga", "SkyboxTexture\\blizzard_bk.tga", 
                     "SkyboxTexture\\blizzard_lf.tga", "SkyboxTexture\\blizzard_rt.tga", 
                     "SkyboxTexture\\blizzard_up.tga", "SkyboxTexture\\blizzard_dn.tga" });
private:
    struct SkyBoxVertex {
        float x, y, z;
        float tu, tv;
        enum FVF {
            FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
        };
    };
    float m_fLength;
    bool  m_bRenderFrame;
    std::vector<LPDIRECT3DTEXTURE9> m_vpTexture;
};

