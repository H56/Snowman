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
    m_fTerrainWidth    = 20000.0f;
    m_fTerrainHeight   = m_fTerrainWidth / 20.0f;
    m_fSkyHeight       = m_fTerrainWidth / 10.0f;
    m_fViewLength      = m_fSkyHeight * 2.0f;
    m_bCameraOnCube2   = false;
    m_bCameraOnCube1   = false;
    m_fSnowmanHeight      = 10.0f;

    m_pCamera = new Camera(0.0f, 100.0f, -20.0f);
    m_pCamera->SetTargetPosition(0.0f, 0.0f, 0.0f);
    m_pCamera->SetFarPlane(m_fViewLength);
    m_pCamera->UpdateViewMatrix();
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


    m_pSky = new SkyBox(m_pd3dDevice, 0.0f, 0.0f, 0.0f, m_fSkyHeight);
    m_vMeshes.push_back(m_pSky);

    m_pTerrain = new Terrain(m_pd3dDevice, 0.0f, m_fTerrainHeight, 0.0f);
    m_pTerrain->SetSize(m_fTerrainWidth, m_fTerrainWidth, m_fTerrainHeight * 2);
    m_pTerrain->SetBlend(false);
    m_vMeshes.push_back(m_pTerrain);


    m_pGamer = new Snowman(m_pd3dDevice, 0.0f, 0.0f, 0.0f, m_fSnowmanHeight);
    m_pGamer->SetBlend(false);
    m_vMeshes.push_back((RenderObj *)m_pGamer);


    m_pCube1 = new Cube(m_pd3dDevice, -80.0f, 0.0f, 0.0f, 20.0f, 20.0f, 20.0f, true);
    m_pCube1->SetBlend(false);
    D3DXVECTOR3 dvCube = m_pTerrain->ComputePositionInMySurface(m_pCube1->GetPosition());
    //dvCube.y += 10.0f;
    m_pCube1->SetPosition(dvCube);
    D3DXVECTOR3 position = m_pCube1->GetPosition();
    m_pSnowman1 = new Snowman(m_pd3dDevice, position.x, position.y + 20.0f, position.z, m_fSnowmanHeight);
    m_pSnowman1->SetBlend(false);
    m_vMeshes.push_back((RenderObj *)m_pCube1);
    m_vMeshes.push_back((RenderObj *)m_pSnowman1);


    m_pCube2 = new Cube(m_pd3dDevice, 50.0f, 0.0f, 0.0f, 20.0f, 20.0f, 20.0f, true);
    m_pCube2->SetBlend(false);
    dvCube = m_pTerrain->ComputePositionInMySurface(m_pCube2->GetPosition());
    dvCube.y += 15.0f;
    m_pCube2->SetPosition(dvCube);
    position = m_pCube2->GetPosition();
    m_pSnowman2 = new Snowman(m_pd3dDevice, position.x, position.y + 20.0f, position.z, 10.0f);
    m_pSnowman2->SetBlend(false);
    m_vMeshes.push_back((RenderObj *)m_pCube2);
    m_vMeshes.push_back((RenderObj *)m_pSnowman2);


    m_pGamer->SetPosition(m_pTerrain->ComputePositionInMySurface(m_pGamer->GetPosition()));
    position = m_pCamera->GetCameraPosition();
    position.y = m_pGamer->GetPosition().y + m_fSnowmanHeight;
    m_pCamera->SetCameraPosition(position);
}

void Renderer::CreateWindowSizeDependentResources() {
    // Create the view matrix and the perspective matrix.
    CreateViewAndPerspective();
}


//-----------------------------------------------------------------------------
// Update the scene.
//-----------------------------------------------------------------------------
void Renderer::Update() {
    m_pDeviceResources->SetLightPosition(0, D3DXVECTOR3(0.0f, 200.0f, -100.0f));
    GetRealTimeUserInput();
    // Rotate the cube 1 degree per frame.
    if (m_pCube2->IsOnMe(m_pCamera->GetCameraPosition())) {
        m_bCameraOnCube2 = true;
    } else if(m_pCube1->IsOnMe(m_pCamera->GetCameraPosition())) {
        m_bCameraOnCube1 = true;
    } else {
        m_bCameraOnCube2 = false;
        m_bCameraOnCube1 = false;
        D3DXVECTOR3 position = m_pGamer->GetPosition();
        m_pCamera->SetTargetPosition(position);
    }

    //m_pCamera->SetCameraPosition(position.x, position.y + 50.0f, position.z - 100.0f);
    //m_pCamera->SetLength(m_fViewLength);
    //D3DXVECTOR3 move = m_pCamera->GetMoveVector();
    //m_pSky->MoveAlongVector(move, D3DXVec3Length(&move));
    m_pCamera->UpdateViewMatrix();    
    m_pSky->SetPosition(m_pCamera->GetCameraPosition());
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
    m_pd3dDevice->LightEnable(0, TRUE);
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
    float y = m_pCamera->GetCameraPosition().y;
    if (y < m_fSkyHeight) {
        m_pCamera->MoveAlongLookVector(step * 5.0f);
    }
    //m_pGamer->UpdateDistance(step);
    //m_pCamera->SetLength(m_fViewLength -= step);
}
void Renderer::UpdateSpinPosition(float fShiftSpinX, float fShiftSpinY) {
    //for (auto &mesh : this->m_vMeshes) {
    //    mesh->SpinXY(fShiftSpinX * 0.005, fShiftSpinY * 0.005);
    //}
    //m_pCamera->SpinXY(fShiftSpinX * 0.005, fShiftSpinY * 0.005);
    //m_pGamer->SpinXY(fShiftSpinX * 0.005, fShiftSpinY * 0.005);
    if (fShiftSpinY < 0.0f && m_pCamera->GetCameraY() < m_pCube2->GetPosition().y + 10.0f) {
        fShiftSpinY = 0.0f;
    }
    m_pCamera->Spin(0.0f, fShiftSpinX, fShiftSpinY);
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
    m_vMeshes.clear();
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
    UpdateSpinPosition(-1.0f, 0.0f);
}
void Renderer::RightKeyDown() {
    UpdateSpinPosition(1.0f, 0.0f);
}

