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
/// 
/// </summary>
public ref class ImageControl : public Control
{
private:
    /// <summary>
    /// 
    /// </summary>
    Surface ^image;

    /// <summary>
    /// 
    /// </summary>
    System::Drawing::Point ^location;

public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="imageName"></param>
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
    /// 
    /// </summary>
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