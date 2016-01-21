#pragma once
class RenderObj {
protected:
    struct Vertex {
        D3DXVECTOR3 pos;
        D3DXVECTOR3 norm;
        float tu;
        float tv;
        enum FVF {
            FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
        };
    };
    struct Color {
        DWORD color;
    };

    bool  m_bBlending = true;

    LPDIRECT3DDEVICE9       m_pd3dDevice = NULL;
    LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer = NULL;
    LPDIRECT3DTEXTURE9      m_pTexture = NULL;
    LPD3DXMESH              m_pMesh = NULL;
    D3DMATERIAL9            m_d3dMaterial;

    D3DXMATRIX m_d3dmTranslate;
    D3DXMATRIX m_d3dmRotate;
    D3DXMATRIX m_d3dmScale;
    D3DXMATRIX m_d3dmTransform;

    D3DXMATRIX m_d3dmFace;

    float m_fRotationX, m_fRotationY, m_fRotationZ;

public:
    RenderObj(float x, float y, float z);
    virtual ~RenderObj();

    virtual void Render(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwAttribId = 0) = 0;
    virtual void RestoreDeviceObjects(void) = 0;

    virtual void InvalidateDeviceObjects(void);

    virtual void UpdateDistance(float step = 0.1f) {
        m_d3dmTranslate._43 += step;
    }
    virtual void SetBlend(bool bStatus) {
        this->m_bBlending = bStatus;
    }
    virtual void Reset();
    virtual D3DXVECTOR3 ComputePositionInMySurface(float x, float y, float z);

    D3DXVECTOR3 ComputePositionInMySurface(D3DXVECTOR3 position) {
        return ComputePositionInMySurface(position.x, position.y, position.z);
    }

    LPDIRECT3DTEXTURE9 LoadTexture(LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR pSrcFile);
    void Render() {
        this->Render(this->m_pd3dDevice);
    }
    void Render(DWORD dwAttribId) {
        this->Render(this->m_pd3dDevice, dwAttribId);
    }
    
    void AbsoluteTranslate(float x, float y, float z);
    void RelativeTranslate(float x, float y, float z);
    void AbsoluteRotate(float x, float y, float z);
    void RelativeRotate(float x, float y, float z);
    void AbsoluteScale(float x, float y, float z);
    void RelativeScale(float x, float y, float z);
    void SpinXY(float shiftX, float shitfY) {
        RelativeRotate(shiftX, shitfY, 0.0f);
    }
    void MoveAlongVector(D3DXVECTOR3 direction, float step);

    D3DXMATRIX* GetTransform() {
        m_d3dmTransform = m_d3dmScale * m_d3dmRotate * m_d3dmTranslate;
        return &m_d3dmTransform;
    }
    D3DXVECTOR3 Rotation(D3DXVECTOR3 target);
    void SetPosition(float x, float y, float z) {
        AbsoluteTranslate(x, y, z);
    }
    void SetPosition(D3DXVECTOR3 position) {
        AbsoluteTranslate(position.x, position.y, position.z);
    }
    void SetXPosition(float x) {
        m_d3dmTranslate._41 = x;
    }
    void SetYPosition(float y) {
        m_d3dmTranslate._42 = y;
    }
    void SetZPosition(float z) {
        m_d3dmTranslate._43 = z;
    }
    void SetXRotation(float x) { 
        AbsoluteRotate(x, m_fRotationY, m_fRotationZ);
    }
    void SetYRotation(float y) { 
        AbsoluteRotate(m_fRotationX, y, m_fRotationZ);
    }
    void SetZRotation(float z) { 
        AbsoluteRotate(m_fRotationX, m_fRotationY, z);
    }
    void SetXScale(float x) { 
        m_d3dmScale._11 = x;
    }
    void SetYScale(float y) { 
        m_d3dmScale._22 = y;
    }
    void SetZScale(float z) { 
        m_d3dmScale._33 = z;
    }

    D3DXVECTOR3 GetPosition() {
        return D3DXVECTOR3(m_d3dmTranslate._41, m_d3dmTranslate._42, m_d3dmTranslate._43);
    }
};

