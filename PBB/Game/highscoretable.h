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

public ref class HighScoreTable : Control
{
private:
    // each row that makes up the table. A list is being used instead of an array
    // so if the number of rows that makes up the table happens to change,
    // there's no numbers to change.
    List<HighScoreRow ^> ^rows;

public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="selectedIndex"></param>
    /// <param name="numberOfRows"></param>
    /// <param name="vertialSpacing"></param>
    /// <param name="indexXPosition"></param>
    /// <param name="rowNumberFont"></param>
    /// <param name="PlayerNameXPosition"></param>
    /// <param name="playerNameFont"></param>
    /// <param name="scoreXPosition"></param>
    /// <param name="scoreFont"></param>
    /// <param name="parentContainer"></param>
    HighScoreTable(int x, int y, int numberOfRows, int vertialSpacing,
        int indexXPosition, String ^rowNumberFont, int PlayerNameXPosition, String ^playerNameFont,
        int scoreXPosition, String ^scoreFont);

    void ReceiveSceneArgs(array<Object ^, 1> ^args) override
    {
    }

    void Update(Keys ^keyboardState, Mouse ^mouseState) override
    {

    }

    void Render() override
    {
        for(int i = 0; i < rows->Count; i++)
        {
            rows[i]->Render();
        }
    }
};