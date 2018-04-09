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

#include <ddraw.h>

using namespace System;

/// <summary>
/// thrown when DDERR_DIRECTDRAWALREADYCREATED is returned by a DirectDraw function, indicating 
/// DirectDraw has already been initialised for this process using this driver. 
/// </summary>
public ref class DirectDrawAlreadyCreatedException : public Exception
{
public:
    DirectDrawAlreadyCreatedException() : Exception()
    {
        this->HResult = DDERR_DIRECTDRAWALREADYCREATED;
    }

    DirectDrawAlreadyCreatedException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_DIRECTDRAWALREADYCREATED;
    }
};

/// <summary>
/// thrown when DDERR_EXCLUSIVEMODEALREADYSET is returned by a call to DirectDraw, indicating an 
/// attempt was made to set the cooperative level when it was already set to exclusive.
/// </summary>
public ref class DirectDrawExclusiveModeAlreadySetException : public Exception
{
public:
    DirectDrawExclusiveModeAlreadySetException() : Exception()
    {
        this->HResult = DDERR_EXCLUSIVEMODEALREADYSET;
    }

    DirectDrawExclusiveModeAlreadySetException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_EXCLUSIVEMODEALREADYSET;
    }
};

/// <summary>
/// thrown when DDERR_GENERIC is returned by a DirectDraw call, indicating a non-specific error. 
/// </summary>
public ref class DirectDrawGenericException : public Exception
{
public:
    DirectDrawGenericException() : Exception()
    {
        this->HResult = DDERR_GENERIC;
    }

    DirectDrawGenericException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_GENERIC;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_HWNDALREADYSET, indicating that The DirectDraw cooperative
/// level window handle has already been set. It cannot be reset while the process has surfaces or 
/// palettes created. 
/// </summary>
public ref class DirectDrawHWNDAlreadySetException : public Exception
{
public:
    DirectDrawHWNDAlreadySetException() : Exception()
    {
        this->HResult = DDERR_HWNDALREADYSET;
    }

    DirectDrawHWNDAlreadySetException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_HWNDALREADYSET;
    }
};

/// <summary>
/// thrown when a DirectDraw call returns DDERR_HWNDSUBCLASSED, indicating that DirectDraw is prevented 
/// from restoring the state because the DirectDraw cooperative level window handle has been subclassed. 
/// </summary>
public ref class DirectDrawHWNDSubclassedException : public Exception
{
public:
    DirectDrawHWNDSubclassedException() : Exception()
    {
        this->HResult = DDERR_HWNDSUBCLASSED;
    }

    DirectDrawHWNDSubclassedException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_HWNDSUBCLASSED;
    }
};

/// <summary>
/// thrown when a call to DirectDraw returns DDERR_IMPLICITLYCREATED, indicating the surface cannot 
/// be restored because it is an implicitly created surface. 
/// </summary>
public ref class DirectDrawImplicitlyCreatedException : public Exception
{
public:
    DirectDrawImplicitlyCreatedException() : Exception()
    {
        this->HResult = DDERR_IMPLICITLYCREATED;
    }

    DirectDrawImplicitlyCreatedException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_IMPLICITLYCREATED;
    }
};

/// <summary>
/// thrown when DDERR_INCOMPATIBLEPRIMARY is returned by a directdraw function, indicating the 
/// primary surface creation request does not match the existing primary surface. 
/// </summary>
public ref class DirectDrawIncompatiblePrimarySurfaceException : public Exception
{
public:
    DirectDrawIncompatiblePrimarySurfaceException() : Exception()
    {
        this->HResult = DDERR_INCOMPATIBLEPRIMARY;
    }

    DirectDrawIncompatiblePrimarySurfaceException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INCOMPATIBLEPRIMARY;
    }
};

