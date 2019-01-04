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

using namespace System::Drawing;
using namespace System::ComponentModel; // Win32Exception
using namespace System::Runtime::InteropServices; // COMException

// importing all these with SetLastError=true so Marshal::GetLastWin32Error() can be used
// to report why they fail if they do. Only these methods are being explicitly imported because
// they're the only ones that make use of GetLastWin32Error() and the Win32Exception class when failing.
[DllImport("gdi32.dll", SetLastError=true)]
IntPtr CreateCompatibleDC(IntPtr hDC);

[DllImport("gdi32.dll", SetLastError=true)]
bool BitBlt(IntPtr hObject, int xDest, int yDest, int width, int height, IntPtr hDCSrc,
	int xSrc, int ySrc, int rasterOperations);

[DllImport("gdi32.dll", SetLastError=true)]
int LoadImage(int hinst, int lpszName, unsigned int uType, int cxDesired, int cyDesired, unsigned int fuLoad);

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
    ddSD.dwSize = sizeof(DDSURFACEDESC2);
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
        throw gcnew Win32Exception(Marshal::GetLastWin32Error());
    }

    SelectObject(hdcImage, hBitmap);

    if(!BitBlt(hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcImage, 0, 0, SRCCOPY))
    {
        throw gcnew Win32Exception(Marshal::GetLastWin32Error());
    }

    lpDDSurface->ReleaseDC(hDC);
    DeleteObject(hdcImage);

    return gcnew Surface(lpDDSurface, bitmap.bmWidth, bitmap.bmHeight);
}

