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
#include <windows.h>
#include <vcclr.h>
#include "exceptions.h"
#include "surface.h"
#include "video.h"

using namespace System::Runtime::InteropServices; // COMException

void Video::AttachClipper(HWND hWnd)
{
    pin_ptr<LPDIRECTDRAWCLIPPER> pinnedClipper = &lpDDClipper;

    HRESULT result = lpDD->CreateClipper(0, pinnedClipper, NULL);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDraw7::CreateClipper: DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDraw7::CreateClipper: one or more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_NOCOOPERATIVELEVELSET:
                throw gcnew DirectDrawNoCooperativeLevelSetException("IDirectDraw7::CreateClipper: attempting to attach a clipper without first setting the cooperative level.");
                break;

            case DDERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("IDirectDraw7::CreateClipper: not enough memory available to complete the operation.");
                break;

            default:
                throw gcnew COMException("IDirectDraw7::CreateClipper failed.\n");
                break;
        }
    }

    // clipper was successfully created, attach it to the window.
    result = lpDDClipper->SetHWnd(0, hWnd);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_INVALIDCLIPLIST:
                throw gcnew DirectDrawInvalidClipListException("IDirectDrawClipper::SetHWnd: DirectDraw doesn't support the provided clip list.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDrawClipper::SetHWnd: DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDrawClipper::SetHWnd: One or more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("IDirectDrawClipper::SetHWnd: not enough memory to complete the operation.");
                break;

            default:
                throw gcnew COMException("IDirectDrawClipper::SetHWnd failed.", result);
                break;
        }
    }
   
    result = lpDDSSecondarySurface->SetClipper(lpDDClipper);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDrawSurface7::SetClipper: "
                    "DirectDraw doesn't support the provided clip list.");
                break;
            
            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDrawSurface7::SetClipper: "
                    "DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDSURFACETYPE:
                throw gcnew DirectDrawInvalidSurfaceTypeException("IDirectDrawSurface7::SetClipper: the "
                    "requested operation could not be performed because the surface was of the wrong type.");
                break;

            case DDERR_NOCLIPPERATTACHED:
                throw gcnew DirectDrawNoClipperAttachedException("SetClipper: No DirectDrawClipper "
                    "object is attached to the surface object.");
                break;

            default:
                throw gcnew COMException("IDirectDraw7::SetCooperativeLevel failed.", result);
                break;
        }
    }
}

