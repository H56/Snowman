#include "stdafx.h"
#include "RenderObj.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Snowman.h"



Snowman::Snowman(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, float fHeight)
                 : RenderObj(x, y - fHeight, z) {
    this->m_pd3dDevice = pd3dDevice;
    this->m_fHeight    = fHeight;
    this->m_uSlices = 40;
    this->m_uStacks = 40;
    this->SetBlend(FALSE);

    this->RestoreDeviceObjects();
}


Snowman::~Snowman() {
}

void Snowman::Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId) {
    if (this->m_vTexture.empty()) {
        this->m_vTexture = LoadTexture();
    }
    pd3dDevice->SetTransform(D3DTS_WORLD, GetTransform());
    if (m_bBlending) {
        pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }/* else {
        pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    }*/
    pd3dDevice->SetMaterial(&m_d3dMaterial);
    for (int i = 0; i < m_vBodyMeshes.size(); ++i) {
        pd3dDevice->SetTexture(0, m_vTexture[i]);
        for (auto &mesh : m_vBodyMeshes[i]) {
            mesh->DrawSubset(dwAttribId);
        }
    }
}

std::vector<std::vector<LPD3DXMESH>> Snowman::CreateMesh(LPDIRECT3DDEVICE9 pd3dDevice, float fHeight) {
    std::vector<std::vector<LPD3DXMESH>> vecMeshes;
    float fHeadRadius = fHeight / 4.0f, fTrunkRadius = fHeight / 3.0f;
    
    // body meshes
    LPD3DXMESH pMeshTrunk = Sphere::CreateMesh(pd3dDevice, fTrunkRadius, m_uSlices, m_uStacks);
    LPD3DXMESH pMeshHead = Sphere::CreateMesh(pd3dDevice, fHeadRadius, m_uSlices, m_uStacks);
    Vertex *pVertices;
    float fTrunkHeight = 2 * fTrunkRadius, fShift = fHeadRadius / 8.0f;

    if (SUCCEEDED(pMeshTrunk->LockVertexBuffer(0, (void **)&pVertices))) {
        int numVertices = pMeshTrunk->GetNumVertices();
        for (int i = 0; i < numVertices; ++i) {
            pVertices->pos.y += fTrunkRadius;
            ++pVertices;
        }
        pMeshTrunk->UnlockVertexBuffer();
    } else {
        return vecMeshes;
    }

    if (SUCCEEDED(pMeshHead->LockVertexBuffer(0, (void **)&pVertices))) {
        int numVertices = pMeshHead->GetNumVertices();
        for (int i = 0; i < numVertices; ++i) {
            pVertices->pos.y += fHeadRadius + fTrunkHeight - fShift;
            ++pVertices;
        }
        pMeshHead->UnlockVertexBuffer();
    } else {
        return vecMeshes;
    }
    std::vector<LPD3DXMESH> vBodyMeshes;
    vBodyMeshes.push_back(pMeshHead);
    vBodyMeshes.push_back(pMeshTrunk);

    // eyes and buttons
    std::vector<LPD3DXMESH> vBlackMeshes;
    // eyes
    float fEyeRadius = fHeight / 30.0f;
    LPD3DXMESH pEyeLeft = Sphere::CreateMesh(pd3dDevice, fEyeRadius, m_uSlices, m_uStacks);
    if (SUCCEEDED(pEyeLeft->LockVertexBuffer(0, (void **)&pVertices))) {
        int numVertices = pEyeLeft->GetNumVertices();
        for (int i = 0; i < numVertices; ++i) {
            pVertices->pos.x -= fHeadRadius / 3.0f;
            pVertices->pos.y += 2.5f * fHeadRadius / 2.0f + fTrunkHeight - fShift;
            pVertices->pos.z -= fHeadRadius * 1.8f / 2.0f;
            ++pVertices;
        }
        pEyeLeft->UnlockVertexBuffer();
    } else {
        return vecMeshes;
    }

    LPD3DXMESH pEyeRight;
    if (FAILED(pEyeLeft->CloneMeshFVF(D3DXMESH_SYSTEMMEM, Vertex::FVF_Flags | D3DFVF_NORMAL, pd3dDevice, &pEyeRight))) {
        return vecMeshes;
    }
    if (SUCCEEDED(pEyeRight->LockVertexBuffer(0, (void **)&pVertices))) {
        int numVertices = pEyeRight->GetNumVertices();
        for (int i = 0; i < numVertices; ++i) {
            pVertices->pos.x += 2.0f * fHeadRadius / 3.0f;
            ++pVertices;
        }
        pEyeRight->UnlockVertexBuffer();
    } else {
        return vecMeshes;
    }
    vBlackMeshes.push_back(pEyeLeft);
    vBlackMeshes.push_back(pEyeRight);
    // mouth
    const UINT num = 20;
    float fMouthRadius = fHeadRadius / 2.0f, fMouthSphereRadius = fEyeRadius / 3.0f;
    float zShift = sqrtf(fHeadRadius * fHeadRadius - fMouthRadius * fMouthRadius);
    for (int i = 5; i < num - 5; ++i) {
        LPD3DXMESH pButton = Sphere::CreateMesh(pd3dDevice, fMouthSphereRadius, m_uSlices, m_uStacks);
        if (SUCCEEDED(pButton->LockVertexBuffer(0, (void **)&pVertices))) {
            int numVertices = pButton->GetNumVertices();
            float xShift = sinf((i - num / 2.0f) / (float)num * D3DX_PI) * fMouthRadius;
            float yShift = cosf((i - num / 2.0f) / (float)num * D3DX_PI) * fMouthRadius - fTrunkHeight - fHeadRadius + fShift;
            for (int j = 0; j < numVertices; ++j) {
                pVertices->pos.x += xShift;
                pVertices->pos.y -= yShift;
                pVertices->pos.z -= zShift;
                ++pVertices;
            }
            pButton->UnlockVertexBuffer();
        } else {
            return vecMeshes;
        }
        vBlackMeshes.push_back(pButton);
    }
    // buttons
    float fButtonRadius = fHeight / 20.0f;
    for (int i = 0; i < 3; ++i) {
        LPD3DXMESH pButton = Sphere::CreateMesh(pd3dDevice, fButtonRadius, m_uSlices, m_uStacks);
        if (SUCCEEDED(pButton->LockVertexBuffer(0, (void **)&pVertices))) {
            int numVertices = pButton->GetNumVertices();
            for (int j = 0; j < numVertices; ++j) {
                pVertices->pos.y += sinf((i - 1) / 6.0f * D3DX_PI) * fTrunkRadius + fTrunkRadius;
                pVertices->pos.z -= cosf((i - 1) / 6.0f * D3DX_PI) * fTrunkRadius;
                ++pVertices;
            }
            pButton->UnlockVertexBuffer();
        } else {
            return vecMeshes;
        }
        vBlackMeshes.push_back(pButton);
    }

    // nose
    float fRadius1 = 0.001f, fRadius2 = fHeadRadius / 10.0f, fLength = fHeadRadius;
    LPD3DXMESH pNose = Cylinder::CreateMesh(pd3dDevice, fRadius1, fRadius2, fLength, m_uSlices, m_uStacks);
    if (!pNose) {
        return vecMeshes;
    }
    if (SUCCEEDED(pNose->LockVertexBuffer(0, (void **)&pVertices))) {
        int numVertices = pNose->GetNumVertices();
        for (int i = 0; i < numVertices - 1; ++i) {
            pVertices->pos.y += fHeadRadius + fTrunkHeight - fShift;
            pVertices->pos.z -= (fHeadRadius * 0.7f + fLength / 2.0);
            ++pVertices;
        }
        pNose->UnlockVertexBuffer();
    } else {
        return vecMeshes;
    }
    std::vector<LPD3DXMESH> vRedMeshes;
    vRedMeshes.push_back(pNose);


    vecMeshes.push_back(vBodyMeshes);
    vecMeshes.push_back(vBlackMeshes);
    vecMeshes.push_back(vRedMeshes);
    return vecMeshes;
}

