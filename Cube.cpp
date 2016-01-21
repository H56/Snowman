#include "stdafx.h"
#include "RenderObj.h"
#include "Cube.h"



Cube::Cube(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fWidth, 
           float fHeight, float fDepth, bool bBlending) : RenderObj(x, y, z) {
    this->m_pd3dDevice = pd3dDevice;
    this->m_fWidth     = fWidth;
    this->m_fHeight    = fHeight;
    this->m_fDepth     = fDepth;
    this->m_bBlending  = bBlending;

    RestoreDeviceObjects();

    ZeroMemory(&m_d3dMaterial, sizeof(D3DMATERIAL9));
    m_d3dMaterial.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
    m_d3dMaterial.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_d3dMaterial.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
    m_d3dMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
    m_d3dMaterial.Power = 100.0f;


}


Cube::~Cube() {
}

void Cube::Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId) {
    // Set texture
    if (!m_pTexture) {
        m_pTexture = LoadTexture(pd3dDevice, "snow.bmp");
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
    pd3dDevice->SetMaterial(&m_d3dMaterial);
    pd3dDevice->SetTexture(0, m_pTexture);
    m_pMesh->DrawSubset(dwAttribId);

    if (m_bBlending) {
        for (auto &it : mDwRenderState) {
            pd3dDevice->SetRenderState((D3DRENDERSTATETYPE)it.first, it.second);
        }
    }
}

LPD3DXMESH Cube::CreateMesh(float fWidth, float fHeight, float fDepth) {
    LPD3DXMESH pMesh, pTexMesh;
    if (FAILED(D3DXCreateBox(m_pd3dDevice, fWidth, fHeight, fDepth, &pMesh, NULL))) {
        return NULL;
    }

    if (FAILED(pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, Vertex::FVF_Flags | D3DFVF_NORMAL, this->m_pd3dDevice, &pTexMesh))) {
        return NULL;
    }
    pMesh->Release();
    Vertex *pVertex;
    if (SUCCEEDED(pTexMesh->LockVertexBuffer(0, (void **)&pVertex))) {
        int numVertex = pTexMesh->GetNumVertices();
        for (int i = 0; i < numVertex; ++i) {
           /* D3DXVECTOR3 v = pVertex->pos;
            D3DXVec3Normalize(&v, &v);*/
            GetTexturePosition(pVertex->pos, pVertex->tu, pVertex->tv);
            ++pVertex;
        }
        pTexMesh->UnlockVertexBuffer();
    }
    return pTexMesh;
}

//void Cube::GenerateNormalVertex(Vertex* pVertices, UINT uNumVerticesAlongWidth, UINT uNumVerticesAlongLength) {
//    for (int z = 0; z < uNumVerticesAlongLength; ++z) {
//        for (int x = 0; x < uNumVerticesAlongWidth; ++x) {
//            float halfWidth = ((float)uNumVerticesAlongWidth - 1.0f) / 2.0f;
//            float halfLength = ((float)uNumVerticesAlongLength - 1.0f) / 2.0f;
//            pVertices[x + z * uNumVerticesAlongLength] = {
//                D3DXVECTOR3((float)x - halfWidth, 0.0f, (float)z - halfLength),
//                D3DXVECTOR3(0.0f, 1.0f, 0.0f),
//                (float)x / (uNumVerticesAlongWidth - 1), (float)z / (uNumVerticesAlongLength - 1)
//            };
//        }
//    }
//}
//
//void Cube::GenerateIndices(int* pIndices, UINT uNumVerticesAlongWidth, UINT uNumVerticesAlongLength) {
//    for (int z = 0, index = -1, x; z < uNumVerticesAlongLength - 1; ++z) {
//        if (z & 0x01) {
//            for (x = uNumVerticesAlongWidth - 1; x >= 0; --x) {
//                pIndices[++index] = x + (z * uNumVerticesAlongWidth);
//                pIndices[++index] = x + (z * uNumVerticesAlongWidth) + uNumVerticesAlongWidth;
//            }
//            if (z != uNumVerticesAlongLength - 2) {
//                pIndices[++index] = ++x + (z * uNumVerticesAlongWidth);
//            }
//        } else {
//            for (x = 0; x < uNumVerticesAlongWidth; ++x) {
//                pIndices[++index] = x + (z * uNumVerticesAlongWidth);
//                pIndices[++index] = x + (z * uNumVerticesAlongWidth) + uNumVerticesAlongWidth;
//            }
//            if (z != uNumVerticesAlongLength - 2) {
//                pIndices[++index] = --x + (z * uNumVerticesAlongWidth);
//            }
//        }
//    }
//}
bool Cube::IsOnMe(D3DXVECTOR3 position) {
    D3DXVECTOR3 myPosition = D3DXVECTOR3(m_d3dmTranslate._41, m_d3dmTranslate._42, m_d3dmTranslate._43);
    float length = D3DXVec3Length(&(position - myPosition));

    // incorrect, just estimate
    return length < sqrtf(2.0f) * m_fWidth / 2.0f;
}