Line ^Video::ClipLine(Line ^line, LPDDSURFACEDESC2 surfaceDescription)
{
    const int clipCodeCenter = 0;
    const int clipCodeWest = 1;
    const int clipCodeEast = 2;
    const int clipCodeSouth = 4;
    const int clipCodeSouthWest = 5;
    const int clipCodeSouthEast = 6;
    const int clipCodeNorth = 8;
    const int clipCodeNorthWest = 9;
    const int clipCodeNorthEast = 10;

    int fromPointCode = 0;
    int toPointCode = 0;

    Point ^tempFromPoint = line->From;
    Point ^tempToPoint = line->To;

    System::Drawing::Rectangle ^clipRect = gcnew System::Drawing::Rectangle(0, 0, 
        surfaceDescription->dwWidth, surfaceDescription->dwHeight);

    if(line->From->Y < 0)
    {
        fromPointCode |= clipCodeNorth;
    }
    else if(line->From->Y > clipRect->Height)
    {
        fromPointCode |= clipCodeSouth;
    }

    if(line->From->X < 0)
    {
        fromPointCode |= clipCodeWest;
    }
    else if(line->From->X > clipRect->Width)
    {
        fromPointCode |= clipCodeEast;
    }

    if(line->To->Y < 0)
    {
        toPointCode |= clipCodeNorth;
    }
    else if(line->To->Y > clipRect->Height)
    {
        toPointCode |= clipCodeSouth;
    }

    if(line->To->X < 0)
    {
        toPointCode |= clipCodeWest;
    }
    else if(line->To->X > clipRect->Width)
    {
        toPointCode |= clipCodeEast;
    }

    // check if both points match or the line is completely off screen. 
    // if they do, we can't clip anything because there's nothing to be clipped.
    if(fromPointCode & toPointCode)
    {
        throw gcnew ArgumentException("both points match or the line is completely off screen.");
    }
    else if(fromPointCode == 0 && toPointCode == 0)
    {
        // neither point is clipped by the bounding rect, so there's nothing more to do.
        return line;
    }

    // determine the end clip point for the first point
    switch(fromPointCode)
    {
        case clipCodeNorth:
            tempFromPoint->Y = clipRect->Top;
            tempFromPoint->X = line->From->X + 0.5 + (clipRect->Y - line->From->Y) * (line->To->X - line->From->X) / (line->To->Y - line->From->Y);
            break;

        case clipCodeSouth:
            tempFromPoint->Y = clipRect->Bottom;
            tempFromPoint->X = line->From->X + 0.5 + (clipRect->Bottom - line->From->Y) * (line->To->X - line->From->X) / (line->To->Y - line->From->Y);
            break;

        case clipCodeWest:
            tempFromPoint->X = clipRect->Left;
            tempFromPoint->Y = line->From->Y + 0.5 + (clipRect->Left - line->From->X) * (line->To->Y - line->From->Y) / (line->To->X - line->From->X);
            break;

        case clipCodeEast:
            tempFromPoint->X = clipRect->Right;
            tempFromPoint->Y = line->From->Y + 0.5 + (clipRect->Right - line->From->X) * (line->To->Y - line->From->Y) / (line->To->X - line->From->X);
            break;

            //these cases are more complex, must compute 2 intersections
        case clipCodeNorthEast:
            // north horizontal line intersection
            tempFromPoint->Y = clipRect->Y;
            tempFromPoint->X = line->From->X + 0.5 + (clipRect->Y - line->From->Y) * (line->To->X - line->From->X) / (line->To->Y - line->From->Y);

            // test if intersection is valid; if so then done, else compute next
            if(tempFromPoint->X < clipRect->X || tempFromPoint->X > clipRect->Right)
            {
                // east vertical line intersection
                tempFromPoint->X = clipRect->Right;
                tempFromPoint->Y = line->From->Y + 0.5 + (clipRect->Right - line->From->X) * (line->To->Y - line->From->Y) / (line->To->X - line->From->X);
            }

            break;

        case clipCodeSouthEast:
            // south horizontal line intersection
            tempFromPoint->Y = clipRect->Bottom;
            tempFromPoint->X = line->From->X + 0.5 + (clipRect->Bottom - line->From->Y) * (line->To->X - line->From->X) / (line->To->Y - line->From->Y);

            // test if intersection is valid; if so then done, else compute next
            if(tempFromPoint->X < clipRect->X || tempFromPoint->X > clipRect->Right)
            {
                //east vertical line intersection
                tempFromPoint->X = clipRect->Right;
                tempFromPoint->Y = line->From->Y + 0.5 + (clipRect->Right - line->From->X) * (line->To->Y - line->From->Y) / (line->To->X - line->From->X);
            }

            break;

        case clipCodeNorthWest:
            // north horizontal line intersection
            tempFromPoint->Y = clipRect->Y;
            tempFromPoint->X = line->From->X + 0.5 + (clipRect->Y - line->From->Y) * (line->To->X - line->From->X) / (line->To->Y - line->From->Y);

            // test if intersection is valid; if so then done, else compute next
            if(tempFromPoint->X < clipRect->X || tempFromPoint->X > clipRect->Right)
            {
                tempFromPoint->X = clipRect->X;
                tempFromPoint->Y = line->From->Y + 0.5 + (clipRect->X - line->From->X) * (line->To->Y - line->From->Y) / (line->To->X - line->From->X);
            }

            break;

        case clipCodeSouthWest:
            // south hline intersection
            tempFromPoint->Y = clipRect->Bottom;
            tempFromPoint->X = line->From->X + 0.5 + (clipRect->Bottom - line->From->Y) * (line->To->X - line->From->X) / (line->To->Y - line->From->Y);

            // test if intersection is valid; if so then done, else compute next
            if(tempFromPoint->X < clipRect->X || tempFromPoint->X > clipRect->Right)
            {
                tempFromPoint->X = clipRect->X;
                tempFromPoint->Y = line->From->Y + 0.5 + (clipRect->X - line->From->X) * (line->To->Y - line->From->Y) / (line->To->X - line->From->X);
            }

            break;

        default:
            // log here
            break;
    }

    // determine the clipping point for tempToPoint.
    switch(toPointCode)
    {
        case clipCodeNorth:
            tempToPoint->Y = clipRect->Y;
            tempToPoint->X = line->To->X + (clipRect->Y - line->To->Y) * (line->From->X - line->To->X) / (line->From->Y - line->To->Y);
            break;

        case clipCodeSouth:
            tempToPoint->Y = clipRect->Bottom;
            tempToPoint->X = line->To->X + (clipRect->Bottom - line->To->Y) * (line->From->X - line->To->X) / (line->From->Y - line->To->Y);
            break;

        case clipCodeWest:
            tempToPoint->X = clipRect->X;
            tempToPoint->Y = line->To->Y + (clipRect->X - line->To->X) * (line->From->Y - line->To->Y) / (line->From->X - line->To->X);
            break;

        case clipCodeEast:
            tempToPoint->X = clipRect->Right;
            tempToPoint->Y = line->To->Y + (clipRect->Right - line->To->X) * (line->From->Y - line->To->Y) / (line->From->X - line->To->X);
            break;

            // these cases are more complex, must compute 2 intersections
        case clipCodeNorthEast:
            // north horizontal line intersection
            tempToPoint->Y = clipRect->Y;
            tempToPoint->X = line->To->X + 0.5 + (clipRect->Y - line->To->Y) * (line->From->X - line->To->X) / (line->From->Y - line->To->Y);

            // test if intersection is valid; if so then done, else compute next
            if(tempToPoint->X < clipRect->X || tempToPoint->X > clipRect->Right)
            {
                // east vertical line intersection
                tempToPoint->X = clipRect->Right;
                tempToPoint->Y = line->To->Y + 0.5 + (clipRect->Right - line->To->X) * (line->From->Y - line->To->Y) / (line->From->X - line->To->X);
            }

            break;

        case clipCodeSouthEast:
            // south horizontal line intersection
            tempToPoint->Y = clipRect->Bottom;
            tempToPoint->X = line->To->X + 0.5 + (clipRect->Bottom - line->To->Y) * (line->From->X - line->To->X) / (line->From->Y - line->To->Y);

            // test if intersection is valid; if so then done, else compute next
            if(tempToPoint->X < clipRect->X || tempToPoint->X > clipRect->Right)
            {
                // east vertical line intersection
                tempToPoint->X = clipRect->Right;
                tempToPoint->Y = line->To->Y + 0.5 + (clipRect->Right - line->To->X) * (line->From->Y - line->To->Y) / (line->From->X - line->To->X);
            }

            break;

        case clipCodeNorthWest:
            // north horizontal line intersection
            tempToPoint->Y = clipRect->Y;
            tempToPoint->X = line->To->X + 0.5 + (clipRect->Y - line->To->Y) * (line->From->X - line->To->X) / (line->From->Y - line->To->Y);

            // test if intersection is valid, of so then done, else compute next
            if(tempToPoint->X < clipRect->X || tempToPoint->X > clipRect->Right)
            {
                tempToPoint->X = clipRect->X;
                tempToPoint->Y = line->To->Y + 0.5 + (clipRect->X - line->To->X)*(line->From->Y - line->To->Y) / (line->From->X - line->To->X);
            }

            break;
        case clipCodeSouthWest:
            // south horizotal line intersection
            tempToPoint->Y = clipRect->Bottom;
            tempToPoint->X = line->To->X + 0.5 + (clipRect->Bottom - line->To->Y) * (line->From->X - line->To->X) / (line->From->Y - line->To->Y);

            // test if intersection is valid; if so then done, else compute next
            if(tempToPoint->X < clipRect->X || tempToPoint->X > clipRect->Right)
            {
                tempToPoint->X = clipRect->X;
                tempToPoint->Y = line->To->Y + 0.5 + (clipRect->X - line->To->X)*(line->From->Y - line->To->Y) / (line->From->X - line->To->X);
            }

            break;

        default:
            break;
    }

    // verify the lines have been clipped
    if((tempFromPoint->X < clipRect->X) || (tempFromPoint->X > clipRect->Right) ||
        (tempFromPoint->Y < clipRect->Y) || (tempFromPoint->Y > clipRect->Bottom) ||
        (tempToPoint->X < clipRect->X) || (tempToPoint->X > clipRect->Right) ||
        (tempToPoint->Y < clipRect->Y) || (tempToPoint->Y > clipRect->Bottom))
    {
        // nope. 
        throw gcnew ArgumentException("Failed to clip the line, so have fun with that.");
    }

    return gcnew Line(line->Colour, tempFromPoint, tempToPoint);
}