std::vector<LPDIRECT3DTEXTURE9> Snowman::LoadTexture(LPDIRECT3DDEVICE9 pd3dDevice, std::vector<LPCSTR> vpSrcFile) {
    if (!pd3dDevice) {
        pd3dDevice = m_pd3dDevice;
    }
    std::vector<LPDIRECT3DTEXTURE9> vRet;
    for (auto &file : vpSrcFile) {
        vRet.push_back(RenderObj::LoadTexture(pd3dDevice, file));
    }
    return vRet;
}

void Snowman::RestoreDeviceObjects(void) {
    InvalidateDeviceObjects();
    this->m_vTexture = this->LoadTexture();
    this->m_vBodyMeshes = this->CreateMesh(m_pd3dDevice, m_fHeight);

    ZeroMemory(&m_d3dMaterial, sizeof(D3DMATERIAL9));
    m_d3dMaterial.Ambient  = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
    m_d3dMaterial.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_d3dMaterial.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
    m_d3dMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
    m_d3dMaterial.Power = 10.0f;

}

void Snowman::InvalidateDeviceObjects(void) {
    RenderObj::InvalidateDeviceObjects();
    for (auto &meshes : m_vBodyMeshes) {
        for (auto &mesh : meshes) {
            int nNewRefCount = mesh->Release();
            if (nNewRefCount > 0) {
                static char strError[255];
                sprintf_s(strError, "The mesh object failed to cleanup properly.\n"
                        "Release() returned a reference count of %d", nNewRefCount);
                MessageBox(NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION);
            }
            mesh = NULL;
        }
    }
    m_vBodyMeshes.clear();

    for (auto &texture : m_vTexture) {
        int nNewRefCount = texture->Release();
        if (nNewRefCount > 0) {
            static char strError[255];
            sprintf_s(strError, "The texture object failed to cleanup properly.\n"
                    "Release() returned a reference count of %d", nNewRefCount);
            MessageBox(NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION);
        }
        texture = NULL;
    }
    m_vTexture.clear();
}

