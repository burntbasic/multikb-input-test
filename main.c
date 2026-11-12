#include <stdlib.h>
#include <stdbool.h>
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

    //Raw Input Device List Enumeration
    UINT deviceCount = 0;
    PRAWINPUTDEVICELIST pRawInputDeviceList = NULL;

    while(true) {
        //First call: stores deviceCount value, success returns 0
        if(GetRawInputDeviceList(NULL, &deviceCount, sizeof(RAWINPUTDEVICELIST)) != 0) {
            return EXIT_FAILURE; //Error();
        }
        
        if(deviceCount == 0) { break; } //no devices detected, array allocation does not need to happen
        
        if((pRawInputDeviceList = malloc(sizeof(RAWINPUTDEVICELIST) * deviceCount)) == NULL) {
            return EXIT_FAILURE; //Error();
        } 

        //Second call: stores(in deviceCount) and returns(to deviceReturn) deviceCount value, if error returns (UINT)-1
        UINT deviceReturn = GetRawInputDeviceList(pRawInputDeviceList, &deviceCount, sizeof(RAWINPUTDEVICELIST));

        if (deviceReturn == (UINT)-1) {
            if(GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                free(pRawInputDeviceList);
                return EXIT_FAILURE; //Error();
            }
            free(pRawInputDeviceList);
            continue;
        }
        break;
    }

    HANDLE *pDeviceKb = NULL;

    if (deviceCount > 0) {
        //Keyboard count 
        int kbCount = 0;

        for(int i=0; i<deviceCount; i++) {
            if(pRawInputDeviceList[i].dwType == RIM_TYPEKEYBOARD) {
                kbCount++;       
            }
        }

        if (kbCount > 0) {
            if((pDeviceKb = malloc(kbCount * sizeof(HANDLE))) == NULL) { //malloc() calculates size in bytes
                free(pRawInputDeviceList);
                return EXIT_FAILURE;
            }  
            int kbIndex = 0;

            for(int i=0; i<deviceCount; i++) {
                if(pRawInputDeviceList[i].dwType == RIM_TYPEKEYBOARD) {
                    pDeviceKb[kbIndex] = pRawInputDeviceList[i].hDevice;
                    kbIndex++;
                }
            }
        } else {
            MessageBox(window, L"No Keyboards Found.", NULL, MB_OK);
        }
    } else {
        MessageBox(window, L"No Devices Found", NULL, MB_OK);
    }

    free(pRawInputDeviceList);

    //Once no longer needed run:
    //free(pDeviceKb);

    ShowWindow(window, nShowCmd);
    UpdateWindow(window);

    //Window Message Handling
    MSG msg = {0}; //msg structure declared for GetMessage() pointer, init all members to 0

    while(true){
        int getMsg = GetMessage(&msg, NULL, 0, 0);
        if (getMsg == 0) { //WM_QUIT
            return EXIT_SUCCESS;
        }
        
        if (getMsg == -1) {
            return EXIT_FAILURE; //Error();
        }
        
        if (getMsg > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }  
    }    
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam) {
    
    if (uInt == WM_DESTROY) {
        PostQuitMessage(0); //adds WM_QUIT message to queue
        return 0;
    }
    
    return DefWindowProc(hWnd, uInt, wParam, lParam); //Default WndProc handling
}