void Video::DrawLine(Line ^line, LPDDSURFACEDESC2 surfaceDescription)
{
    // difference between both points on the X axis
    int deltaX;

    // difference between both points on the Y axis.
    int deltaY;

    // the number of pixels to move along the X axis when drawing
    int xIncrementAmount;

    // the number of pixels to move along the Y axis when drawing
    int yIncrementAmount;

    UINT *videoBuffer = (UINT *) surfaceDescription->lpSurface;
    int pitch = surfaceDescription->lPitch >> 2;

    //-------------------------------------------------------------------------
    // determine the angle of the lines slope.
    // ------------------------------------------------------------------------
    deltaX = line->To->X - line->From->X;
    deltaY = line->To->Y - line->From->Y;

    if(deltaX >= 0)
    {
        // slope is to the right
        xIncrementAmount = 1;
    }
    else
    {
        // sloping to the left
        xIncrementAmount = -1;
        deltaX = -deltaX;
    }

    if(deltaY >= 0)
    {
        // sloping downwards.
        yIncrementAmount = 1;
    }
    else
    {
        // sloping upwards.
        yIncrementAmount = -1;
        deltaY = -deltaY;
    }

    int dx2 = deltaX << 1;
    int dy2 = deltaY << 1;

    int xCoordinate = line->From->X;
    int yCoordinate = line->From->Y;

    int error;
    
    unsigned int colour = ARGBTo32Bit(0, line->Colour.R, line->Colour.G, line->Colour.B);
    if(deltaX > deltaY)
    {
        error = dy2 - deltaX;

        for(int i = 0; i < deltaX; i++)
        {
            // draw the pixel
            videoBuffer[xCoordinate + yCoordinate * surfaceDescription->dwWidth] = colour;

            if(error >= 0)
            {
                error -= dx2;
                yCoordinate += yIncrementAmount;
            }

            error += dy2;
            xCoordinate += xIncrementAmount;
        }
    }
    else
    {
        error = dx2 - deltaY;

        for(int i = 0; i < deltaY; i++)
        {
            videoBuffer[xCoordinate + yCoordinate * surfaceDescription->dwWidth] = colour;
            if(error >= 0)
            {
                error -= dy2;
                xCoordinate += xIncrementAmount;
            }

            error += dx2;
            yCoordinate += yIncrementAmount;
        }
    }
}