void Renderer::GoForward() {
    D3DXVECTOR3 move = m_pGamer->GetPosition() - m_pCamera->GetCameraPosition();
    m_pGamer->Move(move, 0.0f, 1.0f);
    D3DXVECTOR3 positionMove = m_pGamer->GetPosition();
    D3DXVECTOR3 position = m_pTerrain->ComputePositionInMySurface(positionMove);
    m_pGamer->SetPosition(position);
    float length = positionMove == position ? 1.0f : 0.0f;
    m_pCamera->MoveForward(length);
    m_pCamera->SetTargetPosition(position);
}
void Renderer::GoBack() {
    D3DXVECTOR3 move = m_pGamer->GetPosition() - m_pCamera->GetCameraPosition();
    m_pGamer->Move(move, 0.0f, -1.0f);
    D3DXVECTOR3 positionMove = m_pGamer->GetPosition();
    D3DXVECTOR3 position = m_pTerrain->ComputePositionInMySurface(positionMove);
    m_pGamer->SetPosition(position);
    positionMove.y = position.y = 0.0f;
    float length = positionMove == position ? -1.0f : 0.0f;
    m_pCamera->MoveForward(length);
    m_pCamera->SetTargetPosition(position);
}
void Renderer::GoLeft() {
    D3DXVECTOR3 move = m_pGamer->GetPosition() - m_pCamera->GetCameraPosition();
    m_pGamer->Move(move, -1.0f, 0.0f);
    //m_pCamera->MoveRight(-1.0f);
    D3DXVECTOR3 position = m_pTerrain->ComputePositionInMySurface(m_pGamer->GetPosition());
    m_pGamer->SetPosition(position);
    m_pCamera->SetTargetPosition(position);

}
void Renderer::GoRight() {
    D3DXVECTOR3 move = m_pGamer->GetPosition() - m_pCamera->GetCameraPosition();
    m_pGamer->Move(move, 1.0f, 0.0f);
    //m_pCamera->MoveRight(1.0f);
    D3DXVECTOR3 position = m_pTerrain->ComputePositionInMySurface(m_pGamer->GetPosition());
    m_pGamer->SetPosition(position);
    m_pCamera->SetTargetPosition(position);
}

// 旋转定时器
VOID CALLBACK  Renderer::TimerProc(HWND  hwnd, UINT     uMsg, UINT_PTR idEvent, DWORD    dwTime) {
    Renderer* pThis = (Renderer *)idEvent;
    pThis->m_pCube2->SpinXY(0.0f, 0.05f);
    pThis->m_pSnowman2->SpinXY(0.0f, 0.05f);
    // 自转
    if (pThis->m_bCameraOnCube2) {
        // pThis->m_pCamera->Spin(0.05f, 0.0f);
        D3DXVECTOR3 cameraPosition = pThis->m_pCamera->GetCameraPosition(), gamerPosition = pThis->m_pGamer->GetPosition();
        cameraPosition = 100.0f * (gamerPosition - cameraPosition);
        cameraPosition.y = pThis->m_pCamera->GetTargetPosition().y;
        pThis->m_pCamera->SetTargetPosition(pThis->m_pCube2->Rotation(cameraPosition));
    }
    D3DXVECTOR3 position1 = pThis->m_pSnowman1->GetPosition(), position2 = pThis->m_pCube1->GetPosition(), pTmp = position1;
    pTmp.y = 0.0f;
    float length = D3DXVec3Length(&pTmp), angle = atan2f(position1.z, position1.x) + 0.005f;
    position1.x = position2.x = length * cosf(angle);
    position1.z = position2.z = length * sinf(angle);    
    pThis->m_pSnowman1->SetPosition(position1);
    pThis->m_pCube1->SetPosition(position2);
    // 围绕(0, 0)公转
    if (pThis->m_bCameraOnCube1) {
        //pThis->m_pCamera->SetTargetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
        D3DXVECTOR3 cameraPosition = pThis->m_pCamera->GetCameraPosition();
        cameraPosition.x = position1.x;
        cameraPosition.z = position1.z;
        pThis->m_pCamera->SetCameraPosition(cameraPosition);
        D3DXVECTOR3 directionVec = cameraPosition;
        directionVec.z += position1.x;
        directionVec.x += -position1.z;
        pThis->m_pCamera->SetTargetPosition(directionVec);
    }
    //pThis->Update();
    //pThis->Render();
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
