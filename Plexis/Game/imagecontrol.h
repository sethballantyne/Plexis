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
#include "control.h"
#include "surface.h"
#include "resourcemanager.h"
#include "video.h"

/// <summary>
/// GUI control for display Bitmap images.
/// </summary>
public ref class ImageControl : public Control
{
private:
    // the image to display.
    Surface ^image;

    // position where the image is going to be rendered on the screen.
    System::Drawing::Point ^location;

public:
    /// <summary>
    /// Initialises a new instance of ImageControl with the specified screen coordinates and image.
    /// </summary>
    /// <param name="x">the images screen position on the x axis.</param>
    /// <param name="y">the images screen position on the y axis.</param>
    /// <param name="imageName">the name of the image to use, minus file extension.</param>
    /// <exception cref="System::ArgumentException"><i>imageName</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>imageName</i> is <b>null</b>.</exception>
    /// <exception cref="ResourceNotFoundException">a surface with the name specified in <i>imageName</i> doesn't exist within the resource manager.</exception>
    ImageControl(int x, int y, String ^imageName) : Control(x, y)
    {
        if(nullptr == imageName)
        {
            throw gcnew ArgumentNullException("imageName");
        }

        if(String::Empty == imageName)
        {
            throw gcnew ArgumentException("imageName evaluates to String::Empty.");
        }

        location = gcnew System::Drawing::Point(x, y);

        try
        {
            image = ResourceManager::GetSurface(imageName);
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Renders the image to the backbuffer.
    /// </summary>
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
    void Render() override
    {
        try
        {
            Video::Blit(location->X, location->Y, image);
        }
        catch(...)
        {
            throw;
        }
    }
};