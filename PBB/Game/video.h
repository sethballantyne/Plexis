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
    /// <exception cref="DirectDrawNoExclusiveModeException">exclusive mode is required to complete the operation.</exception>
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
    /// Clips the specified line if either of its points are located off screen, so Video::DrawLine()
    /// doesn't attempt to access invalid regions of memory when drawing the line.
    /// </summary>
    /// <param name="line">the line to be clipped.</param>
    /// <param name="surfaceDescription"></param>
    /// <returns>A clipped version of <i>line. If the line didn't need to be clipped, <i>line</i> is 
    /// returned unmodified.</returns>
    /// <exception cref="System::ArgumemtException">The lines From and To points are the same, or
    /// the line is located off the screen and can't be clipped.</exception>
    static Line ^ClipLine(Line ^line, LPDDSURFACEDESC2 surfaceDescription);

    /// <summary>
    /// Draws <i>line</i> to the backbuffer using the Bresenham line drawing algorithm.
    /// </summary>
    /// <param name="line">the line to drawn.</param>
    /// <param name="surfaceDescription"></param>
    /// <remarks>Clip the line via Video::ClipLine() before using this method, else bad things will happen.</remarks>
    static void DrawLine(Line ^line, LPDDSURFACEDESC2 surfaceDescription);

    /// <summary>
    /// Internal function used to release DirectDraw objects.
    /// </summary>
    /// <param name="ddUnknown">the object to release.</param>
    static void SafeRelease(IUnknown *ddUnknown)
    {
        //pin_ptr<IUnknown> pinnedPtr = ddUnknown;
        if(ddUnknown != NULL)
        {
            ddUnknown->Release();
            ddUnknown = NULL;
        }
    }

