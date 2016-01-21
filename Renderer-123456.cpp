#include "stdafx.h"
#include "resource.h"

#include <string>
#include <memory>
#include <ppltasks.h>

//#include "Camera.h"
//#include "Cube.h"
//#include "Sphere.h"
//#include "Snowman.h"
#include "SkyBox.h"
//#include "Terrain.h"
#include "Renderer.h"


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
Renderer::Renderer(std::shared_ptr<DeviceResources> deviceResources) {

    m_pd3dDevice       = deviceResources->GetDevice();
    m_bDeviceLost      = false;
    m_pDeviceResources = deviceResources;
    m_fSkyLength       = 1000.0f;
    m_fViewLength      = 100.0f;

    m_pCamera = new Camera(0.0f, 100.0f, -50.0f);
    m_pCamera->SetTargetPosition(0.0f, 0.0f, 0.0f);
    m_pCamera->SetFarPlane(200.0f);
    m_pCamera->UpdateViewMatrix();
}

//-----------------------------------------------------------------------------
// Create Direct3D shader resources by loading the .cso files.
//-----------------------------------------------------------------------------
HRESULT Renderer::CreateShaders() {
    HRESULT hr = S_OK;
    return hr;
}

//-----------------------------------------------------------------------------
// Create the view matrix and create the perspective matrix.
//-----------------------------------------------------------------------------
void Renderer::CreateViewAndPerspective() {
    RECT rect;
    if (GetWindowRect(m_pDeviceResources->GetHwnd(), &rect)) {
        m_pCamera->SetAspectRatio((float)(rect.right - rect.left) / (float)(rect.bottom - rect.top));
    }
    // set projection
    m_pd3dDevice->SetTransform(D3DTS_PROJECTION, m_pCamera->GetProjectionMatrix());

    SetTimer(m_pDeviceResources->GetHwnd(), (UINT_PTR)this, 1.0f, TimerProc);
}

//-----------------------------------------------------------------------------
// Create device-dependent resources for rendering.
//-----------------------------------------------------------------------------
void Renderer::CreateDeviceDependentResources() {
    // add cube
    //m_vMeshes.push_back((RenderObj *)(new Cube(m_pd3dDevice, 0.0f, 0.0f, 4.5f)));
    //m_vMeshes.push_back((RenderObj *)(new Sphere(m_pd3dDevice, 0.0f, 0.0f, 4.5f)));


    m_pTerrain = new Terrain(m_pd3dDevice, 0.0f, 0.0f, 0.0f);
    m_pTerrain->SetSize(m_fSkyLength + 10.0f, m_fSkyLength / 5, m_fSkyLength);
    m_pTerrain->SetBlend(false);
    m_vMeshes.push_back(m_pTerrain);
    m_vMeshes.push_back((RenderObj *)(new SkyBox(m_pd3dDevice, 0.0f, m_fSkyLength / 2.0, 0.0f, m_fSkyLength)));

    m_pGamer = new Snowman(m_pd3dDevice, 0.0f, 0.0f, 0.0f, 10.0f);
    m_pGamer->SetBlend(false);
    m_vMeshes.push_back((RenderObj *)m_pGamer);

    m_pCube = new Cube(m_pd3dDevice, 50.0f, 50.0f, 0.0f, 20.0f, 20.0f, 20.0f, true);
    m_pCube->SetBlend(false);
    m_pCube->SetPosition(m_pTerrain->ComputePositionInMySurface(m_pCube->GetPosition()));
    D3DXVECTOR3 position = m_pCube->GetPosition();
    m_pSnowman2 = new Snowman(m_pd3dDevice, position.x, position.y + 20.0f, position.z, 10.0f);
    m_pSnowman2->SetBlend(false);
    m_vMeshes.push_back((RenderObj *)m_pCube);
    m_vMeshes.push_back((RenderObj *)m_pSnowman2);


    m_pGamer->SetPosition(m_pTerrain->ComputePositionInMySurface(m_pGamer->GetPosition()));
}

void Renderer::CreateWindowSizeDependentResources() {
    // Create the view matrix and the perspective matrix.
    CreateViewAndPerspective();
}


//-----------------------------------------------------------------------------
// Update the scene.
//-----------------------------------------------------------------------------
void Renderer::Update() {
    //m_pDeviceResources->SetLightPosition(0, D3DXVECTOR3(0.0f, 0.0f, -50.0f));
    GetRealTimeUserInput();
    // Rotate the cube 1 degree per frame.
    D3DXVECTOR3 position = m_pGamer->GetPosition();
    m_pCamera->SetTargetPosition(position);
    //m_pCamera->SetCameraPosition(position.x, position.y + 50.0f, position.z - 100.0f);
    //m_pCamera->SetLength(m_fViewLength);
    m_pCamera->UpdateViewMatrix();
}

