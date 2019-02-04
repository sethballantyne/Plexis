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
#include "exceptions.h"
#include "logmanager.h"

using namespace System::Runtime::InteropServices; // COMException

/// <summary>
/// Wrapper for LPDIRECTSOUNDBUFFER8 objects. SoundBuffer's are produced by Audio.
/// </summary>
public ref class SoundBuffer
{
private:
    LPDIRECTSOUNDBUFFER8 lpDSBuffer;

    String ^path;
public:
    SoundBuffer(LPDIRECTSOUNDBUFFER8 lpDSSoundBuffer)
    {
        if(!lpDSSoundBuffer)
        {
            throw gcnew ArgumentNullException("lpDSSoundBuffer");
        }

        this->lpDSBuffer = lpDSSoundBuffer;
    }

    /// <summary>
    /// Plays the audio data stored in the buffer at the volume specified by SoundBuffer::Volume property.
    /// </summary>
    /// <exception cref="System::Runtime::InteropServices::COMException">An unspecified COM+ error was returned.</exception>
    /// <exception cref="DirectSoundBufferLostException">the buffer memory has been lost and must be restored.</exception>
    /// <exception cref="DirectSoundInvalidCallException">this function is not valid for the current state of this object.</exception>
    /// <exception cref="DirectSoundInvalidParameterException">an invalid parameter was passed to DirectSound.</exception>
    /// <exception cref="DirectSoundPriorityLevelNeededException">a cooperative level of DSSCL_PRIORITY or higher is required.</exception>
    void Play();

    /// <summary>
    /// Releases the encapsulated DirectSoundBuffer8 object. Once this is called, SoundBuffer::Play, 
    /// SoundBuffer::Restore, SoundBuffer::Stop, SoundBuffer::IsPlaying, and SoundBuffer::Volume can no 
    /// longer be called.
    /// </summary>
    void Release()
    {
        if(lpDSBuffer != NULL)
        {
            lpDSBuffer->Release();
            lpDSBuffer = NULL;
        }
    }

    /// <summary>
    /// Stops the audio buffer if it's playing.
    /// </summary>
    /// <exception cref="System::Runtime::InteropServices::COMException">an unspecified COM+ error was returned by DirectSound.</exception>
    /// <exception cref="DirectSoundInvalidCallException">this function is not valid for the current state of this object.</exception>
    /// <exception cref="DirectSoundInvalidParameterException">an invalid parameter was passed to the returning function.</exception>
    /// <exception cref="DirectSoundPriorityLevelNeededException">a cooperative level of DSSCL_PRIORITY or higher is required.</exception>
    void Stop();

    /// <summary>
    /// Gets or sets the underlying IDirectSoundBuffer8 instance.
    /// </summary>
    property LPDIRECTSOUNDBUFFER8 Data
    {
        LPDIRECTSOUNDBUFFER8 get()
        {
            return this->lpDSBuffer;
        }
        void set(LPDIRECTSOUNDBUFFER8 value)
        {
            this->lpDSBuffer = value;
        }
    }
    /// <summary>
    /// Returns true if the buffer is currently playing, otherwise false.
    /// </summary>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectSound returned an unspecified error while querying the status of the buffer.</exception>
    /// <exception cref="DirectSoundInvalidParameterException">One or more invalid arguments were passed while querying DirectSound.</exception>
    property bool IsPlaying
    {
        bool get()
        {
            DWORD status = 0;

            HRESULT result = lpDSBuffer->GetStatus(&status);
            if(result != DS_OK)
            {
                switch(result)
                {
                    case DSERR_INVALIDPARAM:
                        throw gcnew DirectSoundInvalidParameterException("IDirectSoundBuffer8::GetStatus: an invalid parameter was passed.");
                        break;

                    default:
                        // the documentation mentions "If the method fails, the return value may be DSERR_INVALIDPARAM". *may be*. Really
                        // helpful, Microsoft. Some DirectSound exceptions will probably be caught here.
                        throw gcnew COMException("IDirectSoundBuffer8::GetStatus failed.", result);
                        break;
                }
            }

            return (status & DSBSTATUS_PLAYING);
        }
    }

    /// <summary>
    /// Gets or sets the location of the audio file used to create the sound buffer instance.
    /// </summary>
    /// <exception cref="System::ArgumentException">attempting to assign an empty string or a string that contains all whitespace.</exception>
    /// <exception cref="System::ArgumentNullException">attempting to assign a <b>null</b> value.</exception>
    /// <remarks>includes the filename of the originating .wav file.</remarks>
    property String ^Path
    {
        String ^get()
        {
            return this->path;
        }

        void set(String ^value)
        {
            if(nullptr == value)
            {
                throw gcnew ArgumentNullException("value");
            }
            if(String::Empty == value || String::IsNullOrWhiteSpace(value))
            {
                throw gcnew ArgumentException("invalid path.");
            }
        }
    }

    /// <summary>
    /// Gets and sets the buffer volume in the range of 0  - 100, 0 being silent and 100 being full volume.
    /// DirectSound treats 0 as being full volume, -10,000 is silent.
    /// </summary>
    /// <exception cref="System::Runtime::InteropServices::COMException">A unspecified COM+ error was returned.</exception>
    /// <exception cref="DirectSoundControlUnavailableException">the volume buffer control requested by the caller is unavailable.</exception>
    /// <exception cref="DirectSoundGenericException">DirectSound returned an undefined error condition.</exception>
    /// <exception cref="DirectSoundInvalidParameterException">an invalid parameter was passed to the returning function.</exception>
    /// <exception cref="DirectSoundPriorityLevelNeededException">A cooperative level of DSSCL_PRIORITY or higher is required.</exception>
    property unsigned int Volume
    {
        unsigned int get()
        {
            long volume;
            HRESULT result = lpDSBuffer->GetVolume(&volume);

            if(result != DS_OK)
            {
                switch(result)
                {
                    case DSERR_CONTROLUNAVAIL:
                        throw gcnew DirectSoundControlUnavailableException("IDirectSoundBuffer8::GetVolume: the volume buffer control requested by the caller is unavailable.");
                        break;

                    case DSERR_INVALIDPARAM :
                        throw gcnew DirectSoundInvalidParameterException("IDirectSoundBuffer8:GetVolume: an invalid parameter was passed to the returning function.");
                        break;

                    case DSERR_PRIOLEVELNEEDED:
                        throw gcnew DirectSoundPriorityLevelNeededException("IDirectSoundBuffer8:GetVolume: A cooperative level of DSSCL_PRIORITY or higher is required.");
                        break;

                    default:
                        throw gcnew COMException("IDirectSoundBuffer8::GetVolume failed", result);
                        break;   
                }
            }
            else
            {
                return (10000 - -volume) / 100;
            }
        }
        void set(unsigned int value)
        {
            LONG volume = 10000 - (value * 100);
            volume = -volume;

            HRESULT result = lpDSBuffer->SetVolume(volume);
            if(result != DS_OK)
            {
                switch(result)
                {
                    case DSERR_CONTROLUNAVAIL:
                        throw gcnew DirectSoundControlUnavailableException("IDirectSoundBuffer8::SetVolume: the volume buffer control requested by the caller is unavailable.");
                        break;

                    case DSERR_GENERIC:
                        throw gcnew DirectSoundGenericException("IDirectSoundBuffer8::SetVolume: an undetermined error occurred inside the DirectSound subsystem. Call a psychic, fucked if I know.");
                        break;

                    case DSERR_INVALIDPARAM:
                        throw gcnew DirectSoundInvalidParameterException("IDirectSoundBuffer8::SetVolume: an invalid parameter was passed to the returning function.");
                        break;

                    case DSERR_PRIOLEVELNEEDED:
                        throw gcnew DirectSoundPriorityLevelNeededException("IDirectSoundBuffer8::SetVolume: a cooperative level of DSSCL_PRIORITY or higher is required.");
                        break;

                    default:
                        throw gcnew COMException("IDirectSoundBuffer8::SetVolume: an unspecified COM+ error was returned.", result);
                        break;
                }
            }
        }
    }

    !SoundBuffer()
    {
        Release();
    }

    ~SoundBuffer()
    {
        Release();
    }
};