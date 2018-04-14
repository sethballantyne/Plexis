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
#pragma once

#include <windows.h>
#include <dinput.h>
#include "keys.h"
#include "logmanager.h"

using namespace System;

/// <summary>
/// 
/// </summary>
public ref class Input abstract sealed
{
private:
    static LPDIRECTINPUT8 lpDI;

    static LPDIRECTINPUTDEVICE8 lpDIKeyboard;
    static LPDIRECTINPUTDEVICE8 lpDIMouse;

    /*static UCHAR previousKeyboardState[256];
    static UCHAR currentKeyboardState[256];*/
    static array<UCHAR, 1> ^previousKeyboardState;
    static array<UCHAR, 1> ^currentKeyboardState;

    static HKL keyboardLayout;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="hWnd"></param>
    static void InitKeyboard(HWND hWnd);
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="hWnd"></param>
    static void Initialise(HINSTANCE hInstance, HWND hWnd);

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    static Keys ^ReadKeyboard()
    {
        currentKeyboardState->CopyTo(previousKeyboardState, 0);

        pin_ptr<UCHAR> pinnedCurrentKeyboardState = &currentKeyboardState[0];
        lpDIKeyboard->GetDeviceState(currentKeyboardState->Length, (LPVOID)pinnedCurrentKeyboardState);

        return gcnew Keys(currentKeyboardState, previousKeyboardState);
    }
};