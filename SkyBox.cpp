#include "stdafx.h"
//#include "RenderObj.h"
#include "SkyBox.h"



SkyBox::SkyBox(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fLength)
               : RenderObj(x, y, z) {
    m_fLength = fLength;
    m_pd3dDevice = pd3dDevice;

    RestoreDeviceObjects();
}


SkyBox::~SkyBox() {
}

void SkyBox::Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId) {
    DWORD dwRenderStatus, dwTextureColorop, dwTextureColorRarg1;
    m_pd3dDevice->GetRenderState(D3DRS_LIGHTING, &dwRenderStatus);
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwTextureColorop);
    m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

    m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwTextureColorRarg1);
    m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    //D3DXMATRIX matWorld;
    //D3DXMatrixTranslation(&matWorld, x, y, z);
    //pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
    pd3dDevice->SetTransform(D3DTS_WORLD, GetTransform());
    m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SkyBoxVertex));
    m_pd3dDevice->SetFVF(SkyBoxVertex::FVF_Flags);
    for (int i = 0; i < m_vpTexture.size(); i++) {
        m_pd3dDevice->SetTexture(0, m_vpTexture[i]);
        m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
    }

    if (m_bRenderFrame)
    {
        m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        for (int i = 0; i < m_vpTexture.size(); i++) {
            m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
        }
        m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    }

    m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, dwRenderStatus);
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwTextureColorop);
    m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwTextureColorRarg1);

}
void SkyBox::RestoreDeviceObjects(void) {
    InvalidateDeviceObjects();
    LoadTexture();
    m_pd3dDevice->CreateVertexBuffer(4 * m_vpTexture.size() * sizeof(SkyBoxVertex), 0, SkyBoxVertex::FVF_Flags, D3DPOOL_MANAGED, &m_pVertexBuffer, 0);
    float length = m_fLength / 2.0f;
    SkyBoxVertex vertices[] = {
        
        // front
        {  length, -length, -length, 0.0f, 1.0f }, // E
        {  length,  length, -length, 0.0f, 0.0f }, // F
        { -length, -length, -length, 1.0f, 1.0f }, // H
        { -length,  length, -length, 1.0f, 0.0f }, // G

        // back
        { -length, -length, length, 0.0f, 1.0f }, // A
        { -length,  length, length, 0.0f, 0.0f }, // B
        {  length, -length, length, 1.0f, 1.0f }, // D
        {  length,  length, length, 1.0f, 0.0f }, // C


        // left
        { -length, -length, -length, 0.0f, 1.0f }, // H
        { -length,  length, -length, 0.0f, 0.0f }, // G
        { -length, -length,  length, 1.0f, 1.0f }, // A
        { -length,  length,  length, 1.0f, 0.0f }, // B

        // right
        {  length, -length,  length, 0.0f, 1.0f }, // D
        {  length,  length,  length, 0.0f, 0.0f }, // C
        {  length, -length, -length, 1.0f, 1.0f }, // E
        {  length,  length, -length, 1.0f, 0.0f }, // F

        // up
        {  length,  length, -length, 0.0f, 1.0f }, // F
        {  length,  length,  length, 0.0f, 0.0f }, // C
        { -length,  length, -length, 1.0f, 1.0f }, // G
        { -length,  length,  length, 1.0f, 0.0f }, // B

        // bottom
        { -length, -length, -length, 0.0f, 1.0f }, // H
        { -length, -length,  length, 0.0f, 0.0f }, // A
        {  length, -length, -length, 1.0f, 1.0f }, // E
        {  length, -length,  length, 1.0f, 0.0f }, // D

    };

    void *pVertices;
    if (SUCCEEDED(m_pVertexBuffer->Lock(0, 0, (void **)&pVertices, 0))) {
        memcpy(pVertices, vertices, sizeof(vertices));
        m_pVertexBuffer->Unlock();
    }
}

void SkyBox::InvalidateDeviceObjects(void) {
    RenderObj::InvalidateDeviceObjects();
    for (auto &texture : m_vpTexture) {
        texture->Release();
    }
    m_vpTexture.clear();
}

void SkyBox::LoadTexture(LPDIRECT3DDEVICE9 pd3dDevice, std::vector<LPCSTR> vpSrcFile) {
    if (!pd3dDevice) {
        pd3dDevice = m_pd3dDevice;
    }
    for (auto &file : vpSrcFile) {
        m_vpTexture.push_back(RenderObj::LoadTexture(pd3dDevice, file));
    }
}
