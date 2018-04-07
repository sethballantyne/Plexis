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

#include <dsound.h>

using namespace System;

/// <summary>
/// thrown when a DirectSound function returns DSERR_ALLOCATED, indicating the request failed 
/// because resources, such as a priority level, were already in use by another caller.
/// </summary>
public ref class DirectSoundAllocatedException : public Exception
{
public:
    DirectSoundAllocatedException() : Exception() 
    {
        this->HResult = DSERR_ALLOCATED;
    }
    DirectSoundAllocatedException(String ^message) : Exception(message) 
    {
        this->HResult = DSERR_ALLOCATED;
    }
};

/// <summary>
/// thrown when a DirectSound call returns DSERR_BADFORMAT, indicating that the specified 
/// wave format is not supported. 
/// </summary>
public ref class DirectSoundBadFormatException : public Exception
{
public:
    DirectSoundBadFormatException() : Exception()
    {
        this->HResult = DSERR_BADFORMAT;
    }

    DirectSoundBadFormatException(String ^format) : Exception(format)
    {
        this->HResult = DSERR_BADFORMAT;
    }
};

/// <summary>
/// thrown when a DirectSound call returns DSERR_BUFFERLOST, indicating the buffer memory 
/// has been lost and must be restored.
/// </summary>
public ref class DirectSoundBufferLostException : public Exception
{
    DirectSoundBufferLostException() : Exception()
    {
        this->HResult = DSERR_BUFFERLOST;
    }

    DirectSoundBufferLostException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_BUFFERLOST;
    }
};

/// <summary>
/// thrown when a DirectSound call returns DSERR_BUFFERTOOSMALL, indicating the buffer 
/// size is not great enough to enable effects processing.
/// </summary>
public ref class DirectSoundBufferTooSmallException : public Exception
{
public:
    DirectSoundBufferTooSmallException() : Exception()
    {
        this->HResult = DSERR_BUFFERTOOSMALL;
    }

    DirectSoundBufferTooSmallException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_BUFFERTOOSMALL;
    }
};

/// <summary>
/// thrown when a DirectSound call returns DSERR_CONTROLUNAVAIL, indicating the buffer control 
/// (volume, pan, and so on) requested by the caller is not available. Controls must be specified
/// when the buffer is created, using the dwFlags member of DSBUFFERDESC.
/// </summary>
public ref class DirectSoundControlUnavailableException : public Exception
{
public:
    DirectSoundControlUnavailableException() : Exception()
    {
        this->HResult = DSERR_CONTROLUNAVAIL;
    }

    DirectSoundControlUnavailableException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_CONTROLUNAVAIL;
    }
};

/// <summary>
/// thrown when a DirectSound call returns DSERR_DS8_REQUIRED, indicating a DirectSound object 
/// of class CLSID_DirectSound8 or later is required for the requested functionality.
/// </summary>
public ref class DirectSoundVersion8RequiredException : public Exception
{
public:
    DirectSoundVersion8RequiredException() : Exception()
    {
        this->HResult = DSERR_DS8_REQUIRED;
    }

    DirectSoundVersion8RequiredException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_DS8_REQUIRED;
    }
};

/// <summary>
/// thrown when DSERR_GENERIC is returned by a DirectSound call, indicating an 
/// undetermined error occurred inside the DirectSound subsystem. 
/// </summary>
public ref class DirectSoundGenericException : public Exception
{
public:
    DirectSoundGenericException() : Exception()
    {
        this->HResult = DSERR_GENERIC;
    }

    DirectSoundGenericException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_GENERIC;
    }
};

/// <summary>
/// thrown when a DirectSound call returns DSERR_INVALIDCALL, indicating the function is not 
/// valid for the current state of this object.
/// </summary>
public ref class DirectSoundInvalidCallException : public Exception
{
public:
    DirectSoundInvalidCallException() : Exception()
    {
        this->HResult = DSERR_INVALIDCALL;
    }

    DirectSoundInvalidCallException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_INVALIDCALL;
    }
};

/// <summary>
/// thrown when a DirectSound function retuns DSERR_INVALIDPARAM, indicating an invalid parameter 
/// was passed to the returning function.
/// </summary>
public ref class DirectSoundInvalidParameterException : public Exception
{
public:
    DirectSoundInvalidParameterException() : Exception()
    {
        this->HResult = DSERR_INVALIDPARAM;
    }

    DirectSoundInvalidParameterException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_INVALIDPARAM;
    }
};

/// <summary>
/// thrown when a DirectSound call returns DSERR_NOAGGREGATION, indicating the 
/// object does not support aggregation.
/// </summary>
public ref class DirectSoundNoAggregationException : public Exception
{
public:
    DirectSoundNoAggregationException() : Exception()
    {
        this->HResult = DSERR_NOAGGREGATION;
    }

    DirectSoundNoAggregationException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_NOAGGREGATION;
    }
};

/// <summary>
/// thrown when a DirectSound fuction returns DSERR_NODRIVER, indicating no 
/// sound driver is available for use, or the given GUID is not a valid DirectSound device ID.
/// </summary>
public ref class DirectSoundNoDriverException : public Exception
{
public:
    DirectSoundNoDriverException() : Exception()
    {
        this->HResult = DSERR_NODRIVER;
    }

    DirectSoundNoDriverException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_NODRIVER;
    }
};

/// <summary>
/// thrown when a DirectSound function returns DSERR_PRIOLEVELNEEDED, indicating a 
/// cooperative level of DSSCL_PRIORITY or higher is required.
/// </summary>
public ref class DirectSoundPriorityLevelNeededException : public Exception
{
public:
    DirectSoundPriorityLevelNeededException() : Exception()
    {
        this->HResult = DSERR_PRIOLEVELNEEDED;
    }

    DirectSoundPriorityLevelNeededException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_PRIOLEVELNEEDED;
    }
};

/// <summary>
/// thrown when a DirectSound function returns DSERR_UNINITIALIZED, indicating the 
/// IDirectSound8::Initialize method has not been called or has not been called successfully 
/// before other methods were called. 
/// </summary>
public ref class DirectSoundUninitializedException : public Exception
{
public:
    DirectSoundUninitializedException() : Exception()
    {
        this->HResult = DSERR_UNINITIALIZED;
    }

    DirectSoundUninitializedException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_UNINITIALIZED;
    }
};

/// <summary>
/// thrown when a DirectSound call returns DSERR_UNSUPPORTED, indicating the function 
/// called is not supported.
/// </summary>
public ref class DirectSoundUnsupportedException : public Exception
{
public:
    DirectSoundUnsupportedException() : Exception()
    {
        this->HResult = DSERR_UNSUPPORTED;
    }

    DirectSoundUnsupportedException(String ^message) : Exception(message)
    {
        this->HResult = DSERR_UNSUPPORTED;
    }
};