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
#include "highscorerow.h"

HighScoreRow::HighScoreRow(String ^rowNumberFont, Point ^indexPosition, String ^playerNameFont,
    Point ^playerNamePosition, String ^scoreFont, Point ^scorePosition)
{
    if(nullptr == rowNumberFont)
    {
        throw gcnew ArgumentNullException("rowNumberFont");
    }
    else if(nullptr == playerNameFont)
    {
        throw gcnew ArgumentNullException("playerNameFont");
    }
    else if(nullptr == scoreFont)
    {
        throw gcnew ArgumentNullException("scoreFont");
    }
   
    if(String::Empty == rowNumberFont)
    {
        throw gcnew ArgumentException("rowNumberFont evaluates to String::Empty.");
    }
    else if(String::Empty == playerNameFont)
    {
        throw gcnew ArgumentException("playerNameFont evaluats to String::Empty.");
    }
    else if(String::Empty == scoreFont)
    {
        throw gcnew ArgumentException("scoreFont evaluates to String::Empty.");
    }

    try
    {
        rowIndexLabel = gcnew Label(indexPosition->X, indexPosition->Y, rowNumberFont, nullptr);
        scoreLabel = gcnew Label(scorePosition->X, scorePosition->Y, scoreFont, nullptr);
        playerNameLabel = gcnew Label(playerNamePosition->X, playerNamePosition->Y, playerNameFont, nullptr);
    }
    catch(...)
    {
        throw;
    }
}