#include "stdafx.h"
#include "resource.h"

#include <string>
#include <memory>

#include "DeviceResources.h"


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
DeviceResources::DeviceResources() {

};


//-----------------------------------------------------------------------------
//
// Method 1: Create device and swap chain at the same time.
//
// Benefit:  It's easy.
// Drawback: You have to create a new device, and therefore
//           reload all DirectX device resources, every time
//           you recreate the swap chain.
//
//-----------------------------------------------------------------------------
HRESULT DeviceResources::CreateDeviceResources(HWND hWnd) {
    InvalidateDeviceObjects();
    HRESULT hr = S_OK;
    m_hWnd = hWnd;
    //if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
    //    return S_FALSE;
    //}

    //D3DDISPLAYMODE d3ddm;
    //if (FAILED(hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
    //    return hr;
    //}
    //ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
    //m_d3dpp.Windowed               = TRUE;
    //m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    //m_d3dpp.BackBufferFormat       = d3ddm.Format;
    //m_d3dpp.EnableAutoDepthStencil = TRUE;
    //m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    //m_d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    //hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice);
    //
    //// Setup a light
    //m_d3dLight0.Type = D3DLIGHT_DIRECTIONAL;
    //m_d3dLight0.Ambient   = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
    //m_d3dLight0.Direction = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
    //m_d3dLight0.Diffuse   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    //m_d3dLight0.Specular  = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);


    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!m_pD3D) {
        return FALSE;
    }

    m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_displayMode);
    m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3dCaps);

    // Check for hardware T&L
    DWORD vertexProcessing = 0;
    if (m_d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
        vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
        // Check for pure device
        if (m_d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE) {
            vertexProcessing |= D3DCREATE_PUREDEVICE;
        }
    } else {
        vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    if (!BuildPresentParameters()) {
        InvalidateDeviceObjects();
        return FALSE;
    }
    // Create the device
    hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, vertexProcessing, &m_d3dpp, &m_pd3dDevice);
    if (FAILED(hr)) {
        InvalidateDeviceObjects();
        return FALSE;
    }
    
    ResetDevice();    
    return hr;
}

//-----------------------------------------------------------------------------
// Method 2, continued. Creates the swap chain.
//-----------------------------------------------------------------------------
HRESULT DeviceResources::CreateWindowResources(HWND hWnd) {
    HRESULT hr = S_OK;
    return hr;
}


HRESULT DeviceResources::GoFullScreen() {
    HRESULT hr = S_OK;

    return hr;
}

HRESULT DeviceResources::GoWindowed() {
    HRESULT hr = S_OK;

    return hr;
}


//-----------------------------------------------------------------------------
// Destructor.
//-----------------------------------------------------------------------------
DeviceResources::~DeviceResources() {
    InvalidateDeviceObjects();
}

void DeviceResources::InvalidateDeviceObjects() {
    if (m_pTexture) {
        m_pTexture->Release();
        m_pTexture = NULL;
    }

    if (m_pVertexBuffer) {
        m_pVertexBuffer->Release();
        m_pVertexBuffer = NULL;
    }

    if (m_pd3dDevice) {
        m_pd3dDevice->Release();
        m_pd3dDevice = NULL;
    }

    if (m_pD3D) {
        m_pD3D->Release();
        m_pD3D = NULL;
    }
}

void DeviceResources::ResetDevice(void) {
    m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pd3dDevice->SetLight(0, &(m_vLight[0]));
    m_pd3dDevice->LightEnable(0, TRUE);
}

bool DeviceResources::BuildPresentParameters() {
    ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
    D3DFORMAT adapterFormat = (m_bFullScreen) ? D3DFMT_X8R8G8B8 : m_displayMode.Format;
    if (SUCCEEDED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8))) {
        m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    } else if (SUCCEEDED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8))) {
        m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
    } else if (SUCCEEDED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16))) {
        m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    } else {
        return false;
    }
    m_d3dpp.BackBufferWidth = (m_bFullScreen) ? m_displayMode.Width : 0;
    m_d3dpp.BackBufferHeight = (m_bFullScreen) ? m_displayMode.Height : 0;
    m_d3dpp.BackBufferFormat = adapterFormat;
    m_d3dpp.BackBufferCount = 1;
    m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    m_d3dpp.MultiSampleQuality = 0;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.hDeviceWindow = m_hWnd;
    m_d3dpp.Windowed = !m_bFullScreen;
    m_d3dpp.EnableAutoDepthStencil = TRUE;
    m_d3dpp.FullScreen_RefreshRateInHz = (m_bFullScreen) ? m_displayMode.RefreshRate : 0;
    m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    // light
    D3DLIGHT9 light0;
    ZeroMemory(&light0, sizeof(D3DLIGHT9));
    light0.Type = D3DLIGHT_DIRECTIONAL;
    light0.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    light0.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    light0.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
    light0.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.5f);
    light0.Range = 100.0f;
    light0.Falloff = 1.0f;
    light0.Attenuation0 = 0.0f;
    light0.Attenuation1 = 0.125f;
    light0.Attenuation2 = 0.3f;
    light0.Theta = D3DXToRadian(10.0f);
    light0.Phi = D3DXToRadian(15.0f);
    m_vLight.push_back(light0);

    return true;
}

