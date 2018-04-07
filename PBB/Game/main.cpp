// Copyright(c) 2018 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

// Copyright(c) 2018 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "game.h"
#include "logtype.h"
#include "logmanager.h"
#include "messageboxlogger.h"
#include "version.h"
#include "vsoutputlogger.h"

#define WINDOW_CLASS_NAME L"WindowClass"

using namespace System;

HRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MAXHIDE || wParam == SIZE_MINIMIZED)
        {
            Game::IsWindowActive = false;
        }
        else
        {
            Game::IsWindowActive = true;
        }
        break;

    case WM_SYSCOMMAND:
        if (wParam == SC_MOVE || wParam == SC_SIZE || wParam == SC_MAXIMIZE || wParam == SC_MONITORPOWER)
        {
            return true;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX windowClass;
    MSG msg;
    HWND hWnd;
    DWORD lastTime = 0;

    SecureZeroMemory(&windowClass, sizeof(windowClass));
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hInstance = hInstance;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.lpszClassName = WINDOW_CLASS_NAME;
    windowClass.lpszMenuName = NULL;
    windowClass.hIconSm = NULL;
    windowClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.cbSize = sizeof(WNDCLASSEX);

    if (!RegisterClassEx(&windowClass))
    {
        MessageBox(NULL, L"Failed to register the window class.",
            L"Unholy Error", MB_OK | MB_ICONEXCLAMATION);

        return -1;
    }

    hWnd = CreateWindowEx(
        NULL,                       // extended window style
        WINDOW_CLASS_NAME,
        _T(VER_PRODUCTNAME_STR),    // window title
        WS_POPUP | WS_VISIBLE,      // window style flags
        0,                          // x position
        0,                          // y position
        400,                        // window width
        400,                        // window height
        NULL,                       // handle to parent window
        NULL,                       // menu handle
        hInstance,                  // handle to the winddows module instance,
        NULL                        // pointer to extras if needed
    );

    if(!hWnd)
    {
        MessageBox(NULL, L"CreateWindowEx failed.", L"Unholy Error", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    
    ShowWindow(hWnd, SW_SHOWNORMAL);
    
    MessageBoxLogger ^msgBoxLogger = gcnew MessageBoxLogger(hWnd);
    LogManager::Add(LogType::Error, msgBoxLogger);

#ifdef _DEBUG
    VSOutputLogger ^vsOutputLogger = gcnew VSOutputLogger();
    LogManager::Add(LogType::Debug | LogType::Error | LogType::Log, vsOutputLogger);
#endif

    LogManager::Write(LogType::Error, "test!");
    try
    {
        Game::Initialise(hInstance, hWnd);

        while(Game::IsRunning)
        {
            if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if(msg.message != WM_QUIT)
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else
                {
                    Game::IsRunning = false;
                }
            }

            // only update the game when the window is active
            if(Game::IsWindowActive)
            {
                Game::Update();
                Game::Render();
            }
        }
    }
    catch(Exception ^e)
    {
        LogManager::WriteLine(LogType::Error | LogType::Debug,
            "Fatal Error: {0}\n\nStack trace: {1}", e->Message, e->StackTrace);
    }
    finally
    {
        Game::Shutdown();
    }

    return msg.wParam;
}