public:
    /// <summary>
    /// Initialises DirectDraw. This method must be called before any other Video methods can
    /// be used.
    /// </summary>
    /// <exception cref="DirectDrawInvalidGUIDException">The GUID used to initialise DirectDraw is not a valid DirectDraw driver identifier.</exception>
    /// <exception cref="DirectDrawAlreadyCreatedException">A DirectDraw object representing this driver has already been created.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">One or more of the parameters used to initialise DirectDraw was incorrect.</exception>
    /// <exception cref="DirectDrawGenericException">DirectDraw returned an undefined error condition.</exception>
    /// <exception cref="DirectDrawNoHardwareException">Hardware-only DirectDraw object creation is not possible; the driver does not support any hardware. </exception>
    /// <exception cref="OutOfMemoryException">There's not enough memory available to initialise DirectDraw.</exception>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectDraw returned an undefined COM error.</exception>
    static void Initialise();

    /// <summary>
    /// Clears the backbuffer with the specified colour.
    /// </summary>
    /// <param name="colour"></param>
    static void Clear(Color ^colour)
    {
        try
        {
            Video::Clear(colour->R, colour->G, colour->B);
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Clears the backbuffer with the specified colour.
    /// </summary>
    /// <param name="R"></param>
    /// <param name="G"></param>
    /// <param name="B"></param>
    /// <exception cref="System::Runtime::InteropServices::COMException">An unspecified COM error was returned.</exception>
    /// <exception cref="DirectDrawGenericException">DirectDraw returned an unspecified error condition.</exception>
    /// <exception cref="DirectDrawInvalidClipListException">DirectDraw does not support the provided clip list.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawInvalidRectException">the rectangle coordinates used by the surface were invalid.</exception>
    /// <exception cref="DirectDrawNoAlphaHardwareException">no alpha acceleration hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoBlitHardwareException">no blitter hardware is present.</exception>
    /// <exception cref="DirectDrawNoClipListException">no clip list is available.</exception>
    /// <exception cref="DirectDrawNoDDRasterOperationHardwareException">no DirectDraw raster operation (ROP) hardware is available.</exception>
    /// <exception cref="DirectDrawNoMirrorHardwareException">the operation cannot be carried out because no mirroring hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoRasterOperationHardwareException">the operation cannot be carried out because no appropriate raster operation hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoRotationHardwareException">the operation cannot be carried out because no rotation hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoStretchHardwareException">the operation cannot be carried out because there is no hardware support for stretching.</exception>
    /// <exception cref="DirectDrawNoZBufferHardwareException">the operation cannot be carried out because there is no hardware support for Z-buffers.</exception>
    /// <exception cref="DirectDrawSurfaceBusyException">access to the surface is refused because the surface is locked by another thread.</exception>
    /// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone.</exception>
    /// <exception cref="DirectDrawUnsupportedException">the operation is not supported.</exception>
    /// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation is incomplete.</exception>
    static void Clear(unsigned int R, unsigned int G, unsigned int B);

    /// <summary>
    /// Attempts to load a bitmap as a DirectDraw surface.
    /// </summary>
    /// <param name="path">The image to load.</param>
    /// <returns>The specified bitmap as a Surface object.</returns>
    /// <exception cref="System::ArgumentException"><i>path</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>path</i> is <b>null</b>.</exception>
    /// <exception cref="Win32Exception">The function was unable to load the image at the specified path.</exception>
    static Surface ^CreateSurface(String ^path);

    /// <summary>
    /// Draws one or more clipped lines using the Bresenham line algorithm.
    /// </summary>
    /// <param name="lines">the line(s) to draw to the backbuffer.</param>
    /// <exception cref="ArgumentException"></exception>
    /// <exception cref="ArgumentNullException"></exception>
    /// <exception cref="COMException"></exception>
    /// <exception cref="DirectDrawGenericException">DirectDraw returned an unspecified error condition.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawInvalidRectException">the rectangle coordinates used by the surface were invalid.</exception>
    /// <exception cref="DirectDrawNotLockedException">Attempted to unlock a surface that wasn't locked.</exception>
    /// <exception cref="DirectDrawSurfaceBusyException">access to the surface is refused because the surface is locked by another thread.</exception>
    /// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone.</exception>
    /// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation is incomplete.</exception>
    /// <exception cref="OutOfMemoryException">Not enough memory available to complete the operation.</exception>
    static void DrawLines(array<Line ^> ^lines);

    /// <summary>
    /// Flips the buffers, causing the contents of the backbuffer to be rendered to the screen.
    /// </summary>
    /// <exception cref="COMException">DirectDraw returned an unspecified COM error.</exception>
    /// <exception cref="DirectDrawGenericException">DirectDraw returned an unspecified error condition.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawNoFlipHardwareException">flipping visible surfaces is not supported by the video hardware.</exception>
    /// <exception cref="DirectDrawNotFlippableException">Attempting to flip a surface that cannot be flipped.</exception>
    /// <exception cref="DirectDrawSurfaceBusyException">access to the surface is refused because the surface is locked by another thread.</exception>
    /// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone.</exception>
    /// <exception cref="DirectDrawUnsupportedException">the operation is not supported.</exception>
    /// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation is incomplete.</exception>
    static void Flip();

    /// <summary>
    /// Draws the specified surface to the backbuffer at the specified location.
    /// Surfaces can use the system-wide colour key for the blit mask.
    /// </summary>
    /// <param name="x">where to render the image on the x axis.</param>
    /// <param name="y">where to render the image on the y axis.</param>
    /// <param name="surface">The image to be rendered to the backbbuffer.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException">An unspecified COM error was returned.</exception>
    /// <exception cref="DirectDrawGenericException">DirectDraw returned an unspecified error condition.</exception>
    /// <exception cref="DirectDrawInvalidClipListException">DirectDraw does not support the provided clip list.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawInvalidRectException">the rectangle coordinates used by the surface were invalid.</exception>
    /// <exception cref="DirectDrawNoAlphaHardwareException">no alpha acceleration hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoBlitHardwareException">no blitter hardware is present.</exception>
    /// <exception cref="DirectDrawNoClipListException">no clip list is available.</exception>
    /// <exception cref="DirectDrawNoDDRasterOperationHardwareException">no DirectDraw raster operation (ROP) hardware is available.</exception>
    /// <exception cref="DirectDrawNoMirrorHardwareException">the operation cannot be carried out because no mirroring hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoRasterOperationHardwareException">the operation cannot be carried out because no appropriate raster operation hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoRotationHardwareException">the operation cannot be carried out because no rotation hardware is present or available.</exception>
    /// <exception cref="DirectDrawNoStretchHardwareException">the operation cannot be carried out because there is no hardware support for stretching.</exception>
    /// <exception cref="DirectDrawNoZBufferHardwareException">the operation cannot be carried out because there is no hardware support for Z-buffers.</exception>
    /// <exception cref="DirectDrawSurfaceBusyException">access to the surface is refused because the surface is locked by another thread.</exception>
    /// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone.</exception>
    /// <exception cref="DirectDrawUnsupportedException">the operation is not supported.</exception>
    /// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation is incomplete.</exception>
    static void Blit(int x, int y, Surface ^surface);

    /// <summary>
    /// Attempts to restore the primary and secondary buffers. Video::Restore() should only be called when a Video function
    /// throws DirectDrawSurfaceLostException. If the method fails, DirectDraw has to be shutdown and reinitialised.
    /// </summary>
    /// <exception cref="System::Runtime::InteropServices::COMException">An unspecified COM error was returned.</exception>
    /// <exception cref="DirectDrawGenericException">DirectDraw returned an unspecified error condition.</exception>
    /// <exception cref="DirectDrawImplicitlyCreatedException">the surface cannot be restored because it is an implicitly created surface.</exception>
    /// <exception cref="DirectDrawIncompatiblePrimarySurfaceException">the primary surface creation request does not match the existing primary surface.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawNoExclusiveModeException">exclusive mode is required to complete the operation.</exception>
    /// <exception cref="DirectDrawUnsupportedException">the operation is not supported.</exception>
    /// <exception cref="DirectDrawWrongModeException">surface cannot be restored because it was created in a different mode.</exception>
    /// <exception cref="System::OutOfMemoryException">Not enough memory available to complete the operation.</exception>
    static void Restore();

    /// <summary>
    /// Creates a fullscreen window at the specified resolution.
    /// </summary>
    /// <param name="hWnd">The handle of the window that's to be used.</param>
    /// <param name="width">the desired width in pixels.</param>
    /// <param name="height">the desired height in pixels.</param>
    /// <param name="bitsPerPixel">the desired bit count.</param>
    /// <exception cref="System::ArgumentNullException"><i>hWnd</i> is <b>null</b>.</exception>
    /// <remarks>Video currently only supports 32 bit colour.</remarks>
    static void SetDisplayMode(HWND hWnd, const unsigned int width, const unsigned int height, const unsigned int bitsPerPixel);
    
    /// <summary>
    /// Shutsdown the video subsystem. Once this method has been called, Video::Initialise() must
    /// be called again before any the methods in the Video class can be used.
    /// </summary>
    static void Shutdown()
    {
        SafeRelease(lpDDSPrimarySurface);
        //SafeRelease(lpDDSSecondarySurface);
        SafeRelease(lpDDClipper);
        SafeRelease(lpDD);
    }
};