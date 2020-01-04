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

using namespace System;
using namespace System::Text;

/// <summary>
/// represents a single entry in the highscores file.
/// </summary>
public ref class HighScoreRecord
{
private:
    array<unsigned char, 1> ^playerName;

public:

    HighScoreRecord()
    {
        playerName = gcnew array<unsigned char, 1>(12);

        // spells ASS.
        // used as a default value. If there's entries missing from the highscore file, the default will
        // be displayed. 
        playerName[0] = 65;
        playerName[1] = 83;
        playerName[2] = 83;
    }

    /// <summary>
    /// Gets or sets the score associated with this high score table entry.
    /// </summary>
    property unsigned int Score;

    /// <summary>
    /// Attempts to convert and return the player name from a byte array to a string.
    /// </summary>
    /// <returns>the player name as a String consisting of ASCII characters.</returns>
    /// <exception cref="System::ArgumentException">the player name is <b>null</b>.</exception>
    /// <exception cref="System::ArgumentNullException">the player name contains non-ascii characters.</exception>
    String ^PlayerNameAsString()
    {
        try
        {
            return Encoding::ASCII->GetString(playerName);
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// returns the current player name assigned to the player field for this record.
    /// </summary>
    /// <returns>the current name as an array of ASCII characters.</returns>
    /// <remarks>the size of the player name will always be the length of the buffer it's stored in
    /// as its a fixed size.</remarks>
    array<unsigned char, 1> ^GetPlayerName()
    {
        return this->playerName;
    }

    /// <summary>
    /// Assigns a name to the player field for this record.
    /// </summary>
    /// <param name="value">the name to assign.</param>
    /// <exception cref="System::Exception">the length of <i>value</i> is greater than the length 
    /// of the player name buffer, or is the length of <i>value</i> is 0.</exception>
    /// <exception cref="System::ArgumentNullException"><i>value</i> is <b>null</b>.</exception>
    void SetPlayerName(array<unsigned char, 1> ^value)
    {
        if(nullptr == value)
        {
            throw gcnew ArgumentNullException("value");
        }
        else if(value->Length > this->playerName->Length || value->Length == 0)
        {
            throw gcnew ArgumentException("attempting to assign a player name that's the wrong length.");
        }
        else
        {
            Array::Copy(value, this->playerName, value->Length);
        }
    }
};