/// <summary>
/// thrown when a directdraw function returns DDERR_INVALIDCAPS, indicating one or more of the
/// capability bits passed to the callback function are incorrect. 
/// </summary>
public ref class DirectDrawInvalidCapsException : public Exception
{
public:
    DirectDrawInvalidCapsException() : Exception()
    {
        this->HResult = DDERR_INVALIDCAPS;
    }

    DirectDrawInvalidCapsException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INVALIDCAPS;
    }

};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_INVALIDCLIPLIST, indicating DirectDraw does 
/// not support the provided clip list. 
/// </summary>
public ref class DirectDrawInvalidClipListException : public Exception
{
public:
    DirectDrawInvalidClipListException() : Exception()
    {
        this->HResult = DDERR_INVALIDCLIPLIST;
    }

    DirectDrawInvalidClipListException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INVALIDCLIPLIST;
    }
};

/// <summary>
/// thrown when the globally unique identifier (GUID) passed to the DirectDrawCreate function 
/// is not a valid DirectDraw driver identifier. 
/// </summary>
public ref class DirectDrawInvalidGUIDException : public Exception
{
public:
    DirectDrawInvalidGUIDException() : Exception()
    {
        this->HResult = DDERR_INVALIDDIRECTDRAWGUID;
    }

    DirectDrawInvalidGUIDException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INVALIDDIRECTDRAWGUID;
    }
};

/// <summary>
/// thrown when DDERR_INVALIDMODE is returned by a DirectDraw function call, 
/// indicating the requested mode is not supported.
/// </summary>
public ref class DirectDrawInvalidModeException : public Exception
{
public:
    DirectDrawInvalidModeException() : Exception()
    {
        this->HResult = DDERR_INVALIDMODE;
    }

    DirectDrawInvalidModeException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INVALIDMODE;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_INVALIDOBJECT, indicating DirectDraw received 
/// a pointer that was an invalid DirectDraw object. 
/// </summary>
public ref class DirectDrawInvalidObjectException : public Exception
{
public:
    DirectDrawInvalidObjectException() : Exception()
    {
        this->HResult = DDERR_INVALIDOBJECT;
    }

    DirectDrawInvalidObjectException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INVALIDOBJECT;
    }
};

/// <summary>
/// thrown when DDERR_INVALIDPARAMS is returned by a DirectDraw function, indicating one or more of 
/// the parameters passed to the method are incorrect. 
/// </summary>
public ref class DirectDrawInvalidParametersException : public Exception
{
public:
    DirectDrawInvalidParametersException() : Exception()
    {
        this->HResult = DDERR_INVALIDPARAMS;
    }

    DirectDrawInvalidParametersException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INVALIDPARAMS;
    }
};

/// <summary>
/// thrown when a DirectDraw function call returns DDERR_INVALIDPIXELFORMAT, 
/// indicating the pixel format was invalid as specified.
/// </summary>
public ref class DirectDrawInvalidPixelFormatException : public Exception
{
public:
    DirectDrawInvalidPixelFormatException() : Exception()
    {
        this->HResult = DDERR_INVALIDPIXELFORMAT;
    }

    DirectDrawInvalidPixelFormatException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INVALIDPIXELFORMAT;
    }
};

/// <summary>
/// thrown when a DirectDraw call returns DDERR_INVALIDRECT, indicating the provided rectangle was invalid.
/// </summary>
public ref class DirectDrawInvalidRectException : public Exception
{
public:
    DirectDrawInvalidRectException() : Exception()
    {
        this->HResult = DDERR_INVALIDRECT;
    }

    DirectDrawInvalidRectException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INVALIDRECT;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_INVALIDSURFACETYPE, 
/// indicating the requested operation could not be performed because the surface was of the wrong type.
/// </summary>
public ref class DirectDrawInvalidSurfaceTypeException : public Exception
{
public:
    DirectDrawInvalidSurfaceTypeException() : Exception()
    {
        this->HResult = DDERR_INVALIDSURFACETYPE;
    }

    DirectDrawInvalidSurfaceTypeException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_INVALIDSURFACETYPE;
    }
};