void Video::Initialise()
{
    // magenta
    colourKey = ARGBTo32Bit(0, 255, 0, 255);

    // Note: Passing NULL in the first argument on machines with multiple monitors will
    // cause DirectDraw to run in emulation mode when the normal coperatative level is set.
    // see https://msdn.microsoft.com/en-us/library/windows/desktop/gg426118(v=vs.85).aspx.
    pin_ptr<LPDIRECTDRAW7> pinnedlpDD = &lpDD;
    HRESULT result = DirectDrawCreateEx(
        NULL,                   // pointer to a devices GUID. NULL = use default device.
        (void **)pinnedlpDD,    // argument will be set to a valid DirectDraw7 interface pointer if the call succeeds.
        IID_IDirectDraw7,           // must be IID_IDirectDraw7; function fails with DDERR_INVALIDPARAMS otherwise. 
        NULL);                  // reserved for future use, currently fails if NULL is not passed.

    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_INVALIDDIRECTDRAWGUID:
                throw gcnew DirectDrawInvalidGUIDException("IDirectDraw7::CreateDirectDrawEx: Invalid GUID passed to DirectDrawCreateEx");
                break;

            case DDERR_DIRECTDRAWALREADYCREATED:
                throw gcnew DirectDrawAlreadyCreatedException("IDirectDraw7::CreateDirectDrawEx: Direct Draw has already been initialised.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDraw7::CreateDirectDrawEx: Invalid parameters passed to DirectDrawCreateEx");
                break;

            case DDERR_GENERIC:
                throw gcnew DirectDrawGenericException("IDirectDraw7::CreateDirectDrawEx: Undefined error condition returned by DirectDrawCreateEx. Consult your local psychic.");
                break;

            case DDERR_NODIRECTDRAWHW:
                throw gcnew DirectDrawNoHardwareException("IDirectDraw7::CreateDirectDrawEx: The specified driver doesn't support DirectDraw hardware acceleration.");
                break;
            
            case DDERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("IDirectDraw7::CreateDirectDrawEx: Not enough memory available to initialise DirectDraw.");
                break;

            default:
                throw gcnew COMException("IDirectDraw7::CreateDirectDrawEx failed.", result);
        }
    }
}

void Video::Clear(unsigned int R, unsigned int G, unsigned B)
{
    DDBLTFX ddBltFX;

    SecureZeroMemory(&ddBltFX, sizeof(DDBLTFX));
    ddBltFX.dwFillColor = ARGBTo32Bit(255, R, G, B);
    ddBltFX.dwSize = sizeof(DDBLTFX);

    HRESULT result = lpDDSSecondarySurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFX);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_GENERIC:
                throw gcnew DirectDrawGenericException("IDirectDrawSurface7::Blt: failed, fucked if I know why.");
                break;

            case DDERR_INVALIDCLIPLIST:
                throw gcnew DirectDrawInvalidClipListException("IDirectDrawSurface7::Blt: DirectDraw does not support the provided clip list.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDrawSurface7::Blt: DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDrawSurface7::Blt: one or more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_INVALIDRECT:
                throw gcnew DirectDrawInvalidRectException("IDirectDrawSurface7::Blt: the provided rectangle was invalid.");
                break;

            case DDERR_NOALPHAHW:
                throw gcnew DirectDrawNoAlphaHardwareException("IDirectDrawSurface7::Blt: no alpha acceleration hardware is present or available.");
                break;

            case DDERR_NOBLTHW:
                throw gcnew DirectDrawNoBlitHardwareException("IDirectDrawSurface7::Blt: no blitter hardware is present.");
                break;

            case DDERR_NOCLIPLIST:
                throw gcnew DirectDrawNoClipListException("IDirectDrawSurface7::Blt: no clip list is available.");
                break;

            case DDERR_NODDROPSHW:
                throw gcnew DirectDrawNoDDRasterOperationHardwareException("IDirectDrawSurface7::Blt: no DirectDraw raster operation (ROP) hardware is available.");
                break;

            case DDERR_NOMIRRORHW:
                throw gcnew DirectDrawNoMirrorHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because no mirroring hardware is present or available.");
                break;

            case DDERR_NORASTEROPHW:
                throw gcnew DirectDrawNoRasterOperationHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because no appropriate raster operation hardware is present or available.");
                break;

            case DDERR_NOROTATIONHW:
                throw gcnew DirectDrawNoRotationHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because no rotation hardware is present or available.");
                break;

            case DDERR_NOSTRETCHHW:
                throw gcnew DirectDrawNoStretchHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because there is no hardware support for stretching.");
                break;

            case DDERR_NOZBUFFERHW:
                throw gcnew DirectDrawNoZBufferHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because there is no hardware support for Z-buffers.");
                break;

            case DDERR_SURFACEBUSY:
                throw gcnew DirectDrawSurfaceBusyException("IDirectDrawSurface7::Blt: access to the surface is refused because the surface is locked by another thread.");
                break;

            case DDERR_SURFACELOST:
                throw gcnew DirectDrawSurfaceLostException("IDirectDrawSurface7::Blt: access to the surface is refused because the surface memory is gone.");
                break;

            case DDERR_UNSUPPORTED:
                throw gcnew DirectDrawUnsupportedException("IDirectDrawSurface7::Blt: the operation is not supported.");
                break;

            case DDERR_WASSTILLDRAWING:
                throw gcnew DirectDrawWasStillDrawingException("IDirectDrawSurface7::Blt: the previous blit operation is incomplete.");
                break;

            default:
                throw gcnew COMException("IDirectDrawSurface7::Blt failed.", result);
                break;
        }
    }
}

