#include <windows.h>
#include "litapi.h"        // Your header file
#include <d2d1.h>
#include <dwrite.h>

// This is the Direct2D-based draw callback.
void MyDrawCallback(ID2D1HwndRenderTarget* pRenderTarget, int width, int height) {
    // Create a brush for drawing text
    ID2D1SolidColorBrush* pTextBrush = NULL;
    HRESULT hr = pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White), 
        &pTextBrush
    );

    if (SUCCEEDED(hr)) {
        // Create a text format for our "Hello World" text
        IDWriteTextFormat* pTextFormat = NULL;
        IDWriteFactory* pDWriteFactory = NULL;
        
        // The LitAPI library should have already created the global DirectWrite factory.
        // We just need to get a reference to it. (Note: Your library needs to expose this for this to work)
        // For now, let's assume the user has to create it, which is less ideal but works.
        DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory)
        );
        
        if (pDWriteFactory) {
            hr = pDWriteFactory->CreateTextFormat(
                L"Segoe UI",
                NULL,
                DWRITE_FONT_WEIGHT_BOLD,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                24.0f,
                L"en-us",
                &pTextFormat
            );
            
            if (SUCCEEDED(hr)) {
                pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                
                // Draw "Hello World" text in the center of the window
                D2D1_RECT_F textRect = D2D1::RectF(0, TITLE_BAR_HEIGHT, width, height);
                
                pRenderTarget->DrawText(
                    L"Hello World",
                    11,
                    pTextFormat,
                    textRect,
                    pTextBrush
                );
            }
            
            // Release the resources we created for this specific draw call
            SafeRelease(&pTextFormat);
        }
        // The DWriteFactory is a global resource, so we might not release it here
        // depending on how LitAPI is designed. For this example, we will.
        SafeRelease(&pDWriteFactory);
    }

    // ALWAYS release the temporary brush resource!
    SafeRelease(&pTextBrush);
}

LRESULT MyInputCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE) {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        return 1; 
    }
    return 0; 
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize the API. This function is now inside the DLL.
    LitAPI_Init(hInstance);

    // Create the main window. This function is now inside the DLL.
    LitWindow* mainWindow = LitWindow_Create("Example", 800, 600, RGB(45, 45, 48));
    if (!mainWindow) {
        return -1;
    }

    // Set the callbacks. These functions are now inside the DLL.
    LitWindow_SetDrawCallback(mainWindow, MyDrawCallback);
    LitWindow_SetInputCallback(mainWindow, MyInputCallback);

    // Show the window. This function is now inside the DLL.
    LitWindow_Show(mainWindow, nCmdShow);

    // Run the message loop. This function is now inside the DLL.
    int result = LitAPI_MessageLoop();

    // Clean up all resources. This function is now inside the DLL.
    LitAPI_Cleanup();

    return result;
}