void Video::CreateFullScreenWindow(HWND hWnd, unsigned int width, unsigned int height, unsigned int bitsPerPixel)
{
    HRESULT result = lpDD->SetCooperativeLevel(hWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_EXCLUSIVEMODEALREADYSET:
                throw gcnew DirectDrawExclusiveModeAlreadySetException("IDirectDraw7::SetCooperativeLevel: an attempt was made"
                    " to set the cooperative level when it was already set to exclusive.");
                break;

            case DDERR_HWNDALREADYSET:
                throw gcnew DirectDrawHWNDAlreadySetException("IDirectDraw7::SetCooperativeLevel: the window handle has "
                    "already been set. It cannot be reset while the process has surfaces or palettes created.");
                break;

            case DDERR_HWNDSUBCLASSED:
                throw gcnew DirectDrawHWNDSubclassedException("IDirectDraw7::SetCooperativeLevel: the window handle has been subclassed.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDraw7::SetCooperativelevel: DirectDraw received a pointer that was an "
                    "invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDraw7::SetCooperativeLevel: one or more of the parameters "
                    "passed to the method are incorrect.");
                break;

            case DDERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("IDirectDraw7::SetCooperativeLevel: Not enough memory available to "
                    "complete the operation.");
                break;

            default:
                throw gcnew COMException("IDirectDraw7::SetCooperativeLevel failed.", result);
                break;
        }
    }

    result = lpDD->SetDisplayMode(width, height, bitsPerPixel, 0, 0);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_GENERIC:
                throw gcnew DirectDrawGenericException("IDirectDraw7::SetDisplayMode: undefined error condition, so that's cool.");
                break;

            case DDERR_INVALIDMODE:
                throw gcnew DirectDrawInvalidModeException("IDirectDraw7::SetDisplayMode: the requested mode is unsupported.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDraw7::SetDisplayMode: DirectDraw received a "
                    "pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDraw7::SetDisplayMode: one or more of the "
                    "parameters passed to the method are incorrect.");
                break;

            case DDERR_LOCKEDSURFACES:
                throw gcnew DirectDrawLockedSurfaceException("IDirectDraw7::SetDisplayMode: one or more "
                    "surfaces are locked, causing the failure of the requested operation.");
                break;

            case DDERR_NOEXCLUSIVEMODE:
                throw gcnew DirectDrawNoExclusiveModeException("IDirectDraw7::SetDisplayMode: exclusive mode "
                    "required to complete the operation.");
                break;

            case DDERR_SURFACEBUSY:
                throw gcnew DirectDrawSurfaceBusyException("IDirectDraw7::SetDisplayMode: access to the "
                    "surface is refused because the surface is locked by another thread.");
                break;

            case DDERR_UNSUPPORTED:
                throw gcnew DirectDrawUnsupportedException("IDirectDraw7::SetDisplayMode: the operation is not supported.");
                break;

            case DDERR_UNSUPPORTEDMODE:
                throw gcnew DirectDrawUnsupportedModeException("IDirectDraw7::SetDisplayMode: the display is currently "
                    "in an unsupported mode.");
                break;

            case DDERR_WASSTILLDRAWING:
                throw gcnew DirectDrawWasStillDrawingException("IDirectDraw7::SetDisplayMode: the previous blit "
                    "operation that is transferring information to or from this surface is incomplete.");
                break;

            default:
                throw gcnew COMException("IDirectDraw7::SetDisplayMode failed.\n", result);
                break;

        }
    }

    // create the primary surface
    DDSURFACEDESC2 ddSD;
    SecureZeroMemory(&ddSD, sizeof(ddSD));

    ddSD.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddSD.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
    ddSD.dwBackBufferCount = 1;

    pin_ptr<LPDIRECTDRAWSURFACE7> pinnedPrimarySurface = &lpDDSPrimarySurface;
    result = lpDD->CreateSurface(&ddSD, pinnedPrimarySurface, NULL);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_INCOMPATIBLEPRIMARY:
                throw gcnew DirectDrawIncompatiblePrimarySurfaceException("IDirectDraw7::CreateSurface: the primary surface "
                    "creation request does not match the existing primary surface. ");
                break;

            case DDERR_INVALIDCAPS:
                throw gcnew DirectDrawInvalidCapsException("IDirectDraw7::CreateSurface: one or more of the capability bits "
                      "is incorrect.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDraw7::CreateSurface: DirectDraw received a pointer that "
                      "was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDraw7::CreateSurface: one or more of "
                        "the parameters passed to the method are incorrect.");
                break;

            case DDERR_INVALIDPIXELFORMAT:
                throw gcnew DirectDrawInvalidPixelFormatException("IDirectDraw7::CreateSurface: an invalid pixel "
                        "format was specified.");
                break;

            case DDERR_NOALPHAHW:
                throw gcnew DirectDrawNoAlphaHardwareException("IDirectDraw7::CreateSurface: the video device doesn't "
                    "support alpha hardware acceleration or it's unavailable.");
                break;

            case DDERR_NOCOOPERATIVELEVELSET:
                throw gcnew DirectDrawNoCooperativeLevelSetException("IDirectDraw7::CreateSurface: attempting to create a surface "
                    "without first setting the cooperative level.");
                break;

            case DDERR_NODIRECTDRAWHW:
                throw gcnew DirectDrawNoHardwareException("IDirectDraw7::CreateSurface: the current "
                    "device doesn't support hardware-only DirectDraw acceleration.");
                break;

            case DDERR_NOEMULATION:
                throw gcnew DirectDrawNoEmulationException("IDirectDraw7::CreateSurface: software "
                    "emulation isn't available.");
                break;

            case DDERR_NOEXCLUSIVEMODE:
                throw gcnew DirectDrawNoExclusiveModeException("IDirectDraw7::CreateSurface: "
                    "exclusive mode required to complete the operation.");
                break;

            case DDERR_NOFLIPHW:
                throw gcnew DirectDrawNoFlipHardwareException("IDirectDraw7::CreateSurface: flipping visible surfaces "
                    "is not supported by the video hardware.");
                break;

            case DDERR_NOMIPMAPHW:
                throw gcnew DirectDrawNoMipMapHardwareException("IDirectDraw7::CreateSurface: unable to complete "
                    "the operation because mipmapping isn't supported by the hardware or is not available.");
                break;

            case DDERR_NOOVERLAYHW:
                throw gcnew DirectDrawNoOverlayHardwareException("IDirectDraw7::CreateSurface: unable to complete "
                    "the operation because no hardware support for overlay is available.");
                break;

            case DDERR_NOZBUFFERHW:
                throw gcnew DirectDrawNoZBufferHardwareException("IDirectDraw7::CreateSurface: unable to "
                    "complete the operation because no hardware support for Z-ordering overlays is available.");
                break;

            case DDERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("IDirectDraw7::CreateSurface: DirectDraw does not have enough "
                    "memory to perform the operation.");
                break;

            case DDERR_OUTOFVIDEOMEMORY:
                throw gcnew DirectDrawOutOfVideoMemoryException("IDirectDraw7::CreateSurface: DirectDraw does not "
                    "have enough display memory to perform the operation.");
                break;

            case DDERR_PRIMARYSURFACEALREADYEXISTS:
                throw gcnew DirectDrawPrimarySurfaceAlreadyExistsException("IDirectDraw7::CreateSurface: a primary "
                    "surface already exists.");
                break;

            case DDERR_UNSUPPORTEDMODE:
                throw gcnew DirectDrawUnsupportedModeException("IDirectDraw7::CreateSurface: unable to create a "
                    "surface for the current display mode, it's unsupported.");
                break;

            default:
                throw gcnew COMException("IDirectDraw7::CreateSurface failed.", result);
        }
    }

    // associate the backbuffer with the primary surface.
    ddSD.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

    pin_ptr<LPDIRECTDRAWSURFACE7> pinnedBackBuffer = &lpDDSSecondarySurface;

    result = lpDDSPrimarySurface->GetAttachedSurface(&ddSD.ddsCaps, pinnedBackBuffer);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDrawSurface7::GetAttachedSurface: DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDrawSurface7::GetAttachedSurface: one or more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_NOTFOUND:
                throw gcnew DirectDrawNotFoundException("IDirectDrawSurface7::GetAttachedSurface: the requested item was not found.");
                break;

            case DDERR_SURFACELOST:
                throw gcnew DirectDrawSurfaceLostException("IDirectDrawSurface7::GetAttachedSurface: access to the surface is refused because the surface memory is gone. Call IDirectDrawSurfacce7::Restore() on this surface to restore the memory associated with it.");
                break;

            default:
                throw gcnew COMException("IDirectDrawSurface7::GetAttachedSurface failed.", result);
                break;
        }
    }

    try
    {
        AttachClipper(hWnd);
    }
    catch(...)
    {
        throw;
    }
}

