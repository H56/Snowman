#include "stdafx.h"
#include "RenderObj.h"
#include "Terrain.h"



Terrain::Terrain(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, std::string sTextureFile, std::string sTerrainFile) : RenderObj(x, y, z) {
    m_pd3dDevice   = pd3dDevice;
    m_bRenderFrame = false;
    m_sTextureFile = sTextureFile;
    m_sTerrainFile = sTerrainFile;
    m_fMinDistance = 5.0f;

    RestoreDeviceObjects();
}


Terrain::~Terrain() {
}

void Terrain::SetSize(float fWidth, float fLength, float fMaxHeight) {
    long uWidth = floor(sqrt((float)m_uNumVertices));
    RenderObj::AbsoluteScale(fWidth / (float)uWidth, fMaxHeight / (float)uWidth, fLength / (float)uWidth);
}

float Terrain::NormalizeDistance(float x) {
    long shift = floor(sqrt((float)m_uNumVertices));
    shift >>= 1;
    if (x < m_fMinDistance - shift) {
        x = m_fMinDistance - shift;
    } else if (x > shift - m_fMinDistance) {
        x = shift - m_fMinDistance;
    }
    return x;
}
D3DXVECTOR3 Terrain::ComputePositionInMySurface(float x, float y, float z) {
    x = NormalizeDistance(x / m_d3dmScale._11);
    z = NormalizeDistance(z / m_d3dmScale._33);
    y /= m_d3dmScale._22;

    long uWidth = (long)sqrt((float)m_vHeightData.size());
    long terrainX = (long)x + (uWidth >> 1)  - 1;
    long terrainZ = (long)z + (uWidth >> 1) - 1;
    if (terrainZ < 0) {
        terrainZ = 0;
    }
    std::vector<float> vHeight;
    vHeight.push_back(m_vHeightData[terrainZ * uWidth + terrainX]);
    vHeight.push_back(m_vHeightData[terrainZ * uWidth + terrainX + 1]);
    vHeight.push_back(m_vHeightData[terrainZ * (uWidth - 1) + terrainX]);
    vHeight.push_back(m_vHeightData[terrainZ * (uWidth - 1) + terrainX + 1]);

    float terrainY = vHeight[0];
    for (auto i = 1; i < vHeight.size(); ++i) {
        if (terrainY < vHeight[i]) {
            terrainY = vHeight[i];
        }
    }
    if (y < terrainY) {
        y += terrainY;
    }
    return D3DXVECTOR3(x * m_d3dmScale._11, y * m_d3dmScale._22 + m_d3dmTranslate._42, z * m_d3dmScale._33);
}

void Terrain::Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId) {
    if (!m_pTexture) {
        LoadTexture();
    }

    pd3dDevice->SetTransform(D3DTS_WORLD, GetTransform());

    m_pd3dDevice->SetTexture(0, m_pTexture);
    m_pd3dDevice->SetMaterial(&m_d3dMaterial);


    m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
    m_pd3dDevice->SetFVF(Vertex::FVF_Flags | D3DFVF_NORMAL);
    m_pd3dDevice->SetIndices(m_pIndexBuffer);

    // m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    // m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
    m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_uNumVertices, 0, 
                                       m_uNumIndices - 2);

    // m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    // m_pd3dDevice->SetTexture(0, 0);

    if (m_bRenderFrame)
    {
        m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                           m_uNumVertices, 0, m_uNumIndices - 2);
        m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    }
}

