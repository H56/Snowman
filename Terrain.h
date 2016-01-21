#pragma once
class Terrain : public RenderObj {
private:

    float m_fCellsSpace;
    float m_fHeightScale;
    float m_fTerrainWidth;
    float m_fTerrainDepth;
    float m_fMinDistance;

    bool  m_bRenderFrame;

    UINT  m_uNumVertices;
    UINT  m_uNumIndices;


    std::vector<BYTE>  m_vHeightData;

    std::string m_sTextureFile;
    std::string m_sTerrainFile;
    LPDIRECT3DINDEXBUFFER9	m_pIndexBuffer;

    //struct TerrainVertex {
    //    float x, y, z;
    //    float tu, tv;
    //    TerrainVertex(float x, float y, float z, float tu, float tv) : x(x), y(y), z(z), tu(tu),
    //        tv(tv) {
    //    }

    //    enum FVF {
    //        FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
    //    };
    //};

    float NormalizeDistance(float x);

public:
    Terrain(LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, std::string m_sTextureFile = "terrain\\terrainTex.jpg", std::string m_sTerrainFile = "terrain\\height.raw");
    ~Terrain();
    void SetSize(float fWidth, float fLength, float fHeight);
    D3DXVECTOR3 ComputePositionInMySurface(float x, float y, float z);
    D3DXVECTOR3 Terrain::ComputePositionInMySurface(D3DXVECTOR3 position) {
        return ComputePositionInMySurface(position.x, position.y, position.z);
    }
    void Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId = 0);
    void RestoreDeviceObjects(void);
    void LoadTerrainFromFile(LPCSTR pSrcFile);
    void LoadTexture() {
        m_pTexture = RenderObj::LoadTexture(m_pd3dDevice, m_sTextureFile.c_str());
    }
    void InvalidateDeviceObjects(void);
    UINT GetIndicesNum(UINT uNumVerticesAlongWidth, UINT uNumVerticesAlongLength) {
        return (uNumVerticesAlongWidth * 2) * (uNumVerticesAlongLength - 1) + (uNumVerticesAlongLength - 2);
    }
    void GenerateTerrainVertex(Vertex* pVertices, BYTE* pHeight, UINT uNumVerticesAlongWidth, UINT uNumVerticesAlongLength);
    void GenerateIndices(int* pIndices, UINT uNumVerticesAlongWidth, UINT uNumVerticesAlongLength);
};