//-----------------------------------------------------------------------------
// Render the cube.
//-----------------------------------------------------------------------------
void Renderer::Render() {
    // set view matrix
    m_pd3dDevice->SetTransform(D3DTS_VIEW, m_pCamera->GetViewMatrix());
    // Use the Direct3D device context to draw.
    HRESULT hr;
    if (m_bDeviceLost) {
        if (FAILED(hr = m_pd3dDevice->TestCooperativeLevel())) {
            if (hr == D3DERR_DEVICELOST) {
                return;
            }
            if (hr == D3DERR_DEVICENOTRESET) {
                for (auto &obj : m_vMeshes) {
                    obj->InvalidateDeviceObjects();
                }
                if (FAILED(hr = m_pd3dDevice->Reset(&(m_pDeviceResources->GetD3DPP())))) {
                    return;
                }
                m_pDeviceResources->ResetDevice();
                for (auto &obj : m_vMeshes) {
                    obj->RestoreDeviceObjects();
                }
            }
            return;
        }
        m_bDeviceLost = false;
    }
    m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                        D3DCOLOR_COLORVALUE(0.894f, 0.894f, 0.894f, 1.0f), 1.0f, 0);
    //m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    //m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    if (SUCCEEDED(m_pd3dDevice->BeginScene())) {
        for (auto &mesh : m_vMeshes) {
            mesh->Render();
        }
        m_pd3dDevice->EndScene();
        hr = m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
        if (hr == D3DERR_DEVICELOST) {
            m_bDeviceLost = true;
        }
    }
    //_CrtDumpMemoryLeaks();
}

void Renderer::UpdateDistance(float step) {
    //for (auto &mesh : this->m_vMeshes) {
    //    mesh->UpdateDistance(step);
    //}
    m_pCamera->MoveAlongLookVector(step * 10);
    //m_pGamer->UpdateDistance(step);
    //m_pCamera->SetLength(m_fViewLength -= step);
}
void Renderer::UpdateSpinPosition(float fShiftSpinX, float fShiftSpinY) {
    //for (auto &mesh : this->m_vMeshes) {
    //    mesh->SpinXY(fShiftSpinX * 0.005, fShiftSpinY * 0.005);
    //}
    //m_pCamera->SpinXY(fShiftSpinX * 0.005, fShiftSpinY * 0.005);
    //m_pGamer->SpinXY(fShiftSpinX * 0.005, fShiftSpinY * 0.005);
    if (fShiftSpinY < 0.0f && m_pCamera->GetCameraY() < m_pGamer->GetPosition().y + 20.0f) {
        fShiftSpinY = 0.0f;
    }
    m_pCamera->Spin(fShiftSpinX, fShiftSpinY);
}

//-----------------------------------------------------------------------------
// Clean up cube resources when the Direct3D device is lost or destroyed.
//-----------------------------------------------------------------------------
Renderer::~Renderer() {
    // ComPtr will clean up references for us. But be careful to release
    // references to anything you don't need whenever you call Flush or Trim.
    // As always, clean up your system (CPU) memory resources before exit.
    for (auto &it : m_vMeshes) {
        delete it;
    }
    if (m_pCamera) {
        delete m_pCamera;
        m_pCamera = NULL;
    }
}

void Renderer::UpKeyDown() {
    UpdateSpinPosition(0.0f, 1.0f);
}
void Renderer::DownKeyDown() {
    UpdateSpinPosition(0.0f, -1.0f);
}
void Renderer::LeftKeyDown() {
    UpdateSpinPosition(1.0f, 0.0f);
}
void Renderer::RightKeyDown() {
    UpdateSpinPosition(-1.0f, 0.0f);
}

void Renderer::GoForward() {
    m_pGamer->Move(m_pCamera->GetMoveVector(0.0f, 1.0f));
    m_pGamer->SetPosition(m_pTerrain->ComputePositionInMySurface(m_pGamer->GetPosition()));
}
void Renderer::GoBack() {
    m_pGamer->Move(m_pCamera->GetMoveVector(0.0f, -1.0f));
    m_pGamer->SetPosition(m_pTerrain->ComputePositionInMySurface(m_pGamer->GetPosition()));

}
void Renderer::GoLeft() {
    m_pGamer->Move(m_pCamera->GetMoveVector(-1.0f, 0.0f));
    m_pGamer->SetPosition(m_pTerrain->ComputePositionInMySurface(m_pGamer->GetPosition()));

}
void Renderer::GoRight() {
    m_pGamer->Move(m_pCamera->GetMoveVector(0.1f, 0.0f));
    m_pGamer->SetPosition(m_pTerrain->ComputePositionInMySurface(m_pGamer->GetPosition()));
}


VOID CALLBACK  Renderer::TimerProc(HWND  hwnd, UINT     uMsg, UINT_PTR idEvent, DWORD    dwTime) {

    Renderer* pThis = (Renderer *)idEvent;
    pThis->m_pCube->SpinXY(0.05f, 0.0f);
    pThis->m_pSnowman2->SpinXY(0.05f, 0.0f);
    pThis->Update();
    //if (pThis->m_pCube->Is)

    pThis->Render();
}

void Renderer::GetRealTimeUserInput(void) {
    BYTE keys[256];
    GetKeyboardState(keys);
    if (keys['W'] & 0x80 || keys['w']) {
        GoForward();
    }
    if (keys['S'] & 0x80 || keys['s']) {
        GoBack();
    }
    if (keys['A'] & 0x80 || keys['a']) {
        GoLeft();
    }
    if (keys['D'] & 0x80 || keys['d']) {
        GoRight();
    }
    if (keys[VK_UP] & 0x80) {
        UpKeyDown();
    }
    if (keys[VK_DOWN] & 0x80) {
        DownKeyDown();
    }
    if (keys[VK_LEFT] & 0x80) {
        LeftKeyDown();
    }
    if (keys[VK_RIGHT] & 0x80) {
        RightKeyDown();
    }
}