void Terrain::RestoreDeviceObjects(void) {
    InvalidateDeviceObjects();
    LoadTexture();
    LoadTerrainFromFile(m_sTerrainFile.c_str());
    /*if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_uNumVertices * sizeof(TerrainVertex),
                                                D3DUSAGE_WRITEONLY, TerrainVertex::FVF_Flags,
                                                D3DPOOL_MANAGED, &m_pVertexBuffer, 0))) {
        return;
    }
    TerrainVertex *pVertices = NULL;
    if (SUCCEEDED(m_pVertexBuffer->Lock(0, 0, (void **)&pVertices, 0))) {
        float fStartX = -m_fTerrainWidth / 2.0f, fEndX = m_fTerrainWidth / 2.0f;
        float fStartZ = m_fTerrainDepth / 2.0f,  fEndZ = -m_fTerrainDepth / 2.0f;
        float fTextureU = 10.0f / (float)m_uCellsPerRow;
        float fTextureV = 10.0f / (float)m_uCellsPerCol;

        int nIndex = 0, i = 0;
        for (float z = fStartZ; z >= fEndZ; z -= m_fCellsSpace, ++i) {
            int j = 0;
            for (float x = fStartX; x <= fEndX; x += m_fCellsSpace, ++j) {
                nIndex = i * m_uCellsPerRow + j;
                pVertices[nIndex] = TerrainVertex(x, m_vHeightData[nIndex], z, j * fTextureU, i * fTextureV);
                ++nIndex;
            }
        }

        m_pVertexBuffer->Unlock();
    }*/
    UINT uWidth = (int)sqrt((float)m_uNumVertices);
    if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_uNumVertices * sizeof(Vertex),
                                                D3DUSAGE_WRITEONLY, Vertex::FVF_Flags,
                                                D3DPOOL_MANAGED, &m_pVertexBuffer, 0))) {
        return;
    }
    Vertex *pVertices = NULL;
    if (SUCCEEDED(m_pVertexBuffer->Lock(0, 0, (void **)&pVertices, 0))) {
        GenerateTerrainVertex(pVertices, &m_vHeightData[0], uWidth, uWidth);
        m_pVertexBuffer->Unlock();
    }

    m_uNumIndices = GetIndicesNum(uWidth, uWidth);
    if (FAILED(m_pd3dDevice->CreateIndexBuffer(m_uNumIndices * sizeof(int),
                                               D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIndexBuffer, 0))) {
        return;
    }
    int *pIndices = NULL;
    if (SUCCEEDED(m_pIndexBuffer->Lock(0, 0, (void **)&pIndices, 0))) {
        GenerateIndices(pIndices, uWidth, uWidth);
        m_pIndexBuffer->Unlock();
    }
    ZeroMemory(&m_d3dMaterial, sizeof(D3DMATERIAL9));
    m_d3dMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_d3dMaterial.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
    m_d3dMaterial.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
    m_d3dMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
    m_d3dMaterial.Power = 0.0f;
}

void Terrain::LoadTerrainFromFile(LPCSTR pSrcFile) {
    std::ifstream iFile;
    iFile.open(pSrcFile, std::ios::binary);
    iFile.seekg(0, std::ios::end);

    m_vHeightData.resize(iFile.tellg());
    iFile.seekg(std::ios::beg);
    iFile.read((char *)&m_vHeightData[0], m_vHeightData.size());
    iFile.close();
    m_uNumVertices = m_vHeightData.size();
    //for (UINT i = 0; i < data.size(); ++i) {
    //    m_vHeightData[i] = m_fHeightScale * data[i];
    //}
}

void Terrain::InvalidateDeviceObjects(void) {
    RenderObj::InvalidateDeviceObjects();
    m_vHeightData.clear();
    if (m_pIndexBuffer) {
        m_pIndexBuffer->Release();
        m_pIndexBuffer = NULL;
    }
}

