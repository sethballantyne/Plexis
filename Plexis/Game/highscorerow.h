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
#include "label.h"

using namespace System::Drawing;

/// <summary>
/// represents a single row on the HighScoreTable control.
/// </summary>
public ref class HighScoreRow
{
private:
    Label ^rowIndexLabel;
    Label ^playerNameLabel;
    Label ^scoreLabel;
public:
    /// <summary>
    /// Initialises a new instance of HighScoreRow.
    /// </summary>
    /// <param name="rowNumberFont">the name of the font that will be used to render the rank text.</param>
    /// <param name="indexPosition">the screen position where the rank should be rendered.</param>
    /// <param name="playerNameFont">the name of the font that will be used to render the players name.</param>
    /// <param name="playerNamePosition">the screen position where the players name should be rendered.</param>
    /// <param name="scoreFont">the name of the font that will be used to render the players score.</param>
    /// <param name="scorePosition">the screen position where the score should be rendered.</param>
    /// <exception cref="System::ArgumentException">either <i>rowNumberFont</i>, or <i>playerNameFont</i>, 
    /// or <i>scoreFont</i> evaluate to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException">either <i>rowNumberFont</i>, or <i>playerNameFont</i>, 
    /// or <i>scoreFont</i> are <b>null</b>.</exception>
    /// <exception cref="ResourceNotFoundException">the font specified in either <i>rowNumberFont</i>, or <i>playerNameFont</i>, 
    /// or <i>scoreFont</i> doesn't exist within the resource manager.</exception>
    HighScoreRow(String ^rowNumberFont, Point ^indexPosition, String ^playerNameFont,
        Point ^playerNamePosition, String ^scoreFont, Point ^scorePosition);

    /// <summary>
    /// Blits the rank, players name and score to the backbuffer.
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
            rowIndexLabel->Render();
            playerNameLabel->Render();
            scoreLabel->Render();
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Gets the label used to display the scores rank on the high score table. 
    /// </summary>
    property Label ^Index
    {
        Label ^get()
        {
            return rowIndexLabel;
        }
    }

    /// <summary>
    /// Gets the label used to display the players name on the high score table.
    /// </summary>
    property Label ^PlayerName
    {
        Label ^get()
        {
            return playerNameLabel;
        }
    }

    /// <summary>
    /// Gets the label used to display the score on the high score table.
    /// </summary>
    property Label ^Score
    {
        Label ^get()
        {
            return scoreLabel;
        }
    }
};