//void Snowman::UpdateDistance(float step) {
//    RenderObj::UpdateDistance(step);
//    this->m_sHead.UpdateDistance(step);
//    this->m_sBody.UpdateDistance(step);
//}
//
//void Snowman::UpdateSpinPosition(float fShiftSpinX, float fShiftSpinY) {
//    RenderObj::UpdateSpinPosition(fShiftSpinX, fShiftSpinY);
//    this->m_sHead.UpdateSpinPosition(fShiftSpinX, fShiftSpinY);
//    this->m_sBody.UpdateSpinPosition(fShiftSpinX, fShiftSpinY);
//}

void Snowman::Move(D3DXVECTOR3 moveNormal, float right, float forward) {
    D3DXVECTOR3 moveForward = moveNormal, moveRight, move;
    moveRight.z = -moveForward.x;
    moveRight.x = moveForward.z;
    moveForward.y = moveRight.y = 0.0f;
    move = forward * moveForward + right * moveRight;
    D3DXVec3Normalize(&move, &move);
    if (moveNormal.z != 0.0f && move.z != 0.0f) {
        float angle = atan2f(move.x, move.z) + D3DX_PI;
        if (m_fRotationY != angle) {
            RelativeRotate(0.0f, angle - m_fRotationY, 0.0f);
        }
    }
    //m_d3dmTranslate._41 += move.x;
    //m_d3dmTranslate._42 += move.y;
    //m_d3dmTranslate._43 += move.z;
    RelativeTranslate(move.x, 0.0f, move.z);
}