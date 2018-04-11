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
#include <ddraw.h>
#include "line.h"
#include "surface.h"

using namespace System;
using namespace System::Drawing;

ref class Video abstract sealed
{
private:
    // the directdraw com object.
    static LPDIRECTDRAW7 lpDD;

    // the screen surface.
    static LPDIRECTDRAWSURFACE7 lpDDSPrimarySurface;

    // the backbuffer.
    static LPDIRECTDRAWSURFACE7 lpDDSSecondarySurface;

    // the windows clipping rect. If you draw without the clipper and it happens to be drawn outside 
    // the window bounds, you'll corrupt memory and bad things will happen.
    static LPDIRECTDRAWCLIPPER lpDDClipper;

    // the colour key used by the games sprites as transparency.
    static unsigned int colourKey;

    /// <summary>
    /// Converts an ARGB value to a 32 bit integer, as required by DirectDraw.
    /// </summary>
    /// <param name="alpha"></param>
    /// <param name="red"></param>
    /// <param name="green"></param>
    /// <param name="blue"></param>
    /// <returns></returns>
    inline static unsigned int ARGBTo32Bit(unsigned int alpha, unsigned int red, unsigned int green, unsigned int blue)
    {
        return ((blue)+((green) << 8) + ((red) << 16) + ((alpha) << 24));
    }

    /// <summary>
    /// Attaches the clipper to the specified window.
    /// </summary>
    /// <param name="hWnd">The handle of the window the clipper should attach to.</param>
    /// <exception cref="DirectDrawInvalidClipListException">DirectDraw doesn't support the provided clip list.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawInvalidSurfaceTypeException">the requested operation could not be performed because the surface was of the wrong type.</exception>
    /// <exception cref="DirectDrawNoClipperAttachedException">No DirectDrawClipper object is attached to the surface object.</exception>
    /// <exception cref="DirectDrawNoCooperativeLevelSetException">attempting to attach a clipper without first setting the cooperative level.</exception>
    /// <exception cref="System::OutOfMemoryException">not enough memory was available to complete the operation.</exception>
    static void AttachClipper(HWND hWnd);

    /// <summary>
    /// creates a double-buffered fullscreen window at the specified resolution.
    /// </summary>
    /// <param name="hWnd"></param>
    /// <param name="width"></param>
    /// <param name="height"></param>
    /// <param name="bitsPerPixel"></param>
    /// <exception cref="System::Runtime::InteropServices::COMException">An unspecified COM error occured.</exception>
    /// <exception cref="DirectDrawExclusiveModeAlreadySetException">an attempt was made to set the cooperative level when it was already set to exclusive.</exception>
    /// <exception cref="DirectDrawGenericException">DirectDraw returned an undefined error condition.</exception>
    /// <exception cref="DirectDrawHWNDAlreadySetException">the window handle has already been set. It cannot be reset while the process has surfaces or palettes created.</exception>
    /// <exception cref="DirectDrawHWNDSubclassedException">the window handle has been subclassed.</exception>
    /// <exception cref="DirectDrawIncompatiblePrimarySurfaceException">the primary surface creation request does not match the existing primary surface.</exception>
    /// <exception cref="DirectDrawInvalidCapsException">one or more of the capability bits is incorrect.</exception>
    /// <exception cref="DirectDrawInvalidModeException">the requested mode is unsupported.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawInvalidPixelFormatException">an invalid pixel format was specified.</exception>
    /// <exception cref="DirectDrawLockedSurfaceException">one or more surfaces are locked, causing the failure of the requested operation.</exception>
    /// <exception cref="DirectDrawNoAlphaHardwareException">the video device doesn't support alpha hardware acceleration or it's unavailable.</exception>
    /// <exception cref="DirectDrawNoCooperativeLevelSetException">attempting to create a surface without first setting the cooperative level.</exception>
    /// <exception cref="DirectDrawNoEmulationException">software emulation isn't available.</exception>
    /// <exception cref="DirectDrawNoExclusiveModeException">exclusive mode required to complete the operation.</exception>
    /// <exception cref="DirectDrawNoFlipHardwareException">flipping visible surfaces is not supported by the video hardware.</exception>
    /// <exception cref="DirectDrawNoHardwareException">the current device doesn't support hardware-only DirectDraw acceleration.</exception>
    /// <exception cref="DirectDrawNoMipMapHardwareException">unable to complete the operation because mipmapping isn't supported by the hardware or is not available.</exception>
    /// <exception cref="DirectDrawNoOverlayHardwareException">unable to complete the operation because no hardware support for overlay is available.</exception>
    /// <exception cref="DirectDrawNotFoundException">The function used to create the backbuffer is missing from IDirectDrawSurface7.</exception>
    /// <exception cref="DirectDrawNoZBufferHardwareException">unable to complete the operation because no hardware support for Z-ordering overlays is available.</exception>
    /// <exception cref="DirectDrawPrimarySurfaceAlreadyExistsException">a primary surface already exists.</exception>
    /// <exception cref="DirectDrawOutOfVideoMemoryException">DirectDraw does not have enough display memory available to perform the operation.</exception>
    /// <exception cref="DirectDrawSurfaceBusyException">access to the surface is refused because the surface is locked by another thread.</exception>
    /// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone. Call the surfaces restore method to restore the memory associated with it.</exception>
    /// <exception cref="DirectDrawUnsupportedException">the operation is not supported.</exception>
    /// <exception cref="DirectDrawUnsupportedModeException">the display is currently in an unsupported mode.</exception>
    /// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation that is transferring information to or from this surface is incomplete.</exception>
    /// <exception cref="OutOfMemoryException">Not enough memory available to complete the operation.</exception>
    static void CreateFullScreenWindow(HWND hWnd, unsigned int width, unsigned int height, unsigned int bitsPerPixel);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="hBitmap"></param>
    /// <returns></returns>
    /// <exception cref="COMException">An unspecified COM error occured.</exception>
    /// <exception cref="DirectDrawDCAlreadyCreatedException">a device context has already been returned for this surface.</exception>
    /// <exception cref="DirectDrawGenericException">generic error occured, fucked if I know what's going on.</exception>
    /// <exception cref="DirectDrawIncompatiblePrimarySurfaceException">the image surface is incompatible with the format of the existing primary surface.</exception>
    /// <exception cref="DirectDrawInvalidCapsException">one or more of the capability bits is incorrect.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawInvalidPixelFormatException">an invalid pixel format was specified.</exception>
    /// <exception cref="DirectDrawInvalidSurfaceTypeException">the requested operation could not be performed because the surface was of the wrong type.</exception>
    /// <exception cref="DirectDrawNoAlphaHardwareException">the video device doesn't support alpha hardware acceleration or it's unavailable.</exception>
    /// <exception cref="DirectDrawNoCooperativeLevelSetException">attempting to create a surface without first setting the cooperative level.</exception>
    /// <exception cref="DirectDrawNoHardwareException">the current device doesn't support hardware-only DirectDraw acceleration.</exception>
    /// <exception cref="DirectDrawNoEmulationException">software emulation isn't available.</exception>
    /// <exception cref="DirectDrawNoExclusiveModeException">exclusive mode required to complete the operation.</exception>
    /// <exception cref="DirectDrawNoFlipHardwareException">flipping visible surfaces is not supported by the video hardware.</exception>
    /// <exception cref="DirectDrawNoMipMapHardwareException">unable to complete the operation because mipmapping isn't supported by the hardware or is not available.</exception>
    /// <exception cref="DirectDrawNoOverlayHardwareException">unable to complete the operation because no hardware support for overlay is available.</exception>
    /// <exception cref="DirectDrawNoZBufferHardwareException">unable to complete the operation because no hardware support for Z-ordering overlays is available.</exception>
    /// <exception cref="DirectDrawOutOfVideoMemoryException">DirectDraw does not have enough display memory available to perform the operation.</exception>
    /// <exception cref="DirectDrawPrimarySurfaceAlreadyExistsException">a primary surface already exists.</exception>
    /// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone. Call IDirectDrawSurface7::Restore to restore the memory associated with it.</exception>
    /// <exception cref="DirectDrawUnsupportedException">the operation is not supported.</exception>
    /// <exception cref="DirectDrawUnsupportedModeException">unable to create a surface for the current display mode, it's unsupported.</exception>
    /// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation is incomplete.</exception>
    /// <exception cref="OutOfMemoryException">DirectDraw does not have enough memory to perform the operation.</exception>
    /// <exception cref="Win32Exception">Copying the bitmap to the DirectDraw surface failed.</exception>
    static Surface ^CreateSurface(HBITMAP hBitmap);

    /// <summary>
    /// Clips the specified line if either of its points are located off screen, so Video::DrawLine(),
    /// doesn't attempt to access invalid regions of memory when drawing the line.
    /// </summary>
    /// <param name="line"></param>
    /// <param name="surfaceDescription"></param>
    /// <returns></returns>
    static Line ^ClipLine(Line ^line, LPDDSURFACEDESC2 surfaceDescription);
};