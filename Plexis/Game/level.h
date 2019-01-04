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
#include "brick.h"
#include "entitymanager.h"

using namespace System;

/// <summary>
/// Represents a level within the game.
/// A level is defined as being a collection of 1 or more bricks, not the ball, paddle etc.
/// </summary>
public ref class Level
{
private:
    array<Brick ^, 2>^ level;

public:
    /// <summary>
    /// Creates a new instance of <see cred="Level"/> with the specified dimensions. 
    /// </summary>
    /// <param name="width">the width of the level in bricks.</param>
    /// <param name="height">the height of the level in bricks.</param>
    Level(unsigned int width, unsigned int height)
    {
        level = gcnew array<Brick ^, 2>(width, height);
    }

    /// <summary>
    /// Assigns a deep cloneed instance of the specified brick to the specified coordinates.
    /// </summary>
    /// <param name="x">the position on the X axis to place the brick.</param>
    /// <param name="y">the position on the Y axis to place the brick.</param>
    /// <param name="brickName">The name of the brick to assign to the specified coordinates.</param>
    /// <remarks>
    /// the X and Y coordinates are in tile coordinates, not screen coordinates. The specified coordinates
    /// will be converted to screen coordinates during the methods execution.
    /// </remarks>
    void PutBrickAt(unsigned int x, unsigned int y, String ^brickName)
    {
		if (nullptr == brickName)
		{
			throw gcnew ArgumentNullException("brickName");
		}
		else if (String::Empty == brickName)
		{
			throw gcnew ArgumentException("brickName evaluates to String::Empty.");
		}

        level[x, y] = EntityManager::GetEntity<Brick ^>(brickName);
		if (nullptr == level[x, y])
		{
			throw gcnew UnknownEntityException(String::Format("Unknown entity {0}. Check the entities file to see if it's present.", brickName));
		}

        int spriteWidth = level[x, y]->Sprite->CurrentFrame->Coordinates.Width;
        int spriteHeight = level[x, y]->Sprite->CurrentFrame->Coordinates.Height;

        level[x, y]->Position = System::Drawing::Point(x * spriteWidth, y * spriteHeight);
    }

    /// <summary>
    /// Gets the specified brick at the specified tile coordinates.
    /// </summary>
    /// <param name="x">the bricks X coordinate.</param>
    /// <param name="y">the bricks Y coordinate.</param>
    /// <returns>The <see cref="Brick"/> instance at the specified coordinates, or <b>nullptr</b> if no instance exists.</returns>
    Brick ^GetBrickAt(unsigned int x, unsigned int y)
    {
        return level[x, y];
    }

    /// <summary>
    /// Renders the level to the back buffer.
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
    void Render()
    {
        try
        {
            for(int i = 0; i < level->GetLength(0); i++)
            {
                for(int j = 0; j < level->GetLength(1); j++)
                {
                    if(nullptr != level[i, j])
                    {
                        level[i, j]->Sprite->Render();
                    }
                }
            }
        }
        catch(...)
        {
            throw;
        }
    }
};