Surface ^Video::CreateSurface(String ^path)
{
    if(path == nullptr)
    {
        throw gcnew ArgumentNullException("path");
    }
    else if(path == String::Empty)
    {
        throw gcnew ArgumentException("path evaluates to String::Empty.");
    }

    pin_ptr<const wchar_t> pinnedPath = PtrToStringChars(path);

    // LoadImage fails when attempting to load bitmaps that contain colour space information.
    // The gimp generates such bitmaps by default, disable it when exporting the bitmap
    // or this will fail. 
    HBITMAP hBitmap = (HBITMAP) LoadImage(NULL, pinnedPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if(!hBitmap)
    {
        throw gcnew Win32Exception(Marshal::GetLastWin32Error());
    }

    try
    {
        return Video::CreateSurface(hBitmap);
    }
    catch(...)
    {
        throw;
    }
    finally
    {
        DeleteObject(hBitmap);
    }
}

void Video::DrawLines(array<Line ^>^ lines)
{
    if(lines == nullptr)
    {
        throw gcnew ArgumentNullException("lines");
    }
    else if(lines->Length == 0)
    {
        throw gcnew ArgumentException("lines is an empty array.");
    }

    DDSURFACEDESC2 ddSD;
    SecureZeroMemory(&ddSD, sizeof(ddSD));
    ddSD.dwSize = sizeof(DDSURFACEDESC2);

    HRESULT result = lpDDSSecondarySurface->Lock(NULL, &ddSD, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDrawSurface7::Lock: DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDrawSurface7::Lock: one or more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_OUTOFMEMORY:
                throw gcnew OutOfMemoryException("IDirectDrawSurface7::Lock: not enough memory available to complete the operation.");
                break;

            case DDERR_SURFACEBUSY:
                throw gcnew DirectDrawSurfaceBusyException("IDirectDrawSurface7::Lock: access to the surface is refused because the surface is locked by another thread.");
                break;

            case DDERR_SURFACELOST:
                throw gcnew DirectDrawSurfaceLostException("IDirectDrawSurface7::Lock: access to the surface is refused because the surface memory is gone.");
                break;

            case DDERR_WASSTILLDRAWING:
                throw gcnew DirectDrawWasStillDrawingException("IDirectDrawSurface7::Lock: the previous blit operation is incomplete.");
                break;

            default:
                throw gcnew COMException("IDirectDrawSurface7::Lock failed.", result);
                break;
        }
    }
    else
    {
        for(int i = 0; i < lines->Length; i++)
        {
            try
            {
                Line ^clippedLine = Video::ClipLine(lines[i], &ddSD);
                Video::DrawLine(clippedLine, &ddSD);
            }
            catch(...)
            {
                throw;
            }
        }

        result = lpDDSSecondarySurface->Unlock(NULL);
        if(result != DD_OK)
        {
            switch(result)
            {
                case DDERR_GENERIC:
                    throw gcnew DirectDrawGenericException("IDirectDrawSurface7::Unlock: DirectDraw returned an unspecified error condition.");
                    break;

                case DDERR_INVALIDOBJECT:
                    throw gcnew DirectDrawInvalidObjectException("IDirectDrawSurface7::Unlock: DirectDraw received a pointer that was an invalid DirectDraw object.");
                    break;

                case DDERR_INVALIDPARAMS:
                    throw gcnew DirectDrawInvalidParametersException("IDirectDrawSurface7::Unlock: one or more of the parameters passed to the method are incorrect.");
                    break;

                case DDERR_INVALIDRECT:
                    throw gcnew DirectDrawInvalidRectException("IDirectDrawSurface7::Unlock: the rectangle coordinates used by the surface were invalid.");
                    break;

                case DDERR_NOTLOCKED:
                    throw gcnew DirectDrawNotLockedException("IDirectDrawSurface7::Unlock: attempting to unlock the surface when it wasn't locked.");
                    break;

                case DDERR_SURFACELOST:
                    throw gcnew DirectDrawSurfaceLostException("IDirectDrawSurface7::Unlock: access to the surface is refused because the surface memory is gone.");

                default:
                    break;
            }
        }
    }
}