void Terrain::GenerateTerrainVertex(Vertex* pVertices, BYTE* pHeight, UINT uNumVerticesAlongWidth, UINT uNumVerticesAlongLength) {
    UINT uMin = UINT_MAX;
    for (auto &height : m_vHeightData) {
        if (uMin > height) {
            uMin = height;
        }
    }
    for (int z = 0; z < uNumVerticesAlongLength; ++z) {
        for (int x = 0; x < uNumVerticesAlongWidth; ++x) {
            float halfWidth = ((float)uNumVerticesAlongWidth - 1.0f) / 2.0f;
            float halfLength = ((float)uNumVerticesAlongLength - 1.0f) / 2.0f;
            UINT uIndex = x + z * uNumVerticesAlongLength;
            D3DXVECTOR3 position = D3DXVECTOR3((float)x - halfWidth, (float)pHeight[x + z * uNumVerticesAlongLength] - uMin, (float)z - halfLength);
            D3DXVECTOR3 normal;
            pVertices[uIndex] = {
                position,  normal,
                (float)x / (uNumVerticesAlongWidth - 1),
                (float)z / (uNumVerticesAlongLength - 1) 
            };
        }
    }

    for (int z = 0; z < uNumVerticesAlongLength; ++z) {
        for (int x = 0; x < uNumVerticesAlongWidth; ++x) {
            float halfWidth = ((float)uNumVerticesAlongWidth - 1.0f) / 2.0f;
            float halfLength = ((float)uNumVerticesAlongLength - 1.0f) / 2.0f;
            UINT uIndex = x + z * uNumVerticesAlongLength;
            D3DXVECTOR3 normal, vec1, vec2;
            if (z < uNumVerticesAlongLength - 1) {
                if (0x01 & z) {
                    if (x < uNumVerticesAlongWidth - 1) {
                        vec1 = pVertices[uIndex + uNumVerticesAlongWidth].pos - pVertices[uIndex].pos;
                        vec2 = pVertices[uIndex + uNumVerticesAlongWidth + 1].pos - pVertices[uIndex].pos;
                        //D3DXVec3Normalize(&normal, &normal);
                    } else {
                        vec1 = pVertices[uIndex + uNumVerticesAlongWidth].pos - pVertices[uIndex].pos;
                        vec2 = pVertices[uIndex + 1].pos - pVertices[uIndex].pos;
                    }
                } else {
                    if (x < uNumVerticesAlongWidth - 1) {
                        vec1 = pVertices[uIndex + uNumVerticesAlongWidth].pos - pVertices[uIndex].pos;
                        vec2 = pVertices[uIndex + uNumVerticesAlongWidth].pos - pVertices[uIndex].pos;
                        //D3DXVec3Normalize(&normal, &normal);
                    } else {
                        vec1 = pVertices[uIndex  - 1].pos - pVertices[uIndex].pos;
                        vec2 = pVertices[uIndex + 1].pos - pVertices[uIndex].pos;
                    }
                }
                D3DXVec3Cross(&normal, &vec1, &vec2);
                //D3DXVec3Normalize(&normal, &normal);
            } else {
                normal = pVertices[uIndex - uNumVerticesAlongWidth].norm;
            }
            pVertices[uIndex].norm = normal;
        }
    }
}

void Terrain::GenerateIndices(int* pIndices, UINT uNumVerticesAlongWidth, UINT uNumVerticesAlongLength) {
    //int numIndices = GetIndicesNum(uNumVerticesAlongWidth, uNumVerticesAlongLength);
    for (int z = 0, index = -1, x; z < uNumVerticesAlongLength - 1; ++z) {
        if (z & 0x01) {
            for (x = uNumVerticesAlongWidth - 1; x >= 0; --x) {
                pIndices[++index] = x + (z * uNumVerticesAlongWidth);
                pIndices[++index] = x + (z * uNumVerticesAlongWidth) + uNumVerticesAlongWidth;
            }
            if (z != uNumVerticesAlongLength - 2) {
                pIndices[++index] = ++x + (z * uNumVerticesAlongWidth);
            }
        } else {
            for (x = 0; x < uNumVerticesAlongWidth; ++x) {
                pIndices[++index] = x + (z * uNumVerticesAlongWidth);
                pIndices[++index] = x + (z * uNumVerticesAlongWidth) + uNumVerticesAlongWidth;
            }
            if (z != uNumVerticesAlongLength - 2) {
                pIndices[++index] = --x + (z * uNumVerticesAlongWidth);
            }
        }
    }
}

