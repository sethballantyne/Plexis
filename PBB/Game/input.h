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
#include "mouse.h"

using namespace System;

/// <summary>
/// Wrapper around DirectInput. Input::Initalise() must be called before any of its other methods can be called.
/// In order to release the resources allocated by Initialise(), call Input::Shutdown(). Once Shutdown has been
/// called, no methods can be used until the class is initialised again.
/// </summary>
public ref class Input abstract sealed
{
private:
    static LPDIRECTINPUT8 lpDI;

    static LPDIRECTINPUTDEVICE8 lpDIKeyboard;
    static LPDIRECTINPUTDEVICE8 lpDIMouse;

    // the previous state of the keyboard is stored because it's required when
    // checking to see if a key has been pressed: previous state to see if it was down the last time
    // the keyboard state was retrieved and the current state to see if it's now up.
    static array<UCHAR, 1> ^previousKeyboardState;
    static array<UCHAR, 1> ^currentKeyboardState;

    static HKL keyboardLayout;

    /// <summary>
    /// Requests that DirectInput have access to the specified device.
    /// </summary>
    /// <param name="lpDIDevice">the device to request access to.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException"></exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNotInitializedException">the object has not been initialized.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    static void AcquireDevice(LPDIRECTINPUTDEVICE8 lpDIDevice);

    /// <summary>
    /// Puts the keyboard into a useable state.
    /// </summary>
    /// <param name="hWnd">handle of the window that will be capturing input.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectINput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputAcquiredException">the operation cannot be performed while the device is acquired.</exception>
    /// <exception cref="DirectInputDeviceNotRegisteredException">the device or device instance is not registered with DirectInput.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNoInterfaceException">the object does not support the specified interface.</exception>
    /// <exception cref="DirectInputNotInitializedException">this object has not been initialized.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    /// <exception cref="OutOfMemoryException">not enough memory available to complete the operation.</exception>
    static void InitKeyboard(HWND hWnd);

    /// <summary>
    /// Puts the mouse into a useable state.
    /// </summary>
    /// <param name="hWnd">handle of the window that will be capturing input.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectINput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputAcquiredException">the operation cannot be performed while the device is acquired.</exception>
    /// <exception cref="DirectInputDeviceNotRegisteredException">the device or device instance is not registered with DirectInput.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNoInterfaceException">the object does not support the specified interface.</exception>
    /// <exception cref="DirectInputNotInitializedException">this object has not been initialized.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    /// <exception cref="System::OutOfMemoryException">not enough memory available to complete the operation.</exception>
    static void InitMouse(HWND hWnd);

    /// <summary>
    /// Releases the sppecified device. Once this is called, the device is unavailable for use.
    /// </summary>
    /// <param name="device">the DirectInput device to release.</param>
    static void SafeRelease(LPDIRECTINPUTDEVICE8 device)
    {
        if(device != NULL)
        {
            device->Unacquire();
            device->Release();
            device = NULL;
        }
    }
public:
    /// <summary>
    /// Initialises DirectInput and puts the mouse and keyboard into a useable state. Initialise() must be called before
    /// any other methods in the Input class. Once you're done with the Input class, call Shutdown() to release the resources
    /// allocated by Initialise().
    /// </summary>
    /// <param name="hInstance">the instance handle of the application.</param>
    /// <param name="hWnd">the handle of the window that'll be capturing input.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectINput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputAcquiredException">the operation cannot be performed while the device is acquired.</exception>
    /// <exception cref="DirectInputDeviceNotRegisteredException">the device or device instance is not registered with DirectInput.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNoInterfaceException">the object does not support the specified interface.</exception>
    /// <exception cref="DirectInputNotInitializedException">this object has not been initialized.</exception>
    /// <exception cref="DirectInputOldVersionException">DirectX 8 required.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    /// <exception cref="System::OutOfMemoryException">not enough memory available to complete the operation.</exception>
    static void Initialise(HINSTANCE hInstance, HWND hWnd);

    /// <summary>
    /// Captures the current state of the keys on the keyboard.
    /// </summary>
    /// <returns></returns>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectInput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputDeviceLostException">access to the input device has been lost. It must be reacquired.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed to the function, or the object is an invalid state.</exception>
    /// <exception cref="DirectInputDeviceNotAcquiredException">attempting to use the device when it hasn't been acquired.</exception>
    /// <exception cref="DirectInputNotInitializedException">object hasn't been initialised.</exception>
    static Keys ^ReadKeyboard();

    /// <summary>
    /// Captures the current state of the mouse and its buttons.
    /// </summary>
    /// <returns></returns>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectInput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputDeviceLostException">access to the input device has been lost. It must be reacquired.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed to the function, or the object is an invalid state.</exception>
    /// <exception cref="DirectInputDeviceNotAcquiredException">attempting to use the device when it hasn't been acquired.</exception>
    /// <exception cref="DirectInputNotInitializedException">object hasn't been initialised.</exception>
    static Mouse ^ReadMouse();

    static void Release()
    {
        SafeRelease(lpDIKeyboard);
        SafeRelease(lpDIMouse);

        if(lpDI != NULL)
        {
            lpDI->Release();
            lpDI = NULL;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <exception cref="System::Runtime::InteropServices::COMException"></exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNotInitializedException">the object has not been initialized.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    static void Restore()
    {
        try
        {
            AcquireDevice(lpDIKeyboard);
            AcquireDevice(lpDIMouse);
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Shutsdown DirectInput and releases the resources allocated by Initialise(). Once this method is called,
    /// no methods belonging to Input can be called until it's initialised again.
    /// </summary>
    static void Shutdown()
    {
        Release();
    }

};