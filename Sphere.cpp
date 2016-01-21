#include "stdafx.h"
#include "RenderObj.h"
#include "Sphere.h"



Sphere::Sphere(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fRadius, UINT  uSlices, UINT uStacks,
               bool bBlending) : RenderObj(x, y, z) {
    m_pd3dDevice = pd3dDevice;
    m_fRadius    = fRadius;
    m_uSlices    = uSlices;
    m_uStacks    = uStacks;
    m_bBlending  = bBlending;
    
    RestoreDeviceObjects();
}


Sphere::~Sphere() {
}

void Sphere::Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId) {
    if (!m_pTexture) {
        m_pTexture = LoadTexture();
    }
    std::map<int, DWORD> mDwRenderState;
    pd3dDevice->SetTransform(D3DTS_WORLD, GetTransform());
    if (m_bBlending) {
        DWORD dwState;
        pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwState);
        mDwRenderState[D3DRS_SRCBLEND] = dwState;
        pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

        pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &dwState);
        mDwRenderState[D3DRS_SRCBLEND] = dwState;
        pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

        pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &dwState);
        mDwRenderState[D3DRS_DESTBLEND] = dwState;
        pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    } /*else {
      pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
      pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
      }  */
      //pd3dDevice->SetMaterial(&m_d3dMaterial);
    pd3dDevice->SetTexture(0, m_pTexture);
    m_pMesh->DrawSubset(dwAttribId);

    if (m_bBlending) {
        for (auto &it : mDwRenderState) {
            pd3dDevice->SetRenderState((D3DRENDERSTATETYPE)it.first, it.second);
        }
    }
}

LPD3DXMESH Sphere::CreateMesh(LPDIRECT3DDEVICE9 pd3dDevice, float fRadius, UINT  uSlices, UINT uStacks) {
    LPD3DXMESH pMesh, pTexMesh;
    if (FAILED(D3DXCreateSphere(pd3dDevice, fRadius, uSlices, uStacks, &pMesh, NULL))) {
        return NULL;
    }
    if (FAILED(pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, Vertex::FVF_Flags | D3DFVF_NORMAL, pd3dDevice, &pTexMesh))) {
        return NULL;
    }
    pMesh->Release();
    Vertex *pVertices;
    if (SUCCEEDED(pTexMesh->LockVertexBuffer(0, (void **)&pVertices))) {
        int numVertices = pTexMesh->GetNumVertices();
        for (int i = 0; i < numVertices; ++i) {
            pVertices->tu = asinf(pVertices->norm.x) / D3DX_PI + 0.5f;
            pVertices->tv = asinf(pVertices->norm.y) / D3DX_PI + 0.5f;
            ++pVertices;
        }
        pTexMesh->UnlockIndexBuffer();
    }
    return pTexMesh;
}