void Video::Flip()
{
    // DDFLIP_WAIT ensures it'll keep trying to flip until successful if the HAL returns
    // DDERR_WASSTILLDRAWING. DDERR_WASSTILLDRAWING is handled below anyway, for the sake of completeness.
    HRESULT result = lpDDSPrimarySurface->Flip(NULL, DDFLIP_WAIT);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_GENERIC:
                throw gcnew DirectDrawGenericException("IDirectDrawSurface7::Flip: DirectDraw returned an unspecified error condition.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDrawSurface7::Flip: DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDrawSurface7::Flip: one or more of the parameters passed to the method are incorrect.");
                break;
        
            case DDERR_NOFLIPHW:
                throw gcnew DirectDrawNoFlipHardwareException("IDirectDrawSurface7::Flip: flipping visible surfaces is not supported by the video hardware.");
                break;

            case DDERR_NOTFLIPPABLE:
                throw gcnew DirectDrawNotFlippableException("IDirectDrawSurface7::Flip: attempting to flip a surface that cannot be flipped.");
                break;

            case DDERR_SURFACEBUSY:
                throw gcnew DirectDrawSurfaceBusyException("IDirectDrawSurface7::Flip: access to the surface is refused because the surface is locked by another thread.");
                break;

            case DDERR_SURFACELOST:
                throw gcnew DirectDrawSurfaceLostException("IDirectDrawSurface7::Flip: access to the surface is refused because the surface memory is gone.");
                break;

            case DDERR_UNSUPPORTED:
                throw gcnew DirectDrawUnsupportedException("IDirectDrawSurface7::Flip: the operation is not supported.");
                break;

            // probably won't throw this because of the DDFLIP_WAIT flag used in
            // the call to IDirectDrawSurface7::Flip, but it's here for completeness.
            case DDERR_WASSTILLDRAWING:
                throw gcnew DirectDrawWasStillDrawingException("IDirectDrawSurface7::Flip: the previous blit operation is incomplete.");
                break;

            default:
                throw gcnew COMException("IDirectDrawSurface7::Flip() failed.\n");
                break;
        }
    }
}

