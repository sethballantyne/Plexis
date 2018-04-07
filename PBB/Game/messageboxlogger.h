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
#include "ilog.h"

using namespace System;

/// <summary>
/// Displays message on the screen via a call to the win32 function MessageBox. 
/// MessageBoxLogger is designed to act as a subscriber to LogManager.
/// </summary>
public ref class MessageBoxLogger : public ILog
{
private:

    // the handle the displayed message boxes will belong to.
    HWND hWnd;
public:

    /// <summary>
    /// Initialises the object with the handle of the game window.
    /// </summary>
    /// <param name="hWnd">the window handle the message box will belong to.</param>
    /// <exception cref="System::ArgumentNullException">hWnd isn't a valid window handle.</exception>
    MessageBoxLogger(HWND hWnd);
    
    /// <summary>
    /// Displays a MessageBox with the specified message.
    /// </summary>
    /// <param name="message"></param>
    void virtual Write(String ^message);

    /// <summary>
    /// Displays a MessageBox with the specified message. Effectively the same as Write.
    /// </summary>
    /// <param name="message"></param>
    void virtual WriteLine(String ^message);

};