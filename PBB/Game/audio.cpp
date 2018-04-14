#include <vcclr.h>
#include "audio.h"
#include "exceptions.h"
#include "soundbuffer.h"

using namespace System::IO; // IOException
using namespace System::Runtime::InteropServices; // COMException

LPDIRECTSOUNDBUFFER8 Audio::CreateDirectSoundBuffer(unsigned int bufferSize)
{
    WAVEFORMATEX waveFormat;
    SecureZeroMemory(&waveFormat, sizeof(waveFormat));
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 1; // mono
    waveFormat.nSamplesPerSec = 11025;
    waveFormat.nBlockAlign = 1;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.wBitsPerSample = 8;
    waveFormat.cbSize = 0;    // always set to zero

    DSBUFFERDESC dsBD;
    SecureZeroMemory(&dsBD, sizeof(DSBUFFERDESC));
    dsBD.dwSize = sizeof(dsBD);
    dsBD.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
    dsBD.dwBufferBytes = bufferSize;
    dsBD.lpwfxFormat = &waveFormat;

    LPDIRECTSOUNDBUFFER lpDSTempBuffer = NULL;
    LPDIRECTSOUNDBUFFER8 lpDSSoundBuffer8 = NULL;

    HRESULT result = lpDS->CreateSoundBuffer(&dsBD, &lpDSTempBuffer, NULL);
    if(result != DS_OK)
    {
        switch(result)
        {
            case DSERR_ALLOCATED:
                throw gcnew DirectSoundAllocatedException("IDirectSound8::CreateSoundBuffer: the request "
                    "failed because resources, such as a priority level, were already in use by another caller.");
                break;

            case DSERR_BADFORMAT:
                throw gcnew DirectSoundBadFormatException("IDirectSound8::CreateSoundBuffer: the specified wave format is not supported.");
                break;

            case DSERR_BUFFERTOOSMALL:
                throw gcnew DirectSoundBufferTooSmallException("IDirectSound8::CreateSoundBuffer: the buffer size is not big enough.");
                break;

            case DSERR_CONTROLUNAVAIL:
                throw gcnew DirectSoundControlUnavailableException("IDirectSound8::CreateSoundBuffer: the buffer control requested by the caller is not available.");
                break;

            case DSERR_DS8_REQUIRED:
                throw gcnew DirectSoundVersion8RequiredException("IDirectSound8::CreateSoundBuffer: a DirectSound object of class CLSID_DirectSound8 or later is required for the requested functionality.");
                break;

            case DSERR_INVALIDCALL:
                throw gcnew DirectSoundInvalidCallException("IDirectSound8::CreateSoundBuffer: this function is not valid for the current state of this object.");
                break;

            case DSERR_INVALIDPARAM:
                throw gcnew DirectSoundInvalidParameterException("IDirectSound8::CreateSoundBuffer: an invalid parameter was passed to the returning function.");
                break;

            case DSERR_NOAGGREGATION:
                throw gcnew DirectSoundNoAggregationException("IDirectSound8::CreateSoundBuffer: the object does not support aggregation.");
                break;

            case DSERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("IDirectSound8::CreateSoundBuffer: the DirectSound subsystem could not allocate sufficient memory to complete the caller's request.");
                break;

            case DSERR_UNINITIALIZED:
                throw gcnew DirectSoundUninitializedException("IDirectSound8::CreateSoundBuffer: DirectSound hasn't been initialised.");
                break;

            case DSERR_UNSUPPORTED:
                throw gcnew DirectSoundUnsupportedException("IDirectSound8::CreateSoundBuffer: the function called is not supported.");
                break;

            default:
                throw gcnew COMException("IDirectSound::CreateSoundBuffer failed.", result);
                break;
        }
    }

    // create a DirectSound8 version of the buffer.
    result = lpDSTempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID *) &lpDSSoundBuffer8);
    if(result != S_OK) // S_OK is the same as DS_OK, fyi.
    {
        lpDSTempBuffer->Release();
        throw gcnew COMException("IDirectSoundBuffer::QueryInterface failed.", result);
    }
    
    return lpDSSoundBuffer8;
}

