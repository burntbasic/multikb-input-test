#include <stdlib.h>
#define UNICODE
#include <windows.h>

//Declaration
LRESULT CALLBACK WndProc(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam);

//Main
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
    
    WNDCLASS wc = {0}; //window class structure variable, init all members to 0

    wc.lpfnWndProc = WndProc; //assign WndProc function to pointer
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Raw Input Test Window";

    if(RegisterClass(&wc)==0){
        GetLastError();
        return EXIT_FAILURE;
    }

    HWND window = CreateWindowEx(0, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, hInstance, NULL);
    
    if(window==NULL) {
        GetLastError();
        return EXIT_FAILURE;
    }

    ShowWindow(window, nShowCmd);
    UpdateWindow(window);

    MSG msg = {0}; //msg structure declared for GetMessage() pointer, init all members to 0

    for (int getMsg = GetMessage(&msg, NULL, 0, 0);;) {
        if (getMsg == 0) { //WM_QUIT
            return EXIT_SUCCESS;
        }

        if (getMsg == -1) {
            return EXIT_FAILURE;
        }

        if (getMsg > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        getMsg = GetMessage(&msg, NULL, 0, 0);
    }
    
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam) {
    
    if (uInt == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(hWnd, uInt, wParam, lParam); //Default WndProc handling
}