#include "stdafx.h"
#include "resource.h"

#include <string>
#include <memory>

#include "MainClass.h"



std::shared_ptr<Renderer> MainClass::m_renderer;
//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
MainClass::MainClass() {
    m_windowClassName = "Direct3DWindowClass";
    m_hInstance = NULL;
}

//-----------------------------------------------------------------------------
// Create a window for our Direct3D viewport.
//-----------------------------------------------------------------------------
HRESULT MainClass::CreateDesktopWindow() {
    // Window resources are dealt with here.

    if (m_hInstance == NULL)
        m_hInstance = (HINSTANCE)GetModuleHandle(NULL);

    HICON hIcon = LoadIcon(m_hInstance, "codesampler.ico");
    TCHAR szExePath[MAX_PATH];
    GetModuleFileName(NULL, szExePath, MAX_PATH);

    // If the icon is NULL, then use the first one found in the exe
    if (hIcon == NULL)
        hIcon = ExtractIcon(m_hInstance, szExePath, 0);

    // Register the windows class
    WNDCLASS wndClass;
    wndClass.style = CS_DBLCLKS;
    wndClass.lpfnWndProc = MainClass::StaticWindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = m_hInstance;
    wndClass.hIcon = hIcon;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = m_windowClassName;

    if (!RegisterClass(&wndClass)) {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
            return HRESULT_FROM_WIN32(dwError);
    }

    m_rc;
    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;

    // No menu in this example.
    m_hMenu = NULL;

    // This example uses a non-resizable 1366 by 768 viewport for simplicity.
    int nDefaultWidth = 1366;
    int nDefaultHeight = 768;
    SetRect(&m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
    AdjustWindowRect(
        &m_rc,
        WS_OVERLAPPEDWINDOW,
        (m_hMenu != NULL) ? true : false
        );

    // Create the window for our viewport.
    m_hWnd = CreateWindow(
        m_windowClassName,
        "Snowman",
        WS_OVERLAPPEDWINDOW,
        x, y,
        (m_rc.right - m_rc.left), (m_rc.bottom - m_rc.top),
        0,
        m_hMenu,
        m_hInstance,
        0
        );

    if (m_hWnd == NULL) {
        DWORD dwError = GetLastError();
        return HRESULT_FROM_WIN32(dwError);
    }

    return S_OK;
}

HRESULT MainClass::Run(std::shared_ptr<DeviceResources> deviceResources,std::shared_ptr<Renderer> renderer) {
    HRESULT hr = S_OK;
    m_renderer = renderer;

    if (!IsWindowVisible(m_hWnd))
        ShowWindow(m_hWnd, SW_SHOW);

    // The render loop is controlled here.
    bool bGotMsg;
    MSG  msg;
    msg.message = WM_NULL;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    while (WM_QUIT != msg.message) {
        // Process window events.
        // Use PeekMessage() so we can use idle time to render the scene. 
        bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

        if (bGotMsg) {
            // Translate and dispatch the message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Update the scene.
            renderer->Update();

            // Render frames during idle time (when no messages are waiting).
            renderer->Render();

            // Present the frame to the screen.
            //deviceResources->Present();
        }
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Destructor.
//-----------------------------------------------------------------------------
MainClass::~MainClass() {

}

//-----------------------------------------------------------------------------
// Process windows messages. This looks for window close events, letting us
// exit out of the sample.
//-----------------------------------------------------------------------------
LRESULT CALLBACK MainClass::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static POINT ptLastMousePosit;
    static POINT ptCurrentMousePosit;
    static bool bMousing;

    switch (uMsg) {
    case WM_CLOSE:{
        HMENU hMenu;
        hMenu = GetMenu(hWnd);
        if (hMenu != NULL) {
            DestroyMenu(hMenu);
        }
        DestroyWindow(hWnd);
        UnregisterClass(m_windowClassName, m_hInstance);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN: {
        switch (wParam) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        default:
            break;
        } break;
    case WM_LBUTTONDOWN: {
        ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD(lParam);
        ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD(lParam);
        bMousing = true;
    } break;

    case WM_LBUTTONUP: {
        bMousing = false;
    } break;

    case WM_MOUSEWHEEL: {
        if ((INT)wParam > 0) {
            m_renderer->UpdateDistance(-0.1f);
        } else {
            m_renderer->UpdateDistance(0.1f);
        }
    } break;

    case WM_MOUSEMOVE: {
        ptCurrentMousePosit.x = LOWORD(lParam);
        ptCurrentMousePosit.y = HIWORD(lParam);
        if (bMousing) {
            m_renderer->UpdateSpinPosition(ptLastMousePosit.x - ptCurrentMousePosit.x, ptLastMousePosit.y - ptCurrentMousePosit.y);
        }
        ptLastMousePosit.x = ptCurrentMousePosit.x;
        ptLastMousePosit.y = ptCurrentMousePosit.y;
    }

    }

    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}