SoundBuffer ^Audio::CreateSoundBuffer(String ^filename)
{
    if(filename == nullptr)
    {
        throw gcnew ArgumentNullException("filename");
    }
    else if(filename == String::Empty)
    {
        throw gcnew ArgumentException("filename evaluates to String::Empty.");
    }

    HMMIO hWav;  // handle to the wave file
    MMCKINFO parentChunk;
    MMCKINFO childChunk;

    WAVEFORMATEX waveFormat;

    UCHAR *soundBuffer;
    UCHAR *audioPtr1 = NULL;
    UCHAR *audioPtr2 = NULL;

    DWORD audioLength1 = 0;
    DWORD audioLength2 = 0;


    parentChunk.ckid = (FOURCC)0;
    parentChunk.cksize = 0;
    parentChunk.fccType = (FOURCC)0;
    parentChunk.dwDataOffset = 0;
    parentChunk.dwFlags = 0;

    childChunk = parentChunk;

    pin_ptr<const wchar_t> pinnedBufferStr = PtrToStringChars(filename);

    //IntPtr ptr = Marshal::StringToHGlobalAnsi(buffer);
    hWav = mmioOpen((LPWSTR)pinnedBufferStr, NULL, MMIO_READ | MMIO_ALLOCBUF);
    if(!hWav)
    {
        // not using FileNotFoundException because it may have failed because of another
        // reason, such as permissions.
        throw gcnew IOException(String::Format("Failed to open {0}", filename));
    }

    parentChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E'); //mmioFOURCC is a macro
    MMRESULT mmIOResult = mmioDescend(hWav, &parentChunk, NULL, MMIO_FINDRIFF);

    if(mmIOResult != MMSYSERR_NOERROR) // returns MMIOERR_CHUNKNOTFOUND on error
    {
        mmioClose(hWav, 0);

        throw gcnew IOException(String::Format("{0} is either corrupt or isn't a valid WAV file.", filename));
    }

    childChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmIOResult = mmioDescend(hWav, &childChunk, &parentChunk, 0);
    if(mmIOResult != MMSYSERR_NOERROR)
    {
        mmioClose(hWav, 0);

        throw gcnew IOException(String::Format("{0} is either corrupt or isn't a valid WAV file.", filename));
    }

    // read the wave format information
    LONG bytesRead = mmioRead(hWav, (char *)&waveFormat, sizeof(waveFormat));
    if(bytesRead != sizeof(waveFormat))
    {
        mmioClose(hWav, 0);

        if(bytesRead == 0) // reached the end of the file while reading.
        {
            throw gcnew EndOfStreamException(String::Format("mmioRead: reached the end of {0} while attempting to read in the format data.", filename));
        }
        else
        {
            // -1 returned; error reading from the file.
            throw gcnew IOException(String::Format("mmioRead: failed to read from {0}", filename));
        }
    }

    if(waveFormat.wFormatTag != WAVE_FORMAT_PCM)
    {
        mmioClose(hWav, 0);

        throw gcnew FileFormatException(String::Format("{0} is not a valid WAV file.", filename));
    }

    mmIOResult = mmioAscend(hWav, &childChunk, 0);
    if(mmIOResult != MMSYSERR_NOERROR)
    {
        mmioClose(hWav, 0);

        // MMIOERR_CANNOTSEEK returned.mmioAscend also returns MMIOERR_CANNOTWRITE but
        // it's not handled here because it's not relevant; we're not writing to the file.
        throw gcnew IOException(String::Format("mmioAscend: failed to seek while processing {0}.", filename));
    }

    childChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    mmIOResult = mmioDescend(hWav, &childChunk, &parentChunk, MMIO_FINDCHUNK);
    if(mmIOResult != MMSYSERR_NOERROR)
    {
        mmioClose(hWav, 0);

        throw gcnew IOException("mmioDescend: the end of the file or parent chunk was reached before the desired chunk was found.");
    }

    // TODO: check for failure
    soundBuffer = (UCHAR *)malloc(childChunk.cksize);
    if(!soundBuffer)
    {
        mmioClose(hWav, 0);
        throw gcnew OutOfMemoryException("Not enough memory available to load the WAV file.");
    }

    bytesRead = mmioRead(hWav, (char*)soundBuffer, childChunk.cksize);
    if(bytesRead != childChunk.cksize)
    {
        mmioClose(hWav, 0);

        if(bytesRead == 0) // reached the end of the file while reading.
        {
            // TODO: considering changing this to IOException as well just to simplify it.
            throw gcnew EndOfStreamException(String::Format("mmioRead: reached the end of {0} while attempting to read in the format data.", filename));
        }
        else
        {
            // -1 returned; error reading from the file.
            throw gcnew IOException(String::Format("mmioRead: failed to read from {0}", filename));
        }
    }

    mmioClose(hWav, 0);

    LPDIRECTSOUNDBUFFER8 lpDSBuffer = NULL;
    try
    {
        lpDSBuffer = Audio::CreateDirectSoundBuffer(childChunk.cksize);
    }
    catch(...)
    {
        free(soundBuffer);
        throw;
    }


    HRESULT result = lpDSBuffer->Lock(0, childChunk.cksize, (void **)&audioPtr1,
             &audioLength1, (void **)&audioPtr2, &audioLength2, DSBLOCK_FROMWRITECURSOR);

    if(result != DS_OK)
    {
        free(soundBuffer);
        switch(result)
        {
            case DSERR_BUFFERLOST:
                throw gcnew DirectSoundBufferLostException("IDirectSoundBuffer8::Lock: the buffer memory has been lost and must be restored.");
                break;

            case DSERR_INVALIDCALL:
                throw gcnew DirectSoundInvalidCallException("IDirectSoundBuffer8::Lock: This function is not valid for the current state of this object.");
                break;

            case DSERR_INVALIDPARAM:
                throw gcnew DirectSoundInvalidParameterException("IDirectSoundBuffer8::Lock: An invalid parameter was passed to the returning function.");
                break;

            case DSERR_PRIOLEVELNEEDED:
                throw gcnew DirectSoundPriorityLevelNeededException("IDirectSoundBuffer8::Lock: A cooperative level of DSSCL_PRIORITY or higher is required.");
                break;

            default:
                throw gcnew COMException("IDirectSoundBuffer8::Lock failed.", result);
                break;
        }

        lpDSBuffer->Release();
    }

    // TODO: change these to secure versions
    memcpy(audioPtr1, soundBuffer, audioLength1);
    memcpy(audioPtr2, (soundBuffer + audioLength1), audioLength2);

    result = lpDSBuffer->Unlock(audioPtr1, audioLength1, audioPtr2, audioLength2);
    if(result != DS_OK)
    {
        free(soundBuffer);
        switch(result)
        {
            case DSERR_INVALIDCALL:
                throw gcnew DirectSoundInvalidCallException("IDirectSoundBuffer8::Unlock: this function is not valid for the current state of this object.");
                break;

            case DSERR_INVALIDPARAM:
                throw gcnew DirectSoundInvalidParameterException("IDirectSoundBuffer8::Unlock: an invalid parameter was passed to the returning function.");
                break;

            case DSERR_PRIOLEVELNEEDED:
                throw gcnew DirectSoundPriorityLevelNeededException("IDirectSoundBuffer8::Unlock: A cooperative level of DSSCL_PRIORITY or higher is required.");
                break;

            default:
                throw gcnew COMException("IDirectSoundBuffer::Unlock failed.", result);
                break;
        }

        lpDSBuffer->Release();
    }

    free(soundBuffer);

    return gcnew SoundBuffer(lpDSBuffer);
}

