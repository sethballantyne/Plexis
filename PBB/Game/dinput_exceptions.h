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

using namespace System;

/// <summary>
/// thrown when a DirectInput call returns DIERR_ACQUIRED, indicating the operation
///  cannot be performed while the device is acquired. 
/// </summary>
public ref class DirectInputAcquiredException : public Exception
{
public:
    DirectInputAcquiredException() : Exception()
    {
        this->HResult = DIERR_ACQUIRED;
    }

    DirectInputAcquiredException(String ^message) : Exception(message)
    {
        this->HResult = DIERR_ACQUIRED;
    }
};

/// <summary>
/// thrown when a DirectInput call returns DIERR_DEVICENOTREG, indicating the device 
/// or device instance is not registered with DirectInput.
/// </summary>
public ref class DirectInputDeviceNotRegisteredException : public Exception
{
public:
    DirectInputDeviceNotRegisteredException() : Exception()
    {
        this->HResult = DIERR_DEVICENOTREG;
    }

    DirectInputDeviceNotRegisteredException(String ^message) : Exception(message)
    {
        this->HResult = DIERR_DEVICENOTREG;
    }
};

/// <summary>
/// thrown when a DirectInput call returns DIERR_INPUTLOST, indicating access 
/// to the input device has been lost. It must be reacquired. 
/// </summary>
public ref class DirectInputDeviceLostException : public Exception
{
public:
    DirectInputDeviceLostException() : Exception()
    {
        this->HResult = DIERR_INPUTLOST;
    }

    DirectInputDeviceLostException(String ^message) : Exception(message)
    {
        this->HResult = DIERR_INPUTLOST;
    }
};

/// <summary>
/// thrown when a DirectInput call returns DIERR_INVALIDPARAM, indicating an invalid parameter 
/// was passed to the returning function, or the object was not in a state that permitted the 
/// function to be called. 
/// </summary>
public ref class DirectInputInvalidParameterException : Exception
{
public:
    DirectInputInvalidParameterException() : Exception()
    {
        this->HResult = DIERR_INVALIDPARAM;
    }

    DirectInputInvalidParameterException(String ^message) : Exception(message)
    {
        this->HResult = DIERR_INVALIDPARAM;
    }
};

/// <summary>
/// thrown when a DirectInput call returns DIERR_NOTACQUIRED, indicating the operation 
/// cannot be performed unless the device is acquired.
/// </summary>
public ref class DirectInputDeviceNotAcquiredException : public Exception
{
public:
    DirectInputDeviceNotAcquiredException() : Exception()
    {
        this->HResult = DIERR_NOTACQUIRED;
    }

    DirectInputDeviceNotAcquiredException(String ^message) : Exception(message)
    {
        this->HResult = DIERR_NOTACQUIRED;
    }
};

/// <summary>
/// thrown when a DirectInput call returns DIERR_NOINTERFACE, indicating the object does 
/// not support the specified interface.
/// </summary>
public ref class DirectInputNoInterfaceException : public Exception
{
public:
    DirectInputNoInterfaceException() : Exception()
    {
        this->HResult = DIERR_NOINTERFACE;
    }

    DirectInputNoInterfaceException(String ^message) : Exception(message)
    {
        this->HResult = DIERR_NOINTERFACE;
    }
};

/// <summary>
/// thrown when a DirectInput call returns DIERR_NOTINITIALIZED, indicating the object 
/// has not been initialized. 
/// </summary>
public ref class DirectInputNotInitializedException : public Exception
{
public:
    DirectInputNotInitializedException() : Exception()
    {
        this->HResult = DIERR_NOTINITIALIZED;
    }

    DirectInputNotInitializedException(String ^message) : Exception(message)
    {
        this->HResult = DIERR_NOTINITIALIZED;
    }
};

/// <summary>
/// thrown when a DirectInput call returns DIERR_OLDDIRECTINPUTVERSION, indicating the application 
/// requires a newer version of DirectInput. 
/// </summary>
public ref class DirectInputOldVersionException : public Exception
{
public:
    DirectInputOldVersionException() : Exception()
    {
        this->HResult = DIERR_OLDDIRECTINPUTVERSION;
    }

    DirectInputOldVersionException(String ^message) : Exception(message)
    {
        this->HResult = DIERR_OLDDIRECTINPUTVERSION;
    }
};

/// <summary>
/// thrown when a DirectInput call returns DIERR_OTHERAPPHASPRIO, indicating another application 
/// has a higher priority level, preventing this call from succeeding. 
/// </summary>
public ref class DirectInputOtherApplicationHasPriorityException : public Exception
{
public:
    DirectInputOtherApplicationHasPriorityException() : Exception()
    {
        this->HResult = DIERR_OTHERAPPHASPRIO;
    }

    DirectInputOtherApplicationHasPriorityException(String ^message) : Exception(message)
    {
        this->HResult = DIERR_OTHERAPPHASPRIO;
    }
};