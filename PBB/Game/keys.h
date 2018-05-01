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

#include <dinput.h>
#include <windows.h>

using namespace System;

///<summary>
///encapsulates a snapshot of the keyboard state.
///</summary>
public ref class Keys
{
private:
    DWORD firstKey;

    array<UCHAR, 1> ^currentKeyboardState;
    array<UCHAR, 1> ^previousKeyboardState;

public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="currentKeyboardState"></param>
    /// <param name="previousKeyboardState"></param>
    Keys(array<UCHAR, 1> ^currentKeyboardState, array<UCHAR, 1> ^previousKeyboardState);

    /// <summary>
    /// returns true if the specified key is pressed down, otherwise returns false.
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    bool KeyDown(UCHAR key)
    {
        return this->currentKeyboardState[key] & 0x80;
    }

    /// <summary>
    /// returns true if the specified key has been pressed (pressed down and then released), otherwise
    /// returns false.
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    bool KeyPressed(UCHAR key)
    {
        if(previousKeyboardState == nullptr)
        {
            return false;
        }

        bool currentKeyState = this->currentKeyboardState[key] & 0x80;
        bool previousKeyState = this->previousKeyboardState[key] & 0x80;

        // key pressed if the currentState is false (key is up) and previous is true (key is down)
        return ((currentKeyState == false) && (previousKeyState == true));
    }

    /// <summary>
    /// returns the current pressed key as a DirectInput scancode. 
    /// If there are multiple keys pressed, you're only getting one!
    /// </summary>
    property DWORD Key
    {
        DWORD get()
        {
            return this->firstKey;
        }
    }

    /// <summary>
    /// Returns the last key that was pressed, or -1 if no key was pressed.
    /// </summary>
    property int PressedKey
    {
        int get()
        {
            for(int i = 0; i < 256; i++)
            {
                if(KeyPressed(i))
                {
                    return i;
                }
            }

            return -1;
        }
    }
};