void Audio::Initialise(HWND hWnd)
{
    if(!hWnd)
    {
        throw gcnew ArgumentNullException("hWnd");
    }

    pin_ptr<LPDIRECTSOUND8> pinnedlpDS = &lpDS;
    HRESULT result = DirectSoundCreate8(NULL, pinnedlpDS, NULL);
    if(result != DS_OK)
    {
        switch(result)
        {
            case DSERR_ALLOCATED:
                throw gcnew DirectSoundAllocatedException("DirectSoundCreate8: the request failed because resources were already in use by another caller.");
                break;

            case DSERR_INVALIDPARAM:
                throw gcnew DirectSoundInvalidParameterException("DirectSoundCreate8: an invalid parameter was passed.");
                break;

            case DSERR_NOAGGREGATION:
                throw gcnew DirectSoundNoAggregationException("DirectSoundCreate8: the object does not support aggregation.");
                break;

            case DSERR_NODRIVER:
                throw gcnew DirectSoundNoDriverException("DirectSoundCreate8: no sound driver is available for use, or the given GUID is not a valid DirectSound device ID.");
                break;

            case DSERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("DirectSoundCreate8: not enough memory available to complete the operation.");
                break;

            default:
                throw gcnew COMException("DirectSoundCreate8 failed.", result);
                break;
        }
    }

    result = lpDS->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
    if(result != DS_OK)
    {
        switch(result)
        {
            case DSERR_ALLOCATED:
                throw gcnew DirectSoundAllocatedException("IDirectSound8::SetCooperativeLevel: the request failed because resources, such as a priority level, were already in use by another caller.");
                break;

            case DSERR_INVALIDPARAM:
                throw gcnew DirectSoundInvalidParameterException("IDirectSound8::SetCooperativeLevel: an invalid parameter was passed to the returning function.");
                break;

            case DSERR_UNINITIALIZED:
                throw gcnew DirectSoundUninitializedException("IDirectSound8::SetCooperativeLevel: the IDirectSound8::Initialize method has not been called or has not been called successfully before other methods were called.");
                break;

            case DSERR_UNSUPPORTED:
                throw gcnew DirectSoundUnsupportedException("IDirectSound8::SetCooperativeLevel: the function called is not supported.");
                break;
            
            default:
                throw gcnew COMException("IDirectSoundd8::SetCooperativeLevel failed", result);
                break;
        }
    }
}