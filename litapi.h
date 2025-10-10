#ifndef LITAPI_H
#define LITAPI_H

#include <windows.h>
#include <stdbool.h>
#include <d2d1.h>

// --- Public API Constants ---
#define TITLE_BAR_HEIGHT 30
#define BORDER_WIDTH 5
#define BUTTON_WIDTH 45
#define CORNER_RADIUS 10

// Forward declarations
struct ID2D1HwndRenderTarget;
typedef struct LitWindow LitWindow;

// This macro handles DLL import/export for cross-platform compatibility.
#ifdef LITAPI_EXPORTS
#define LITAPI_API __declspec(dllexport)
#else
#define LITAPI_API __declspec(dllimport)
#endif

// This block ensures the functions can be called from both C and C++ code.
#ifdef __cplusplus
extern "C" {
#endif

// --- Callbacks ---
// The draw callback provides a Direct2D render target for hardware-accelerated drawing.
typedef void (*LitWindow_DrawCallback)(struct ID2D1HwndRenderTarget* pRenderTarget, int width, int height);
typedef LRESULT(*LitWindow_InputCallback)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// --- Core API Functions ---
LITAPI_API void LitAPI_Init(HINSTANCE hInstance);
LITAPI_API LitWindow* LitWindow_Create(const char* title, int width, int height, COLORREF bgColor);
LITAPI_API void LitWindow_Show(LitWindow* window, int nCmdShow);
LITAPI_API HWND LitWindow_GetHWND(LitWindow* window);
LITAPI_API int LitAPI_MessageLoop(void);
LITAPI_API void LitAPI_Cleanup(void);
LITAPI_API void LitWindow_Destroy(LitWindow* window);

// --- Window Utility Functions ---
LITAPI_API void LitWindow_SetTitle(LitWindow* window, const char* title);
LITAPI_API void LitWindow_SetBackgroundColor(LitWindow* window, COLORREF color);
LITAPI_API void LitWindow_Invalidate(LitWindow* window);

// --- Callback Setters ---
LITAPI_API void LitWindow_SetDrawCallback(LitWindow* window, LitWindow_DrawCallback callback);
LITAPI_API void LitWindow_SetInputCallback(LitWindow* window, LitWindow_InputCallback callback);

// --- Obsolete Functions ---
// These are kept for compatibility but do nothing in the Direct2D context.
LITAPI_API HDC LitAPI_GetDC(LitWindow* window);
LITAPI_API void LitWindow_ReleaseDC(LitWindow* window, HDC hdc);

#ifdef __cplusplus
}
#endif

// A helper template for safely releasing COM objects (Direct2D/DirectWrite).
template <typename T>
inline void SafeRelease(T** ppT) {
    if (*ppT) {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

#endif // LITAPI_H