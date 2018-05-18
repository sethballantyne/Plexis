#include "exceptions.h"
#include "input.h"
#include <dinput.h>

using namespace System;
using namespace System::Runtime::InteropServices;

void Input::AcquireDevice(LPDIRECTINPUTDEVICE8 lpDIDevice)
{
    if(lpDIDevice != NULL)
    {
        HRESULT result = lpDIDevice->Acquire();
        if(result != DI_OK)
        {
            switch(result)
            {
                case DIERR_INVALIDPARAM:
                    throw gcnew DirectInputInvalidParameterException("IDirectInputDevice8::Acquire: an invalid parameter was passed or the object wasn't in a callable state (null).");
                    break;

                case DIERR_NOTINITIALIZED:
                    throw gcnew DirectInputNotInitializedException("IDirectInputDevice8::Acquire: this object has not been initialized.");
                    break;

                case DIERR_OTHERAPPHASPRIO:
                    throw gcnew DirectInputOtherApplicationHasPriorityException("IDirectInputDevice8::Acquire: another application has a higher priority level, preventing this call from succeeding.");
                    break;

                default:
                    throw gcnew COMException("IDirectInputDevice8::Acquire failed.", result);
                    break;
            }
        }
    }
}
void Input::InitKeyboard(HWND hWnd)
{
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
                throw gcnew COMException("IDirectInput8::CreateDevice failed.", result);
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
                COMException("IDirectInputDevice8::SetDataFormat: an unspecified COM+ error occured.", result);
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
                throw gcnew COMException("IDirectInputDevice8::Acquire failed.", result);
                break;
        }
    }

    keyboardLayout = GetKeyboardLayout(0);
}

void Input::InitMouse(HWND hWnd)
{
    if(lpDIMouse != NULL)
    {
        lpDIMouse->Unacquire();
        lpDIMouse->Release();
        lpDIMouse = NULL;
    }

    pin_ptr<LPDIRECTINPUTDEVICE8> pinnedMouse = &lpDIMouse;
    HRESULT result = lpDI->CreateDevice(GUID_SysMouse, pinnedMouse, NULL);
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
                throw gcnew COMException("IDIrectInput8::CreateDevice failed.\n", result);
                break;
        }
    }

    result = lpDIMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
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
                throw gcnew COMException("IDirectInputDevice8::SetCooperativeLevel: ", result);
                break;
        }
    }

    result = lpDIMouse->SetDataFormat(&c_dfDIMouse2);
    if(result != DI_OK)
    {
        switch(result)
        {
            case DIERR_ACQUIRED:
                throw gcnew DirectInputAcquiredException("IDirectInputDevice8::SetDataFormat: ");
                break;

            case DIERR_INVALIDPARAM:
                throw gcnew DirectInputInvalidParameterException("IDirectInputDevice8::SetDataFormat: an invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called.");
                break;

            case DIERR_NOTINITIALIZED:
                throw gcnew DirectInputNotInitializedException("IDirectInputDevice8::SetDataFormat: this object has not been initialized.");
                break;

            default:
                throw gcnew COMException("IDirectInputDevice8::SetDataFormat: ", result);
                break;
        }
    }

    result = lpDIMouse->Acquire();
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
                throw gcnew COMException("IDirectInputDevice8::Acquire failed.", result);
                break;
        }
    }
}

void Input::Initialise(HINSTANCE hInstance, HWND hWnd)
{
    InitLookupTable();

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
                throw gcnew COMException("DirectInput8Create failed.", result);
                break;
        }
    }

    try
    {
        if(lpDI != NULL)
        {
            InitKeyboard(hWnd);
            InitMouse(hWnd);
        }
        
    }
    catch(...)
    {
        //Release();
        throw;
    }
}

Keys ^Input::ReadKeyboard()
{
    currentKeyboardState->CopyTo(previousKeyboardState, 0);

    pin_ptr<UCHAR> pinnedCurrentKeyboardState = &currentKeyboardState[0];
    HRESULT result = lpDIKeyboard->GetDeviceState(currentKeyboardState->Length, (LPVOID)pinnedCurrentKeyboardState);
    if(result != DI_OK)
    {
        switch(result)
        {
            case DIERR_INPUTLOST:
                throw gcnew DirectInputDeviceLostException("IDirectInputDevice8::GetDeviceState: access to the input device has been lost. It must be reacquired.");
                break;

            case DIERR_INVALIDPARAM:
                throw gcnew DirectInputInvalidParameterException("IDirectInputDevice8::GetDeviceState: an invalid parameter was passed to the function, or the object is an invalid state.");
                break;

            case DIERR_NOTACQUIRED:
                throw gcnew DirectInputDeviceNotAcquiredException("IDirectInputDevice8::GetDeviceState: attempting to use the device when it hasn't been acquired.");
                break;

            case DIERR_NOTINITIALIZED:
                throw gcnew DirectInputNotInitializedException("IDirectInputDevice8::GetDeviceState: object hasn't been initialised.");
                break;

            default:
                throw gcnew COMException("IDirectInputDevice8::GetDeviceState failed.", result);
                break;
        }
    }

    try
    {
        return gcnew Keys(currentKeyboardState, previousKeyboardState);
    }
    catch(...)
    {
        throw;
    }
}

Mouse ^Input::ReadMouse()
{
    LPDIMOUSESTATE2 mouseState;
    HRESULT result = lpDIMouse->GetDeviceState(sizeof(mouseState), (LPVOID)mouseState);
    if(result != DI_OK)
    {
        switch(result)
        {
            case DIERR_INPUTLOST:
                throw gcnew DirectInputDeviceLostException("IDirectInputDevice8::GetDeviceState: access to the input device has been lost. It must be reacquired.");
                break;

            case DIERR_INVALIDPARAM:
                throw gcnew DirectInputInvalidParameterException("IDirectInputDevice8::GetDeviceState: an invalid parameter was passed to the function, or the object is an invalid state.");
                break;

            case DIERR_NOTACQUIRED:
                throw gcnew DirectInputDeviceNotAcquiredException("IDirectInputDevice8::GetDeviceState: attempting to use the device when it hasn't been acquired.");
                break;

            case DIERR_NOTINITIALIZED:
                throw gcnew DirectInputNotInitializedException("IDirectInputDevice8::GetDeviceState: object hasn't been initialised.");
                break;

            default:
                throw gcnew COMException("IDirectInputDevice8::GetDeviceState failed.", result);
                break;
        }
    }

    try
    {
        return gcnew Mouse(mouseState);
    }
    catch(...)
    {
        throw;
    }
}

