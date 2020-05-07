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
#include "containercontrol.h"
#include "highscorerow.h"

using namespace System::Text;

public ref class HighScoreTable : SelectableControl
{
private:
    // each row that makes up the table. A list is being used instead of an array
    // so if the number of rows that makes up the table happens to change,
    // there's no numbers to change.
    List<HighScoreRow ^> ^rows; 

	String ^rowNumberFont;
	String ^playerNameFont;
	String ^scoreFont;

	int numberOfRows;
	int x;
	int y;
	int verticalSpacing;
	int indexXPosition;
	int playerNameXPosition;
	int scoreXPosition;
	
	void PopulateTable();
public:
    /// <summary>
    /// Initialises a new instance of HighScoreTable with the specified fonts and X coordinates
    /// for each row member.
    /// </summary>
    /// <param name="x">the controls screen position on the X axis.</param>
    /// <param name="y">the controls screen position on the Y axis.</param>
    /// <param name="numberOfRows">the number of ranks the table will consist of. This value should'nt be greater than the number of entries in highscores.dat.</param>
    /// <param name="verticalSpacing">the amount of space between rows in pixels.</param>
    /// <param name="indexXPosition">the position on the X axis where each rank should be rendered.</param>
    /// <param name="rowNumberFont">the name of the font that will be used to render each rank.</param>
    /// <param name="playerNameXPosition">the position on the X axis where each player name should be rendered.</param>
    /// <param name="playerNameFont">the name of the font that will be used to render each player name.</param>
    /// <param name="scoreXPosition">the position on the X axis where each score should be rendered.</param>
    /// <param name="scoreFont">the name of the font that will be used to render each score.</param>
    /// <exception cref="System::ArgumentException">either <i>rowNumberFont</i>, or <i>playerNameFont</i>,
    ///  or <i>scoreFont</i> evaluate to String::Empty.
    /// </exception>
    /// <exception cref="System::ArgumentNullException">either <i>rowNumberFont</i>, or <i>playerNameFont</i>,
    ///  or <i>scoreFont</i> is <b>null</b>.
    /// </exception>
    /// <exception cref="ResourceNotFoundException">the font specified in either <i>rowNumberFont</i>,
    ///  or <i>playerNameFont</i>, or <i>scoreFont</i> doesn't exist within the resource manager.
    /// </exception>
    /// <remarks>When HighScoreTable calculates the spacing between rows, it uses the height of the font specified in <i>rowNumberFont</i>
    /// and assumes the fonts specified in <i>playerNameFont</i> and <i>scoreFont</i> are the same height. If the heights
    /// differ, the rows are going to be displayed incorrectly.
    /// </remarks>
    HighScoreTable(int x, int y, int numberOfRows, int verticalSpacing,
        int indexXPosition, String ^rowNumberFont, int playerNameXPosition, String ^playerNameFont,
        int scoreXPosition, String ^scoreFont);

    /// <summary>
    /// Returns the specified score as a right-aligned string. 
    /// </summary>
    /// <param name="score"></param>
    /// <returns><i>score</i> as a right-aligned 5 charater string. If <i>score</i> is less than 5 decimal places in length, the string is padded with whitespace.</returns>
    /// <remarks>The alignment is achieved by padding numbers less than 5 digits with an empty space on the left
    /// for each digit missing.
    /// when displaying highscores, we want this:
    /// 99999
    ///   999
    /// not this:
    /// 99999
    /// 999</remarks>
    String ^RightAlignScore(unsigned int score)
    {
        StringBuilder ^sb = gcnew StringBuilder(Convert::ToString(score));

        int numberOfSpacesToInsert = 5 - sb->Length;
        sb->Insert(0, " ", numberOfSpacesToInsert);
        return sb->ToString();
    }

    /// <summary>
    ///  
    /// </summary>
    /// <param name="args"></param>
    void ReceiveSceneArgs(array<Object ^, 1> ^args) override;

    /// <summary>
    /// Blits each row in the highscore table to the backbuffer.
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
        for(int i = 0; i < rows->Count; i++)
        {
            rows[i]->Render();
        }
    }

	/// <summary>
	/// 
	/// </summary>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override
    {
    }

	void UpdateTable();
};