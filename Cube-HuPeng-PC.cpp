#include "stdafx.h"
#include "cube.h"



Cube::Cube(LPDIRECT3D9 pD3D, HWND hWnd) {
    if(pD3D) {
        m_pD3D = pD3D;
    } else {
        m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
        D3DDISPLAYMODE d3ddm;
        m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed               = TRUE;
        d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat       = d3ddm.Format;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
        //m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
        //                     D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        //                     &d3dpp, &m_pd3dDevice);
    }
    m_pTexture = loadTexture();
    m_pd3dDevice->CreateVertexBuffer(24 * sizeof(Vertex), 0, Vertex::FVF_Flags,
                                     D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

    //if (pd3dDevice) {
    //    m_pd3dDevice = pd3dDevice;
    //}
}


Cube::~Cube() {
}

LPDIRECT3DTEXTURE9 Cube::loadTexture(LPCSTR pSrcFile, LPDIRECT3DDEVICE9 pd3dDevice) {
    if (!pd3dDevice) {
        pd3dDevice = m_pd3dDevice;
    }
    LPDIRECT3DTEXTURE9 pTexture;
    D3DXCreateTextureFromFile(pd3dDevice, pSrcFile, &pTexture);
    pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    return pTexture;
}