void Video::Blit(int x, int y, Surface ^surface)
{
    RECT destRect;
    RECT srcRect;

    destRect.left = x;
    destRect.top = y;
    destRect.bottom = destRect.top + surface->Size->Height;
    destRect.right = destRect.left + surface->Size->Width;

    srcRect.left = 0;
    srcRect.top = 0;
    srcRect.bottom = surface->Size->Height;
    srcRect.right = surface->Size->Width;

    HRESULT result = lpDDSSecondarySurface->Blt(&destRect, surface->Data, &srcRect, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_GENERIC:
                throw gcnew DirectDrawGenericException("IDirectDrawSurface7::Blt: DirectDraw reported an unspecified error.");
                break;

            case DDERR_INVALIDCLIPLIST:
                throw gcnew DirectDrawInvalidClipListException("IDirectDrawSurface7::Blt: DirectDraw does not support the provided clip list.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDrawSurface7::Blt: one or more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_INVALIDRECT:
                throw gcnew DirectDrawInvalidRectException("IDirectDrawSurface7::Blt: the provided rectangle was invalid.");
                break;

            case DDERR_NOALPHAHW:
                throw gcnew DirectDrawNoAlphaHardwareException("IDirectDrawSurface7::Blt: no alpha acceleration hardware is present or available.");
                break;

            case DDERR_NOBLTHW:
                throw gcnew DirectDrawNoBlitHardwareException("IDirectDrawSurface7::Blt: no blitter hardware is present.");
                break;

            case DDERR_NOCLIPLIST:
                throw gcnew DirectDrawNoClipListException("IDirectDrawSurface7::Blt: no clip list is available.");
                break;

            case DDERR_NODDROPSHW:
                throw gcnew DirectDrawNoDDRasterOperationHardwareException("IDirectDrawSurface7::Blt: no DirectDraw raster operation (ROP) hardware is available.");
                break;

            case DDERR_NOMIRRORHW:
                throw gcnew DirectDrawNoMirrorHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because no mirroring hardware is present or available.");
                break;

            case DDERR_NORASTEROPHW:
                throw gcnew DirectDrawNoRasterOperationHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because no appropriate raster operation hardware is present or available.");
                break;

            case DDERR_NOROTATIONHW:
                throw gcnew DirectDrawNoRotationHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because no rotation hardware is present or available.");
                break;

            case DDERR_NOSTRETCHHW:
                throw gcnew DirectDrawNoStretchHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because there is no hardware support for stretching.");
                break;

            case DDERR_NOZBUFFERHW:
                throw gcnew DirectDrawNoZBufferHardwareException("IDirectDrawSurface7::Blt: the hardware doesn't have Z buffer support.");
                break;

            case DDERR_SURFACEBUSY:
                throw gcnew DirectDrawSurfaceBusyException("IDirectDrawSurface7::Blt: the specified surface locked by another thread and access has been refused.");
                break;

            case DDERR_SURFACELOST:
                throw gcnew DirectDrawSurfaceLostException("IDirectDrawSurface7::Blt: access to the surface is refused because the surface memory is gone.");
                break;

            case DDERR_UNSUPPORTED:
                throw gcnew DirectDrawUnsupportedException("IDirectDrawSurface7::Blt: the operation isn't supported.");
                break;

            case DDERR_WASSTILLDRAWING:
                throw gcnew DirectDrawWasStillDrawingException("IDirectDrawSurface7::Blt: the previous blit operation that is transferring information to or from this surface is incomplete.");
                break;

            default:
                throw gcnew COMException("IDirectDrawSurface7::Blt failed.", result);
                break;
        }
    }
}

void Video::Blit(System::Drawing::Rectangle sourceRect, System::Drawing::Rectangle destinationRect, Surface ^surface)
{
    RECT srcRect;
    srcRect.top = sourceRect.Top;
    srcRect.bottom = sourceRect.Bottom;
    srcRect.left = sourceRect.Left;
    srcRect.right = sourceRect.Right;

    RECT destRect;
    destRect.top = destinationRect.Top;
    destRect.left = destinationRect.Left;
    destRect.bottom = destinationRect.Bottom;
    destRect.right = destinationRect.Right;

    HRESULT result = lpDDSSecondarySurface->Blt(&destRect, surface->Data, &srcRect, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_GENERIC:
                throw gcnew DirectDrawGenericException("IDirectDrawSurface7::Blt: DirectDraw reported an unspecified error.");
                break;

            case DDERR_INVALIDCLIPLIST:
                throw gcnew DirectDrawInvalidClipListException("IDirectDrawSurface7::Blt: DirectDraw does not support the provided clip list.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDrawSurface7::Blt: one or more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_INVALIDRECT:
                throw gcnew DirectDrawInvalidRectException("IDirectDrawSurface7::Blt: the provided rectangle was invalid.");
                break;

            case DDERR_NOALPHAHW:
                throw gcnew DirectDrawNoAlphaHardwareException("IDirectDrawSurface7::Blt: no alpha acceleration hardware is present or available.");
                break;

            case DDERR_NOBLTHW:
                throw gcnew DirectDrawNoBlitHardwareException("IDirectDrawSurface7::Blt: no blitter hardware is present.");
                break;

            case DDERR_NOCLIPLIST:
                throw gcnew DirectDrawNoClipListException("IDirectDrawSurface7::Blt: no clip list is available.");
                break;

            case DDERR_NODDROPSHW:
                throw gcnew DirectDrawNoDDRasterOperationHardwareException("IDirectDrawSurface7::Blt: no DirectDraw raster operation (ROP) hardware is available.");
                break;

            case DDERR_NOMIRRORHW:
                throw gcnew DirectDrawNoMirrorHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because no mirroring hardware is present or available.");
                break;

            case DDERR_NORASTEROPHW:
                throw gcnew DirectDrawNoRasterOperationHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because no appropriate raster operation hardware is present or available.");
                break;

            case DDERR_NOROTATIONHW:
                throw gcnew DirectDrawNoRotationHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because no rotation hardware is present or available.");
                break;

            case DDERR_NOSTRETCHHW:
                throw gcnew DirectDrawNoStretchHardwareException("IDirectDrawSurface7::Blt: the operation cannot be carried out because there is no hardware support for stretching.");
                break;

            case DDERR_NOZBUFFERHW:
                throw gcnew DirectDrawNoZBufferHardwareException("IDirectDrawSurface7::Blt: the hardware doesn't have Z buffer support.");
                break;

            case DDERR_SURFACEBUSY:
                throw gcnew DirectDrawSurfaceBusyException("IDirectDrawSurface7::Blt: the specified surface locked by another thread and access has been refused.");
                break;

            case DDERR_SURFACELOST:
                throw gcnew DirectDrawSurfaceLostException("IDirectDrawSurface7::Blt: access to the surface is refused because the surface memory is gone.");
                break;

            case DDERR_UNSUPPORTED:
                throw gcnew DirectDrawUnsupportedException("IDirectDrawSurface7::Blt: the operation isn't supported.");
                break;

            case DDERR_WASSTILLDRAWING:
                throw gcnew DirectDrawWasStillDrawingException("IDirectDrawSurface7::Blt: the previous blit operation that is transferring information to or from this surface is incomplete.");
                break;

            default:
                throw gcnew COMException("IDirectDrawSurface7::Blt failed.", result);
                break;
        }
    }
}