/// <summary>
/// thrown when a DirectDraw call returns DDERR_LOCKEDSURFACES, indicating the surface being 
/// manipulated is locked, causing the operation to fail.
/// </summary>
public ref class DirectDrawLockedSurfaceException : public Exception
{
public:
    DirectDrawLockedSurfaceException() : Exception()
    {
        this->HResult = DDERR_LOCKEDSURFACES;
    }

    DirectDrawLockedSurfaceException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_LOCKEDSURFACES;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOALPHAHW, indicating that no 
/// alpha acceleration hardware is present or available, causing the failure of the requested operation.  
/// </summary>
public ref class DirectDrawNoAlphaHardwareException : Exception
{
public:
    DirectDrawNoAlphaHardwareException() : Exception()
    {
        this->HResult = DDERR_NOALPHAHW;
    }

    DirectDrawNoAlphaHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOALPHAHW;
    }
};

/// <summary>
/// thrown when a DirectDraw call returns DDERR_NOBLTHW, indicating no blitter hardware is present.
/// </summary>
public ref class DirectDrawNoBlitHardwareException : public Exception
{
public:
    DirectDrawNoBlitHardwareException() : Exception()
    {
        this->HResult = DDERR_NOBLTHW;
    }

    DirectDrawNoBlitHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOBLTHW;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOCLIPLIST, indicating no clip list is available.
/// </summary>
public ref class DirectDrawNoClipListException : Exception
{
public:
    DirectDrawNoClipListException() : Exception()
    {
        this->HResult = DDERR_NOCLIPLIST;
    }

    DirectDrawNoClipListException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOCLIPLIST;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOCLIPPERATTACHED, 
/// indicating no DirectDrawClipper object is attached to the surface object. 
/// </summary>
public ref class DirectDrawNoClipperAttachedException : Exception
{
public:
    DirectDrawNoClipperAttachedException() : Exception()
    {
        this->HResult = DDERR_NOCLIPPERATTACHED;
    }

    DirectDrawNoClipperAttachedException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOCLIPPERATTACHED;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOCOOPERATIVELEVELSET, indicating a create function 
/// is called without the IDirectDraw7::SetCooperativeLevel method being called. 
/// </summary>
public ref class DirectDrawNoCooperativeLevelSetException : public Exception
{
public:
    DirectDrawNoCooperativeLevelSetException() : Exception()
    {
        this->HResult = DDERR_NOCOOPERATIVELEVELSET;
    }

    DirectDrawNoCooperativeLevelSetException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOCOOPERATIVELEVELSET;
    }
};

/// <summary>
/// thrown when a DirectDraw call returns DDERR_NODIRECTDRAWHW, indicating hardware-only DirectDraw 
/// object creation is not possible; the driver does not support any hardware.
/// </summary>
public ref class DirectDrawNoHardwareException : public Exception
{
public:
    DirectDrawNoHardwareException() : Exception()
    {
        this->HResult = DDERR_NODIRECTDRAWHW;
    }

    DirectDrawNoHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NODIRECTDRAWHW;
    }
};

/// <summary>
/// thrown when a DirectDraw call returns DDERR_NODDROPSHW, indicating no DirectDraw 
/// raster operation (ROP) hardware is available. 
/// </summary>
public ref class DirectDrawNoDDRasterOperationHardwareException : public Exception
{
public:
    DirectDrawNoDDRasterOperationHardwareException() : Exception()
    {
        this->HResult = DDERR_NODDROPSHW;
    }

    DirectDrawNoDDRasterOperationHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NODDROPSHW;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOEMULATION, indicating no software emulation is available.
/// </summary>
public ref class DirectDrawNoEmulationException : public Exception
{
public:
    DirectDrawNoEmulationException() : Exception()
    {
        this->HResult = DDERR_NOEMULATION;
    }

    DirectDrawNoEmulationException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOEMULATION;
    }
};

/// <summary>
/// thrown when IDirectDraw7::SetDisplayMode returns DDERR_NOEXCLUSIVEMODE, indicating exclusive 
/// mode is required for the operation.
/// </summary>
public ref class DirectDrawNoExclusiveModeException : public Exception
{
public:
    DirectDrawNoExclusiveModeException() : Exception()
    {
        this->HResult = DDERR_NOEXCLUSIVEMODE;
    }

    DirectDrawNoExclusiveModeException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOEXCLUSIVEMODE;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOFLIPHW, indicating flipping visible 
/// surfaces is not supported by the hardware.
/// </summary>
public ref class DirectDrawNoFlipHardwareException : public Exception
{
public:
    DirectDrawNoFlipHardwareException() : Exception()
    {
        this->HResult = DDERR_NOFLIPHW;
    }

    DirectDrawNoFlipHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOFLIPHW;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOMIPMAPHW. Indicates the operation cannot 
/// be carried out because no mipmap capable texture mapping hardware is present or available.
/// </summary>
public ref class DirectDrawNoMipMapHardwareException : public Exception
{
public:
    DirectDrawNoMipMapHardwareException() : Exception()
    {
        this->HResult = DDERR_NOMIPMAPHW;
    }

    DirectDrawNoMipMapHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOMIPMAPHW;
    }
};

/// /// <summary>
/// thrown when a DirectDraw call returns DDERR_NOMIRRORHW, indicating the operation cannot be carried
/// out because no mirroring hardware is present or available.
/// </summary>
public ref class DirectDrawNoMirrorHardwareException : public Exception
{
public:
    DirectDrawNoMirrorHardwareException() : Exception()
    {
        this->HResult = DDERR_NOMIRRORHW;
    };
    DirectDrawNoMirrorHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOMIRRORHW;
    };
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOOVERLAYHW, indicating the operation 
/// cannot be carried out because no overlay hardware is present or available.
/// </summary>
public ref class DirectDrawNoOverlayHardwareException : public Exception
{
public:
    DirectDrawNoOverlayHardwareException() : Exception()
    {
        this->HResult = DDERR_NOOVERLAYHW;
    }

    DirectDrawNoOverlayHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOOVERLAYHW;
    }
};

/// <summary>
/// thrown when a function returns DDERR_NODDROPSHW, indicating no DirectDraw raster operation (ROP) hardware is available. 
/// </summary>
public ref class DirectDrawNoRasterOperationHardwareException : public Exception
{
public:
    DirectDrawNoRasterOperationHardwareException() : Exception()
    {
        this->HResult = DDERR_NORASTEROPHW;
    }

    DirectDrawNoRasterOperationHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NORASTEROPHW;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOROTATIONHW, indicating the operation cannot 
/// be carried out because no rotation hardware is present or available.
/// </summary>
public ref class DirectDrawNoRotationHardwareException : public Exception
{
public:
    DirectDrawNoRotationHardwareException() : Exception()
    {
        this->HResult = DDERR_NOROTATIONHW;
    }

    DirectDrawNoRotationHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOROTATIONHW;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOSTRETCHHW, indicating the operation cannot be 
/// carried out because there is no hardware support for stretching. 
/// </summary>
public ref class DirectDrawNoStretchHardwareException : public Exception
{
public:
    DirectDrawNoStretchHardwareException() : Exception()
    {
        this->HResult = DDERR_NOSTRETCHHW;
    }

    DirectDrawNoStretchHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOSTRETCHHW;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOTAOVERLAYSURFACE, indicating an overlay 
/// component is called for a non-overlay surface. 
/// </summary>
public ref class DirectDrawNotAnOverlaySurfaceException : public Exception
{
public:
    DirectDrawNotAnOverlaySurfaceException() : Exception()
    {
        this->HResult = DDERR_NOTAOVERLAYSURFACE;
    }

    DirectDrawNotAnOverlaySurfaceException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOTAOVERLAYSURFACE;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOTFLIPPABLE, indicating an attempt has been 
/// made to flip a surface that cannot be flipped. 
/// </summary>
public ref class DirectDrawNotFlippableException : public Exception
{
public:
    DirectDrawNotFlippableException() : Exception()
    {
        this->HResult = DDERR_NOTFLIPPABLE;
    }

    DirectDrawNotFlippableException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOTFLIPPABLE;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOTFOUND, indicating the requested item wasn't found.
/// </summary>
public ref class DirectDrawNotFoundException : public Exception
{
public:
    DirectDrawNotFoundException() : Exception()
    {
        this->HResult = DDERR_NOTFOUND;
    }

    DirectDrawNotFoundException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOTFOUND;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_NOZBUFFERHW, indicating the hardware doesn't have 
/// Z buffer support. 
/// </summary>
public ref class DirectDrawNoZBufferHardwareException : public Exception
{
public:
    DirectDrawNoZBufferHardwareException() : Exception()
    {
        this->HResult = DDERR_NOZBUFFERHW;
    }

    DirectDrawNoZBufferHardwareException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_NOZBUFFERHW;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_OUTOFVIDEOMEMORY, indicating there's not 
/// enough display memory to perfom the operation.
/// </summary>
public ref class DirectDrawOutOfVideoMemoryException : public Exception
{
public:
    DirectDrawOutOfVideoMemoryException() : Exception()
    {
        this->HResult = DDERR_OUTOFVIDEOMEMORY;
    }

    DirectDrawOutOfVideoMemoryException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_OUTOFVIDEOMEMORY;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_PRIMARYSURFACEALREADYEXISTS, indicating that 
/// this process has already created a primary surface. 
/// </summary>
public ref class DirectDrawPrimarySurfaceAlreadyExistsException : public Exception
{
public:
    DirectDrawPrimarySurfaceAlreadyExistsException() : Exception()
    {
        this->HResult = DDERR_PRIMARYSURFACEALREADYEXISTS;
    }

    DirectDrawPrimarySurfaceAlreadyExistsException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_PRIMARYSURFACEALREADYEXISTS;
    }
};

/// <summary>
/// thrown when DirectDraw7::SetDisplayMode returns DDERR_SURFACEBUSY, indicating the specified 
/// surface locked by another thread and access has been refused. 
/// </summary>
public ref class DirectDrawSurfaceBusyException : public Exception
{
public:
    DirectDrawSurfaceBusyException() : Exception()
    {
        this->HResult = DDERR_SURFACEBUSY;
    }

    DirectDrawSurfaceBusyException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_SURFACEBUSY;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_SURFACELOST, indicating access to the 
/// surface is refused because the surface memory is gone.
/// </summary>
public ref class DirectDrawSurfaceLostException : public Exception
{
public:
    DirectDrawSurfaceLostException() : Exception()
    {
        this->HResult = DDERR_SURFACELOST;
    }

    DirectDrawSurfaceLostException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_SURFACELOST;
    }
};

/// <summary>
/// thrown when a DirectDraw function call returns DDERR_UNSUPPORTED, indicating the operation isn't supported.
/// </summary>
public ref class DirectDrawUnsupportedException : public Exception
{
public:
    DirectDrawUnsupportedException() : Exception()
    {
        this->HResult = DDERR_UNSUPPORTED;
    }

    DirectDrawUnsupportedException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_UNSUPPORTED;
    }
};

/// <summary>
/// thrown when a DirectDraw call returns DDERR_UNSUPPORTEDMODE, indicating the desired mode is unsupported.
/// </summary>
public ref class DirectDrawUnsupportedModeException : public Exception
{
public:
    DirectDrawUnsupportedModeException() : Exception()
    {
        this->HResult = DDERR_UNSUPPORTEDMODE;
    }

    DirectDrawUnsupportedModeException(String ^message) : Exception(message)
    {
        this->HResult = DDERR_UNSUPPORTEDMODE;
    }
};

/// <summary>
/// thrown when a DirectDraw function returns DDERR_WASSTILLDRAWING, indicating the previous 
/// blit operation that is transferring information to or from this surface is incomplete. 
/// </summary>
public ref class DirectDrawWasStillDrawingException : public Exception
{
public:
    DirectDrawWasStillDrawingException() : Exception()
    {
        this->HResult = DDERR_WASSTILLDRAWING;
    }

    DirectDrawWasStillDrawingException(String ^message) : Exception()
    {
        this->HResult = DDERR_WASSTILLDRAWING;
    }
};