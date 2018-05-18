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
    /// Opens a WAV file and attempts to load it into memory as a SoundBuffer object.
    /// </summary>
    /// <param name="filename">The name of the wav file to convert into a SoundBuffer object.</param>
    /// <returns>The desired WAV file as a SoundBuffer object.</returns>
    /// <exception cref="System::ArgumentException"><i>filename</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>filename</i> is <b>null</b>.</exception>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectSound returned an unspecified COM+ exception.</exception>
    /// <exception cref="DirectSoundAllocatedException">the request failed because audio resources were already in use by another caller.</exception>
    /// <exception cref="DirectSoundBadFormatException">the specified wave format is not supported.</exception>
    /// <exception cref="DirectSoundBufferLostException">the buffer memory has been lost and must be restored.</exception>
    /// <exception cref="DirectSoundBufferTooSmallException">the buffer size is not big enough.</exception>
    /// <exception cref="DirectSoundControlUnavailableException">the buffer control requested by the caller is not available.</exception>
    /// <exception cref="DirectSoundInvalidCallException">this function is not valid for the current state of this object.</exception>
    /// <exception cref="DirectSoundInvalidParameterException">an invalid parameter was passed to the returning function.</exception>
    /// <exception cref="DirectSoundNoAggregationException">the object does not support aggregation.</exception>
    /// <exception cref="DirectSoundPriorityLevelNeededException">A cooperative level of DSSCL_PRIORITY or higher is required.</exception>
    /// <exception cref="DirectSoundUninitializedException">DirectSound hasn't been initialised.</exception>
    /// <exception cref="DirectSoundUnsupportedException">the function called is not supported.</exception>
    /// <exception cref="DirectSoundVersion8RequiredException">DirectSound8 is required to complete the operation.</exception>
    /// <exception cref="System::IO::EndOfStreamException">the end of the file was reached prematurely.</exception>
    /// <exception cref="System::IO::FileFormatException">the specified file isn't a PCM wav file.</exception>
    /// <exception cref="System::IO::IOException">the method was unable to load the file specified in <i>filename</i> because it either couldn't be found or lacked the permissions to open it, or
    /// the file was corrupt.</exception>
    /// <exception cref="System::OutOfMemoryException">there isn't enough memory available to DirectSound to complete the operation.</exception>
    /// <remarks>Audio::CreateSoundBuffer() currently only supports 11Khz, 8 bit mono. Attempting
    /// to load audio with differing properties will result in disappointment.</remarks>
    static SoundBuffer ^CreateSoundBuffer(String ^filename);

    /// <summary>
    /// Initialises the audio subsystem. This method must be called before calling any other Audio methods.
    /// </summary>
    /// <param name="hWnd">the game window handle.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException">there wasn't enough memory available to DirectSound to complete the operation.</exception>
    /// <exception cref="DirectSoundAllocatedException">the request failed because resources were already in use by another caller.</exception>
    /// <exception cref="DirectSoundInvalidParameterException">an invalid parameter was passed to the returning function.</exception>
    /// <exception cref="DirectSoundNoAggregationException">the object does not support aggregation.</exception>
    /// <exception cref="DirectSoundNoDriverException">no sound driver is available for use, or the given GUID is not a valid DirectSound device ID.</exception>
    /// <exception cref="DirectSoundUninitializedException">Audio::Initialise() attempted to set the cooperative level after DirectSoundCreate8 failed.</exception>
    /// <exception cref="DirectSoundUnsupportedException">IDirectSound8::SetCooperativeLevel() is unsupported on this driver or implementation of DirectSound. Translation: no sound for you!</exception>
    /// <exception cref="OutOfMemoryException">not enough memory available to complete the operation.</exception>
    /// <remarks>Audio::Shutdown() releases the memory allocated by Audio::Initialise() and 
    /// should be called when the Audio subsystem is no longer needed.</remarks>
    static void Initialise(HWND hWnd);

    /// <summary>
    /// Shutsdown the audio subsystem. Once this method is called, Audio::Initialise() must be called again
    /// before any other Audio methods can be used.
    /// </summary>
    /// <remarks>When shutting down Audio, be sure to release any SoundBuffer objects that
    /// have been created before calling Audio::Shutdown(). Failing to do so can cause 
    /// a System::AccessViolation exception to be thrown.</remarks>
    static void Shutdown()
    {
        if(lpDS != NULL)
        {
            lpDS->Release();
            lpDS = NULL;
        }
    }
};