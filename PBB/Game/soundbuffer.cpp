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
#include "soundbuffer.h"

void SoundBuffer::Play()
{
    HRESULT result = lpDSBuffer->Play(0, 0, 0);
    if(result != DS_OK)
    {
        switch(result)
        {
            case DSERR_BUFFERLOST:
                throw gcnew DirectSoundBufferLostException("IDirectSoundBuffer8::Play: the buffer memory has been lost and must be restored.");
                break;

            case DSERR_INVALIDCALL:
                throw gcnew DirectSoundInvalidCallException("IDirectSoundBuffer8::Play: this function is not valid for the current state of this object.");
                break;

            case DSERR_INVALIDPARAM:
                throw gcnew DirectSoundInvalidParameterException("IDirectSoundBuffer8::Play: an invalid parameter was passed to DirectSound.");
                break;

            case DSERR_PRIOLEVELNEEDED:
                throw gcnew DirectSoundPriorityLevelNeededException("IDirectSoundBuffer8::Play: a cooperative level of DSSCL_PRIORITY or higher is required.");
                break;

            default:
                throw gcnew COMException("IDirectSoundBuffer8::Play failed.", result);
                break;
        }

    }
}

void SoundBuffer::Stop()
{
    HRESULT result = lpDSBuffer->Stop();
    if(result != DS_OK)
    {
        switch(result)
        {
            case DSERR_INVALIDPARAM:
                throw gcnew DirectSoundInvalidParameterException("IDirectSoundBuffer8::Stop: an invalid parameter was passed to the returning function.");
                break;
  
            case DSERR_PRIOLEVELNEEDED:
                throw gcnew DirectSoundPriorityLevelNeededException("IDirectSoundBuffer8::Stop: a cooperative level of DSSCL_PRIORITY or higher is required.");
                break;

            default:
                throw gcnew COMException("IDirectSoundBuffer8::Stop failed.", result);
                break;
        }
    }

    result = lpDSBuffer->SetCurrentPosition(0);
    if(result != DS_OK)
    {
        switch(result)
        {
            case DSERR_INVALIDCALL:
                throw gcnew DirectSoundInvalidCallException("IDirectSoundBuffer8::SetCurrentPosition: unable to call this function in DirectSound's current state.");
                break;

            case DSERR_INVALIDPARAM:
                throw gcnew DirectSoundInvalidParameterException("IDirectSoundBuffer8::SetCurrentPosition: an invalid parameter was passed to the returning function.");
                break;

            case DSERR_PRIOLEVELNEEDED:
                throw gcnew DirectSoundPriorityLevelNeededException("IDirectSoundBuffer8::SetCurrentPosition: a cooperative level of DSSCL_PRIORITY or higher is required.");
                break;

            default:
                throw gcnew COMException("IDirectSoundBuffer8::SetCurrentPosition failed.", result);
                break;
        }
    }

}