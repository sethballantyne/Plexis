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
#include "frame.h"
#include "resourcemanager.h"
#include "video.h"

/// <summary>
/// <c>Sprite</c>is used to handle an <see cref="Entity"/>'s image and location information. 
/// Every <see cref="Entity"/> and by extension every <see cref="Sprite"/> consists of at least
/// one frame along with its associated spritesheet coordinates.
/// </summary>
public ref class Sprite
{
private:
    // the sprites position on the screen
    System::Drawing::Point position;

    // the coordinates of the sprites frames on the sprite sheet.
    // also the image coordinates for each frames bounding box.
    array<Frame ^, 1> ^frames = nullptr;

    // the sprites sprite sheet
    Surface ^image;

    // the current frame
    unsigned int currentFrameIndex = 0;

public:
    /// <summary>
    /// Creates a new instance of <see cref="Sprite"/>.
    /// </summary>
    /// <param name="x">the objects initial position on the screens X axis.</param>
    /// <param name="y">the objects initial position on the screens Y axis.</param>
    /// <param name="frames">The image positions of each frame within the spritesheet.</param>
    /// <param name="image">the name of the spritesheet to be used for retrieving the sprites frames.</param>
    Sprite(int x, int y, array<Frame ^, 1> ^frames, String ^image)
    {
        if(nullptr == frames)
        {
            throw gcnew ArgumentNullException("frames");
        }
        else if(nullptr == image)
        {
            throw gcnew ArgumentNullException("image");
        }

        if(String::Empty == image)
        {
            throw gcnew ArgumentException("image evaluates to String::Empty.");
        }

        this->position = System::Drawing::Point(x, y);
        this->image = ResourceManager::GetSurface(image);
        this->frames = frames;
    }

    /// <summary>
    /// Creates a new instance of <see cref="Sprite"/>.
    /// </summary>
    /// <param name="x">the objects initial position on the screens X axis.</param>
    /// <param name="y">the objects initial position on the screens Y axis.</param>
    /// <param name="frames">The image coordinates of each frame within the spritesheet.</param>
    /// <param name="image">the <see cref="Surface"/> instance that contains the objects frames.</param>
    Sprite(int x, int y, array<Frame ^, 1> ^frames, Surface ^image)
    {
        if(nullptr == frames)
        {
            throw gcnew ArgumentNullException("frames");
        }
        else if(nullptr == image)
        {
            throw gcnew ArgumentNullException("image");
        }

        this->position = System::Drawing::Point(x, y);
        this->image = image;
        this->frames = frames;
    }

    /// <summary>
    /// Gets the frame and bounding box coordinates associated with this sprite.
    /// </summary>
    /// <returns>An array of <see cref="Frame"/> objects.</returns>
    array<Frame ^, 1> ^GetFrames()
    {
        return frames;
    }

    /// <summary>
    /// Renders the sprites current frame to the backbuffer.
    /// </summary>
    void Render()
    {
        try
        {
            Video::Blit(frames[currentFrameIndex]->Coordinates, position, image);
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Gets the number of frames used by this sprite.
    /// </summary>
    property unsigned int FrameCount
    {
        unsigned int get()
        {
            return (unsigned int)frames->Length;
        }
    }

    /// <summary>
    /// Gets or sets the current frame as an integer.
    /// </summary>
    property unsigned int CurrentFrameIndex
    {
        unsigned int get()
        {
            return currentFrameIndex;
        }
		void set(unsigned int value)
		{
			System::Diagnostics::Debug::Assert(value < (unsigned int) frames->Length);
			currentFrameIndex = value;
		}
    }

    /// <summary>
    /// Gets the current frame and bounding box coordinates.
    /// </summary>
    /// <remarks>
    /// The bounding box coordinates are in relation the frames image; if the bounding box
    /// has an X value of 3, that means it's 3 pixels into the frames X axis: the bounding box
    /// coordinates are image coordinates, not screen coordinates.
    /// </remarks>
    property Frame^ CurrentFrame
    {
        Frame^ get()
        {
            return GetFrames()[currentFrameIndex];
        }
    }

    /// <summary>
    /// Gets the <see cref="Surface"/> instance associated with this sprite.
    /// </summary>
    property ::Surface ^Surface
    {
        ::Surface ^get()
        {
            return image;
        }
    }

    /// <summary>
    /// Gets a current screen position of the sprite as a <see cref="System::Drawing::Point"/> reference.
    /// </summary>
    property System::Drawing::Point %Position
    {
        System::Drawing::Point %get()
        {
            return this->position;
        }
    }

	///<summary>
	///Gets the center of the current frame in screen coordinates.
	///</summary>
	System::Drawing::Point GetCenter(unsigned int frame)
	{
		if(frame < this->GetFrames()->Length)
		{
			System::Drawing::Point p;

			p.X = this->Position.X + (this->GetFrames()[frame]->BoundingBox.Width / 2);
			p.Y = this->Position.Y + (this->GetFrames()[frame]->BoundingBox.Height / 2);

			return p;
		}
		else
		{
			throw gcnew Exception("Sprite::GetCenter: frame is out of bounds.");
		}
	}
};