Surface ^Video::CreateSurface(HBITMAP hBitmap)
{
    HDC hdcImage;
    HDC hDC;
    BITMAP bitmap;
    LPDIRECTDRAWSURFACE7 lpDDSurface = NULL;
    DDSURFACEDESC2 ddSD;

    if(!hBitmap)
    {
        throw gcnew ArgumentNullException("hBitmap is NULL.");
    }

    // retrieve bitmap settings.
    GetObject(hBitmap, sizeof(bitmap), &bitmap);

    SecureZeroMemory(&ddSD, sizeof(ddSD));
    ddSD.dwSize = sizeof(ddSD);
    ddSD.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CKSRCBLT;
    ddSD.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddSD.dwWidth = bitmap.bmWidth;
    ddSD.dwHeight = bitmap.bmHeight;
    ddSD.ddckCKSrcBlt.dwColorSpaceLowValue = colourKey;
    ddSD.ddckCKSrcBlt.dwColorSpaceHighValue = colourKey;

    HRESULT result = lpDD->CreateSurface(&ddSD, &lpDDSurface, NULL);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_INCOMPATIBLEPRIMARY:
                throw gcnew DirectDrawIncompatiblePrimarySurfaceException("IDirectDraw7::CreateSurface: the image surface is incompatible with the format of the existing primary surface.");
                break;

            case DDERR_INVALIDCAPS:
                throw gcnew DirectDrawInvalidCapsException("IDirectDraw7::CreateSurface: one or more of the capability bits is incorrect.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDraw7::CreateSurface: DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDraw7::CreateSurface: one or more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_INVALIDPIXELFORMAT:
                throw gcnew DirectDrawInvalidPixelFormatException("IDirectDraw7::CreateSurface: an invalid pixel format was specified.");
                break;

            case DDERR_NOALPHAHW:
                throw gcnew DirectDrawNoAlphaHardwareException("IDirectDraw7::CreateSurface: the video device doesn't support alpha hardware acceleration or it's unavailable.");
                break;

            case DDERR_NOCOOPERATIVELEVELSET:
                throw gcnew DirectDrawNoCooperativeLevelSetException("IDirectDraw7::CreateSurface: attempting to create a surface without first setting the cooperative level.");
                break;

            case DDERR_NODIRECTDRAWHW:
                throw gcnew DirectDrawNoHardwareException("IDirectDraw7::CreateSurface: the current device doesn't support hardware-only DirectDraw acceleration.");
                break;

            case DDERR_NOEMULATION:
                throw gcnew DirectDrawNoEmulationException("IDirectDraw7::CreateSurface: software emulation isn't available.");
                break;

            case DDERR_NOEXCLUSIVEMODE:
                throw gcnew DirectDrawNoExclusiveModeException("IDirectDraw7::CreateSurface: exclusive mode required to complete the operation.");
                break;

            case DDERR_NOFLIPHW:
                throw gcnew DirectDrawNoFlipHardwareException("IDirectDraw7::CreateSurface: flipping visible surfaces is not supported by the video hardware.");
                break;

            case DDERR_NOMIPMAPHW:
                throw gcnew DirectDrawNoMipMapHardwareException("IDirectDraw7::CreateSurface: unable to complete the operation because mipmapping isn't supported by the hardware or is not available.");
                break;

            case DDERR_NOOVERLAYHW:
                throw gcnew DirectDrawNoOverlayHardwareException("IDirectDraw7::CreateSurface: unable to complete the operation because no hardware support for overlay is available.");
                break;

            case DDERR_NOZBUFFERHW:
                throw gcnew DirectDrawNoZBufferHardwareException("IDirectDraw7::CreateSurface: unable to complete the operation because no hardware support for Z-ordering overlays is available.");
                break;

            case DDERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("IDirectDraw7::CreateSurface: DirectDraw does not have enough memory to perform the operation.");
                break;

            case DDERR_OUTOFVIDEOMEMORY:
                throw gcnew DirectDrawOutOfVideoMemoryException("IDirectDraw7::CreateSurface: DirectDraw does not have enough display memory to perform the operation.");
                break;

            case DDERR_PRIMARYSURFACEALREADYEXISTS:
                throw gcnew DirectDrawPrimarySurfaceAlreadyExistsException("IDirectDraw7::CreateSurface: a primary surface already exists.");
                break;

            case DDERR_UNSUPPORTEDMODE:
                throw gcnew DirectDrawUnsupportedModeException("IDirectDraw7::CreateSurface: unable to create a surface for the current display mode, it's unsupported.");
                break;

            default:
                throw gcnew COMException("IDirectDraw7::CreateSurface failed.", result);
                break;
        }
    }

    result = lpDDSurface->GetDC(&hDC);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_DCALREADYCREATED:
                throw gcnew DirectDrawDCAlreadyCreatedException("IDirectDrawSurface7::GetDC: a device context has already been returned for this surface.");
                break;

            case DDERR_GENERIC:
                throw gcnew DirectDrawGenericException("IDirectDrawSurface7::GetDC: generic error occured, fucked if I know what's going on.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDrawSurface7::GetDC: DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDrawSurface7::GetDC: one or more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_INVALIDSURFACETYPE:
                throw gcnew DirectDrawInvalidSurfaceTypeException("IDirectDrawSurface7::GetDC: the requested operation could not be performed because the surface was of the wrong type. ");
                break;

            case DDERR_SURFACELOST:
                throw gcnew DirectDrawSurfaceLostException("IDirectDrawSurface7::GetDC: access to the surface is refused because the surface memory is gone. Call IDirectDrawSurface7::Restore to restore the memory associated with it.");
                break;

            case DDERR_UNSUPPORTED:
                throw gcnew DirectDrawUnsupportedException("IDirectDrawSurface7::GetDC: the operation is not supported.");
                break;

            case DDERR_WASSTILLDRAWING:
                throw gcnew DirectDrawWasStillDrawingException("IDirectDrawSurface7::GetDC: the previous blit operation is incomplete.");
                break;

            default:
                throw gcnew COMException("IDirectDrawSurface7::GetDC failed.", result);
                break;
        }
    }

    hdcImage = CreateCompatibleDC(NULL);
    if(!hdcImage)
    {
        throw gcnew Win32Exception(Win32Exception::Format("CreateCompatibleDC"));
    }

    SelectObject(hdcImage, hBitmap);

    if(!BitBlt(hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcImage, 0, 0, SRCCOPY))
    {
        throw gcnew Win32Exception(Win32Exception::Format("BitBlt"));
    }

    lpDDSurface->ReleaseDC(hDC);
    DeleteObject(hdcImage);

    return gcnew Surface(lpDDSurface, bitmap.bmWidth, bitmap.bmHeight);
}

