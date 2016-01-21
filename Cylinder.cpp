#include "stdafx.h"
#include "RenderObj.h"
#include "Cylinder.h"



Cylinder::Cylinder(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fRadius1, float fRadius2, float fLength, UINT  uSlices, UINT uStacks, 
                   bool bBlending) : RenderObj(x, y, z) {
    m_pd3dDevice = pd3dDevice;
    m_fRadius1   = fRadius1;
    m_fRadius2   = fRadius2;
    m_uSlices    = uSlices;
    m_uStacks    = uStacks;
    m_bBlending  = bBlending;
    m_fLength    = fLength;

    RestoreDeviceObjects();
}


Cylinder::~Cylinder() {
}

void Cylinder::Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId) {
    // Set texture
    std::map<int, DWORD> mDwRenderState;
    if (!m_pTexture) {
        m_pTexture = LoadTexture();
    }
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

LPD3DXMESH Cylinder::CreateMesh(LPDIRECT3DDEVICE9 pd3dDevice, float fRadius1, float fRadius2, float fLength, UINT  uSlices, UINT uStacks) {
    LPD3DXMESH pMesh, pTexMesh;
    if (FAILED(D3DXCreateCylinder(pd3dDevice, fRadius1, fRadius2, fLength, uSlices, uStacks, &pMesh, NULL))) {
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
