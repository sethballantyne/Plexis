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

using namespace System;

public ref class Surface
{
private:
    LPDIRECTDRAWSURFACE7 lpDDSurface;

    // width and height of the bitmap used to produce the surface.
    // It's possible for DirectDraw to create a surface that's has a greater 
    // width or height so the distinction is important.
    unsigned int bitmapWidth;
    unsigned int bitmapHeight;

    // the path and filename of the bitmap used to create the surface.
    // these are used when the surface has to be restored by DirectDraw.
    String ^path;
    String ^filename;
    String ^name;

    System::Drawing::Size ^size;
public:
    /// <summary>
    /// Wraps a new instance of surface around the supplied DirectDraw surface.
    /// </summary>
    /// <param name="lpDDSurface"></param>
    /// <param name="width">the width of the bitmap used to create the surface.</param>
    /// <param name="height">the height of the bitmap used to create the surface.</param>
    /// <exception cref="System::ArgumentNullException"><i>lpDDSurface</i> is <b>null</b>.</exception>
    /// <remarks>Do not use the surface width when supplying values for the <i>width</i>
    /// and <i>height</i> parameters as it's possible for DirectDraw to create
    /// a surface of greater dimensions than the supplied bitmap.</remarks>
    Surface(LPDIRECTDRAWSURFACE7 lpDDSurface, unsigned int width, unsigned int height)
    {
        if(!lpDDSurface)
        {
            throw gcnew ArgumentNullException("lpDDSurface");
        }

        this->lpDDSurface = lpDDSurface;
        this->bitmapWidth = width;
        this->bitmapHeight = height;

        this->size = gcnew System::Drawing::Size(bitmapWidth, bitmapHeight);
    }

    /// <summary>
    /// Frees the memory allocated to the surface by DirectDraw.
    /// </summary>
    void Release()
    {
        if(this->lpDDSurface != NULL)
        {
            this->lpDDSurface->Release();
            this->lpDDSurface = NULL;
            /*GC::SuppressFinalize(this);*/
        }
    }

    /*!Surface()
    {
        Release();
    }

    ~Surface()
    {
        this->!Surface();
    }*/

    /// <summary>
    /// Returns a pointer to the underlying DirectDraw surface.
    /// </summary>
    property LPDIRECTDRAWSURFACE7 Data
    {
        LPDIRECTDRAWSURFACE7 get()
        {
            return this->lpDDSurface;
        }
        void set(LPDIRECTDRAWSURFACE7 value)
        {
            this->lpDDSurface = value;
        }
    }

    /// <summary>
    /// Returns the height of the bitmap.
    /// </summary>
    /// <remarks>Note that this is the height of the bitmap, not the surface height.</remarks>
    property unsigned int Height
    {
        unsigned int get()
        {
            return this->bitmapHeight;
        }
    }

    /// <summary>
    /// Gets or sets the name of the surface. 
    /// </summary>
    /// <exception cref="System::ArgumentException">the property was assigned a string consisting of whitespace or String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException">the property was assigned a <b>null</b> value.</exception>
    property String ^Name
    {
        String ^get()
        {
            return this->name;
        }

        void set(String ^value)
        {
            if(nullptr == value)
            {
                throw gcnew ArgumentNullException("value");
            }
            else if(String::IsNullOrWhiteSpace(value))
            {
                throw gcnew ArgumentException("the name of the surface can't be a null or whitespace.");
            }
            else
            {
                this->name = value;
            }
        }
    }

    /// <summary>
    /// Gets the width and height of the bitmap.
    /// </summary>
    property System::Drawing::Size ^Size
    {
        System::Drawing::Size ^get()
        {
            return this->size;
        }
    }

    /// <summary>
    /// Gets or sets the Surface's path, including the filename for the bitmap it originates from.
    /// </summary>
    property String ^Path
    {
        String ^get()
        {
            return this->path;
        }

        void set(String ^value)
        {
            this->path = value;
        }
    }

    /// <summary>
    /// Returns the width of the bitmap.
    /// </summary>
    /// <remarks>Note that this is the width of the bitmap, not the surface width.</remarks>
    property unsigned int Width
    {
        unsigned int get()
        {
            return this->bitmapWidth;
        }
    }
};