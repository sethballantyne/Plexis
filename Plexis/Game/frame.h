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

/// <summary>
/// Represents a single sprite frame.
/// </summary>
/// <remarks>
/// the bounding box coordinates are image coordinates in relation to the coordinates
/// specified in the <i>Frame</i> property. If the bounding box has its X property
/// set to 2, that means the bounding box begins 2 pixels into the image used for this frame.
/// </remarks>
public ref class Frame
{
private:
    // the frames coordinates on the sprite sheet
    System::Drawing::Rectangle frame;

    // the location of the bounding box in relation to frame; uses IMAGE coordinates.
    // if boundingBox.X = 2, then that would mean the bounding box begins
    // 2 pixels in to the image.
    System::Drawing::Rectangle boundingBox;

public:
    /// <summary>
    /// Creates a new instance of Frame using the specified frame and bounding box coordinates
    /// </summary>
    /// <param name="frame">the location of this frames image on the sprite sheet</param>
    /// <param name="boundingBox">the image coordinates of the bounding box for this frame.</param>
    Frame(System::Drawing::Rectangle frame, System::Drawing::Rectangle boundingBox)
    {
        this->frame = frame;
        this->boundingBox = boundingBox;
    }

    /// <summary>
    /// Gets the sprite sheet coordinates for this frame.
    /// </summary>
    property System::Drawing::Rectangle% Coordinates
    {
        System::Drawing::Rectangle% get()
        {
            return frame;
        }
    }

    /// <summary>
    /// Gets the bounding box coordinates for this frame.
    /// </summary>
    property System::Drawing::Rectangle% BoundingBox
    {
        System::Drawing::Rectangle% get()
        {
            return boundingBox;
        }
    }
};
