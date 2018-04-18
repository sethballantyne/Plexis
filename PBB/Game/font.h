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

using namespace System;

/// <summary>
/// Class for loading and rendering bitmap fonts. 1 instance encapsulates 1 font, so for multiple fonts
/// you'll be needing multiple instances.
/// </summary>
public ref class Font
{
private:
    // DirectDraw surface that contains the font to be rendered.
    Surface ^fontSurface;

    // acts as a look up table for the position of each character so it doesn't 
    // have to be computed during rendering.
    array<System::Drawing::Point, 1> ^glyphPositions;

    // the width and height of each character in pixels. It's assumed every character
    // in the bitmap is of the same dimensions.
    unsigned int glyphWidth;
    unsigned int glyphHeight;

    // because I can't call an overloaded constructor inside another constructor.
    void Init(String ^name, Surface ^fontBitmap, unsigned int glyphWidth, unsigned int glyphHeight);

public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="name"></param>
    /// <param name="fontBitmap"></param>
    /// <param name="glyphWidth"></param>
    /// <param name="glyphHeight"></param>
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
    /// 
    /// </summary>
    /// <param name="name"></param>
    /// <param name="fontBitmap"></param>
    /// <param name="glyphSize"></param>
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
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="text"></param>
    void Render(int x, int y, String ^text, ...array<Object ^> ^args);
};