void Video::Restore()
{
    // the return values mentioned on MSDN for RestoreAllSurfaces() are wrong.
    HRESULT result = lpDD->RestoreAllSurfaces();
    if(result != DD_OK)
    {
        switch(result)
        {
            case DDERR_GENERIC:
                throw gcnew DirectDrawGenericException("IDirectDraw7::RestoreAllSurfaces: DirectDraw "
                    "returned an unspecified error condition.");
                break;

            case DDERR_IMPLICITLYCREATED:
                throw gcnew DirectDrawImplicitlyCreatedException("IDirectDraw7::RestoreAllSurfaces: the "
                    "surface cannot be restored because it is an implicitly created surface.");
                break;

            case DDERR_INCOMPATIBLEPRIMARY:
                throw gcnew DirectDrawIncompatiblePrimarySurfaceException("IDirectDraw7::RestoreAllSurfaces: "
                    "the primary surface creation request does not match the existing primary surface.");
                break;

            case DDERR_INVALIDOBJECT:
                throw gcnew DirectDrawInvalidObjectException("IDirectDraw7::RestoreAllSurfaces: DirectDraw received a pointer that was an invalid DirectDraw object.");
                break;

            case DDERR_INVALIDPARAMS:
                throw gcnew DirectDrawInvalidParametersException("IDirectDraw7::RestoreAllSurfaces: one or "
                    "more of the parameters passed to the method are incorrect.");
                break;

            case DDERR_NOEXCLUSIVEMODE:
                throw gcnew DirectDrawNoExclusiveModeException("IDirectDraw7::RestoreAllSurfaces: "
                    "exclusive mode is required to complete the operation.");
                break;

            case DDERR_OUTOFMEMORY:
                throw gcnew System::OutOfMemoryException("IDirectDraw7::RestoreAllSurfaces: there's not enough "
                    "memory available to complete the operation.");
                break;

            case DDERR_UNSUPPORTED:
                throw gcnew DirectDrawUnsupportedException("IDirectDraw7::RestoreAllSurfaces: the "
                    "operation is not supported.");
                break;

            case DDERR_WRONGMODE:
                throw gcnew DirectDrawWrongModeException("IDirectDraw7::RestoreAllSurfaces: the "
                    "surface cannot be restored because it was created in a different mode.");
                break;

            default:
                throw gcnew COMException("IDirectDraw7::RestoreAllSurfaces failed.", result);
                break;
        }
    }
}

void Video::SetDisplayMode(HWND hWnd, const unsigned int width, const unsigned int height, const unsigned int bitsPerPixel)
{
    if(!hWnd)
    {
        throw gcnew ArgumentNullException("hWnd");
    }

    // this method was originally supposed to support window and fullscreen modes.
    // this is here as a temporary measure for when it windowed mode is (finally) added
    bool fullscreen = true;

    // free the surfaces on the off chance that the function is called by the user due to 
    // DirectDrawWrongModeException being thrown(they have to be recreated if that's the case). 
    SafeRelease(lpDDSPrimarySurface);
   /* SafeRelease(lpDDClipper);
    SafeRelease(lpDDSSecondarySurface);*/
    //SafeRelease(lpDDClipper);

    try
    {
        if(fullscreen)
        {
            Video::CreateFullScreenWindow(hWnd, width, height, bitsPerPixel);
        }
    }
    catch(...)
    {
        throw;
    }
}