#pragma once
#include <d3d9.h>
#include <d3dx9.h>
class Cube {
private:
    struct Vertex {
        float x, y, z;
        float tu, tv;
        enum FVF {
            FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
        };
    };
    struct Color {
        DWORD color;
    };

    bool  m_bBlending = true;
    float m_fDistance = 4.5f;
    float m_fSpinX = 0.0f;
    float m_fSpinY = 0.0f;

    LPDIRECT3D9             m_pD3D;
    LPDIRECT3DDEVICE9       m_pd3dDevice;
    LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
    LPDIRECT3DTEXTURE9      m_pTexture;
    HWND                    m_hWnd;

    Vertex m_cubeVertices[24] = {
        { -1.0f,  1.0f, -1.0f,  0.0f,  0.0f },
        {  1.0f,  1.0f, -1.0f,  1.0f,  0.0f },
        { -1.0f, -1.0f, -1.0f,  0.0f,  1.0f },
        {  1.0f, -1.0f, -1.0f,  1.0f,  1.0f },

        { -1.0f,  1.0f,  1.0f,  1.0f,  0.0f },
        { -1.0f, -1.0f,  1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f,  0.0f,  0.0f },
        {  1.0f, -1.0f,  1.0f,  0.0f,  1.0f },

        { -1.0f,  1.0f,  1.0f,  0.0f,  0.0f },
        {  1.0f,  1.0f,  1.0f,  1.0f,  0.0f },
        { -1.0f,  1.0f, -1.0f,  0.0f,  1.0f },
        {  1.0f,  1.0f, -1.0f,  1.0f,  1.0f },

        { -1.0f, -1.0f,  1.0f,  0.0f,  1.0f },
        { -1.0f, -1.0f, -1.0f,  0.0f,  0.0f },
        {  1.0f, -1.0f,  1.0f,  1.0f,  1.0f },
        {  1.0f, -1.0f, -1.0f,  1.0f,  0.0f },

        {  1.0f,  1.0f, -1.0f,  0.0f,  0.0f },
        {  1.0f,  1.0f,  1.0f,  1.0f,  0.0f },
        {  1.0f, -1.0f, -1.0f,  0.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f,  1.0f,  1.0f },

        { -1.0f,  1.0f, -1.0f,  1.0f,  0.0f },
        { -1.0f, -1.0f, -1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f,  0.0f,  0.0f },
        { -1.0f, -1.0f,  1.0f,  0.0f,  1.0f }
    };
public:
    Cube(LPDIRECT3D9 pD3D = NULL, HWND hWnd = NULL);
    ~Cube();

    LPDIRECT3DTEXTURE9 loadTexture(LPCSTR pSrcFile = "radiation_box.tga", LPDIRECT3DDEVICE9 pd3dDevice = NULL);
};

