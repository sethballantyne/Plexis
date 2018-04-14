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
#include "soundbuffer.h"

using namespace System;

/// <summary>
/// Wrapper around DirectSound 8. Audio handles the initialisation and shutting down of DirectSound
/// along with creting SoundBuffers based on WAV files read from disk.
/// </summary>
public ref class Audio abstract sealed
{
private:
    static LPDIRECTSOUND8 lpDS;

    /// <summary>
    /// Creates an empty sound buffer configured for 11Khz, 8bit mono.
    /// </summary>
    /// <param name="bufferSize">size of the buffer to create.</param>
    /// <returns>an empty buffer.</returns>
    /// <exception cref="DirectSoundAllocatedException">the request failed because resources were already in use by another caller.</exception>
    /// <exception cref="DirectSoundBadFormatException">the specified wave format is not supported.</exception>
    /// <exception cref="DirectSoundBufferTooSmallException">the buffer size is too small.</exception>
    /// <exception cref="DirectSoundControlUnavailableException">the buffer control requested by the caller is not available.</exception>
    /// <exception cref="DirectSoundVersion8RequiredException">a DirectSound object of class CLSID_DirectSound8 or later is required for the requested functionality.</exception>
    /// <exception cref="DirectSoundInvalidCallException">an invalid DirectSound call was made.</exception>
    /// <exception cref="DirectSoundInvalidParameterException">an invalid parameter was passed to the returning function.</exception>
    /// <exception cref="DirectSoundNoAggregationException">the object does not support aggregation.</exception>
    /// <exception cref="DirectSoundUninitializedException">DirectSound hasn't been initialised.</exception>
    /// <exception cref="DirectSoundUnsupportedException"> the function called is not supported.</exception>
    /// <exception cref="System::OutOfMemoryException">there wasn't enough memory available to complete the operation.</exception>
    static LPDIRECTSOUNDBUFFER8 CreateDirectSoundBuffer(unsigned int bufferSize);
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="buffer"></param>
    /// <returns></returns>
    static SoundBuffer ^CreateSoundBuffer(String ^buffer);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="hWnd"></param>
    static void Initialise(HWND hWnd);

    /// <summary>
    /// Shutsdown the audio subsystem. Once this method is called, Audio::Initialise() must be called again
    /// before any other Audio methods can be used.
    /// </summary>
    static void Shutdown()
    {
        if(lpDS != NULL)
        {
            lpDS->Release();
            lpDS = NULL;
        }
    }
};