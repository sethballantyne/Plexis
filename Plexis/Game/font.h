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
#include "surface.h"

/// <summary>
/// Class for loading and rendering fixed-width bitmap fonts. 1 instance encapsulates 1 font, so for multiple fonts
/// you'll be needing multiple instances.
/// </summary>
public ref class Font
{
private:
    // DirectDraw surface that contains the font to be rendered.
    Surface ^fontSurface;

    // identifier used by the resource manager to identify the font.
    String ^name;

    // acts as a look up table for the position of each character so it doesn't 
    // have to be computed during rendering.
    array<System::Drawing::Point, 1> ^glyphPositions;

    // the width and height of each character in pixels. It's assumed every character
    // in the bitmap is of the same dimensions.
    unsigned int glyphWidth;
    unsigned int glyphHeight;

    /// <summary>
    /// Puts the class into a useable state; designed to be called within a constructor.
    /// </summary>
    /// <param name="name">a string that the resource manager will use to identify the font.
    /// <i>name</i> should be unique.</param>
    /// <param name="fontBitmap">the fonts DirectDraw surface.</param>
    /// <param name="glyphWidth">the width of each character in the font.</param>
    /// <param name="glyphHeight">the height of each character in the font.</param>
    /// <exception cref="System::ArgumentException"><i>font</i> evaluates to String::Empty, or 
    /// <i>glyphWidth</i> is greater than the width of the bitmap, or <i>glyphHeight</i> is greater than the
    /// height of the bitmap.</exception>
    /// <exception cref="System::ArgumentNullException">either <i>name</i> or <i>fontBitmap</i> are <b>null</b>.</exception>
    /// <remarks>the only reason this exists is because I can't call an overloaded constructor
    /// inside another constructor belonging to the same class.</remarks>
    void Init(String ^name, Surface ^fontBitmap, unsigned int glyphWidth, unsigned int glyphHeight);

public:
    /// <summary>
    /// Creates a new instance of Font.
    /// </summary>
    /// <param name="name">a string that the resource manager will use to identify the font.
    /// <i>name</i> should be unique.</param>
    /// <param name="fontBitmap">the fonts DirectDraw surface.</param>
    /// <param name="glyphWidth">the width of each character in the font.</param>
    /// <param name="glyphHeight">the height of each character in the font.</param>
    /// <exception cref="System::ArgumentException"><i>font</i> evaluates to String::Empty, or 
    /// <i>glyphWidth</i> is greater than the width of the bitmap, or <i>glyphHeight</i> is greater than the
    /// height of the bitmap.</exception>
    /// <exception cref="System::ArgumentNullException">either <i>name</i> or <i>fontBitmap</i> are <b>null</b>.</exception>
    Font(String ^name, Surface ^fontBitmap, unsigned int glyphWidth, unsigned int glyphHeight)
    {
        try
        {
            Init(name, fontBitmap, glyphWidth, glyphHeight);
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Creates a new instance of Font.
    /// </summary>
    /// <param name="name">a string that the resource manager will use to identify the font.
    /// <i>name</i> should be unique.</param>
    /// <param name="fontBitmap">the fonts DirectDraw surface.</param>
    /// <param name="glyphSize">the dimensions of each character in the font.</param>
    /// <exception cref="System::ArgumentException"><i>font</i> evaluates to String::Empty, or 
    /// <i>glyphWidth</i> is greater than the width of the bitmap, or <i>glyphHeight</i> is greater than the
    /// height of the bitmap.</exception>
    /// <exception cref="System::ArgumentNullException">either <i>name</i> or <i>fontBitmap</i> are <b>null</b>.</exception>
    Font(String ^name, Surface ^fontBitmap, System::Drawing::Size glyphSize)
    {
        try
        {
            Init(name, fontBitmap, glyphSize.Width, glyphSize.Height);
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Uses the bitmap font to render the specified string to the backbuffer.
    /// </summary>
    /// <param name="x">the x coordinate to draw to.</param>
    /// <param name="y">the y coordinate to draw to.</param>
    /// <param name="text">the text to render using the font.</param>
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
    /// <remarks>this method only renders ASCII characters within the range 32 (Space) to 126 (~). 0 (NULL) is treated
    /// as a terminator, so when it's encountered within the array the method immediately returns.</remarks>
    void Render(int x, int y, String ^text, ...array<Object ^> ^args);

    /// <summary>
    /// Uses the bitmap font to render the specified array ASCII characters to the backbuffer.
    /// </summary>
    /// <param name="x">the x coordinate to draw to.</param>
    /// <param name="y">the y coordinate to draw to.</param>
    /// <param name="str">the characters to render.</param>
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
    /// <remarks>this method only renders ASCII characters within the range 32 (Space) to 126 (~). 0 (NULL) is treated
    /// as a terminator, so when it's encountered within the array the method immediately returns.</remarks>
    void Render(int x, int y, array<unsigned char, 1> ^str);

    /// <summary>
    /// Gets the fonts name.
    /// </summary>
    property String ^Name
    {
        String ^get()
        {
            return this->name;
        }
    }

    /// <summary>
    /// Gets the Surface containing the bitmap font.
    /// </summary>
    property Surface ^FontSurface
    {
        Surface ^get()
        {
            return this->fontSurface;
        }
    }
};