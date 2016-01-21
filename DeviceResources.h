#pragma once

class DeviceResources {
public:
    DeviceResources();
    ~DeviceResources();

    HRESULT CreateDeviceResources(HWND hWnd);
    HRESULT CreateWindowResources(HWND hWnd);

    //HRESULT ConfigureBackBuffer();
    //HRESULT ReleaseBackBuffer();
    HRESULT GoFullScreen();
    HRESULT GoWindowed();

    //ID3D11Device*           GetDevice() { return m_pd3dDevice.Get(); };
    //ID3D11DeviceContext*    GetDeviceContext() { return m_pd3dDeviceContext.Get(); };
    //ID3D11RenderTargetView* GetRenderTarget() { return m_pRenderTarget.Get(); }
    //ID3D11DepthStencilView* GetDepthStencil() { return m_pDepthStencilView.Get(); }
    LPDIRECT3DDEVICE9       GetDevice() {
        return m_pd3dDevice;
    };

    D3DPRESENT_PARAMETERS&  GetD3DPP() {
        return m_d3dpp;
    }

    HWND GetHwnd() const {
        return m_hWnd;
    }

    void ResetDevice(void);
    void InvalidateDeviceObjects();
    bool BuildPresentParameters();
    void SetLightPosition(UINT id, D3DXVECTOR3 position) {
        m_vLight[id].Position = position;
        m_pd3dDevice->SetLight(id, &(m_vLight[id]));
    }

private:

    //-----------------------------------------------------------------------------
    // Direct3D device
    //-----------------------------------------------------------------------------
    //Microsoft::WRL::ComPtr<ID3D11Device>        m_pd3dDevice;
    //Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pd3dDeviceContext; // immediate context
    //Microsoft::WRL::ComPtr<IDXGISwapChain>      m_pDXGISwapChain;
    LPDIRECT3DDEVICE9       m_pd3dDevice = NULL;
    LPDIRECT3D9             m_pD3D = NULL;
    LPD3DXMESH              m_pMesh = NULL;
    LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer = NULL;
    LPDIRECT3DTEXTURE9      m_pTexture = NULL;
    D3DPRESENT_PARAMETERS   m_d3dpp;
    std::vector<D3DLIGHT9>  m_vLight;
    D3DDISPLAYMODE          m_displayMode;
    D3DCAPS9                m_d3dCaps;

    HWND                    m_hWnd;

    bool                    m_bFullScreen;



    //-----------------------------------------------------------------------------
    // DXGI swap chain device resources
    //-----------------------------------------------------------------------------
    //Microsoft::WRL::ComPtr < ID3D11Texture2D>        m_pBackBuffer;
    //Microsoft::WRL::ComPtr < ID3D11RenderTargetView> m_pRenderTarget;


    //-----------------------------------------------------------------------------
    // Direct3D device resources for the depth stencil
    //-----------------------------------------------------------------------------
    //Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_pDepthStencil;
    //Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;


    //-----------------------------------------------------------------------------
    // Direct3D device metadata and device resource metadata
    //-----------------------------------------------------------------------------
    //D3D_FEATURE_LEVEL       m_featureLevel;
    //D3D11_TEXTURE2D_DESC    m_bbDesc;
    //D3D11_VIEWPORT          m_viewport;
};
