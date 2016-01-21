#pragma once
#include "RenderObj.h"
#include "Camera.h"
#include "Snowman.h"
#include "Terrain.h"
#include "Cube.h"
#include "SkyBox.h"
#include "DeviceResources.h"

class Renderer {
public:
    Renderer(std::shared_ptr<DeviceResources> deviceResources);
    ~Renderer();

    //static Renderer *GetRenderObj(std::shared_ptr<DeviceResources> deviceResources) {
    //    if (!s_pThis) {
    //        s_pThis = new Renderer(deviceResources);
    //    }
    //    return s_pThis;
    //}
    //static Renderer *GetRenderObj() {
    //    return s_pThis;
    //}
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    void Update();
    void Render();
    void UpdateDistance(float step = 0.1f);
    void UpdateSpinPosition(float fShiftSpinX, float fShiftSpinY);
    void SetDeviceLost() {
        m_bDeviceLost = true;
    }
    void UpKeyDown();
    void DownKeyDown();
    void LeftKeyDown();
    void RightKeyDown();

    void CreateViewAndPerspective();
    void GoForward();
    void GoBack();
    void GoLeft();
    void GoRight();

    void SetMousing(bool status) {
        m_bMousing = status;
    }

    void GetRealTimeUserInput(void);

private:

    //static Renderer *s_pThis;

    LPDIRECT3DDEVICE9 m_pd3dDevice;
    std::vector<RenderObj *> m_vMeshes;
    Terrain* m_pTerrain;
    bool m_bDeviceLost;
    bool m_bMousing;
    bool m_bCameraOnCube2;
    bool m_bCameraOnCube1;
    std::shared_ptr<DeviceResources> m_pDeviceResources;

    Camera*  m_pCamera;
    Snowman* m_pGamer;

    float m_fTerrainWidth;
    float m_fTerrainHeight;
    float m_fSkyHeight;
    float m_fViewLength;
    float m_fSnowmanHeight;

    Cube*    m_pCube1;
    Snowman* m_pSnowman1;
    Cube*    m_pCube2;
    Snowman* m_pSnowman2;
    SkyBox*  m_pSky;

    static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
};