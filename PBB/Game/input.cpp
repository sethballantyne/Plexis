#include "exceptions.h"
#include "input.h"
#include <dinput.h>

using namespace System;
using namespace System::Runtime::InteropServices;

void Input::InitKeyboard(HWND hWnd)
{
    if(lpDIKeyboard != NULL)
    {
        lpDIKeyboard->Unacquire();
        lpDIKeyboard->Release();
        lpDIKeyboard = NULL;
    }

    currentKeyboardState = gcnew array<UCHAR, 1>(256);
    previousKeyboardState = gcnew array<UCHAR, 1>(256);
    lpDIKeyboard = NULL;

    pin_ptr<LPDIRECTINPUTDEVICE8> pinnedKeyboard = &lpDIKeyboard;

    HRESULT result = lpDI->CreateDevice(GUID_SysKeyboard, pinnedKeyboard, NULL);
    if(result != DI_OK)
    {
        switch(result)
        {
            case DIERR_DEVICENOTREG:
                throw gcnew DirectInputDeviceNotRegisteredException("IDirectInput8::CreateDevice: the device or device instance is not registered with DirectInput.");
                break;

            case DIERR_INVALIDPARAM:
                throw gcnew DirectInputInvalidParameterException("IDirectInput8::CreateDevice: an invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called.");
                break;

            case DIERR_NOINTERFACE:
                throw gcnew DirectInputNoInterfaceException("IDirectInput8::CreateDevice: the object does not support the specified interface.");
                break;

            case DIERR_NOTINITIALIZED:
                throw gcnew DirectInputNotInitializedException("IDirectInput8::CreateDevice: this object has not been initialized.");
                break;

            case DIERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("IDirectInput8::CreateDevice: the DirectInput subsystem could not allocate sufficient memory to complete the call.");
                break;

            default:
                throw gcnew COMException("IDirectInput8::CreateDevice: an unspecified", result);
                break;
        }
    }

    result = lpDIKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    if(result != DI_OK)
    {
        switch(result)
        {
            case DIERR_INVALIDPARAM:
                throw gcnew DirectInputInvalidParameterException("IDirectInputDevice8::SetCooperativeLevel: an invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called.");
                break;

            case DIERR_NOTINITIALIZED:
                throw gcnew DirectInputNotInitializedException("IDirectInputDevice8::SetCooperativeLevel: this object has not been initialized.");
                break;

            default:
                throw gcnew COMException("IDirectInputDevice8::SetCooperativeLevel: an unspecified COM+ error occured.\n", result);
                break;
        }
    }

    result = lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
    if(result != DI_OK)
    {
        switch(result)
        {
            case DIERR_ACQUIRED:
                throw gcnew DirectInputAcquiredException("IDirectInputDevice8::SetDataFormat: the operation cannot be performed while the device is acquired.");
                break;

            case DIERR_INVALIDPARAM:
                throw gcnew DirectInputInvalidParameterException("IDirectInputDevice8::SetDataFormat: an invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called.");
                break;

            case DIERR_NOTINITIALIZED:
                throw gcnew DirectInputNotInitializedException("IDirectInputDevice8::SetDataFormat: this object has not been initialized.");
                break;

            default:
                COMException("IDirectInputDevice8::SetDataFormat: an unspecified COM+ error occured.\n", result);
                break;
        }
    }

    result = lpDIKeyboard->Acquire();
    if(result != DI_OK)
    {
        switch(result)
        {
            case DIERR_INVALIDPARAM:
                throw gcnew DirectInputInvalidParameterException("IDirectInputDevice8::Acquire: an invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called.");
                break;

            case DIERR_NOTINITIALIZED:
                throw gcnew DirectInputNotInitializedException("IDirectInputDevice8::Acquire: this object has not been initialized.");
                break;

            case DIERR_OTHERAPPHASPRIO:
                throw gcnew DirectInputOtherApplicationHasPriorityException("IDirectInputDevice8::Acquire: another application has a higher priority level, preventing this call from succeeding.");
                break;

            default:
                throw gcnew System::Runtime::InteropServices::COMException("IDirectInputDevice8::Acquire failed", result);
                break;
        }
    }

    keyboardLayout = GetKeyboardLayout(0);
}

void Input::Initialise(HINSTANCE hInstance, HWND hWnd)
{
    pin_ptr<LPDIRECTINPUT8> pinnedPtr = &lpDI;
    HRESULT result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)pinnedPtr, NULL);
    if(result != DI_OK)
    {
        switch(result)
        {
            case DIERR_INVALIDPARAM:
                throw gcnew DirectInputInvalidParameterException("DirectInput8Create: an invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called.");
                break;

            case DIERR_OLDDIRECTINPUTVERSION:
                throw gcnew DirectInputOldVersionException("DirectInput8Create: the application requires a newer version of DirectInput.");
                break;

            case DIERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("DirectInput8Create: the DirectInput subsystem could not allocate sufficient memory to complete the call.");
                break;

            default:
                throw gcnew COMException("DirectInput8Create: ", result);
                break;
        }
    }

    try
    {
        if(lpDI != NULL)
        {
            InitKeyboard(hWnd);
        }
        //InitMouse(hWnd);
    }
    catch(...)
    {
        //Release();
        throw;
    }
}