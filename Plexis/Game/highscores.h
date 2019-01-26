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
#include "highscorerecord.h"

using namespace System::Collections::Generic;
using namespace System::IO;

/// <summary>
/// Acts as a database for the games highsores. If any other object in the game requires highscore
/// data, it has to request it from this class. When a new highscore is given to the class, it updates
/// both its records in memory and the highscores stored on disk.
/// </summary>
public ref class HighScores abstract sealed
{
private:
    // Where all the high scores are kept during gameplay.
    // A list is used instead of an array simply so if things change in the future,
    // I don't have to worry about changing the size of the array.
    static List<HighScoreRecord ^> ^highScores;

    static FileStream ^fileStream;
    static BinaryReader ^binaryReader;
    static BinaryWriter ^binaryWriter;

    static const int defaultNumberOfEntries = 10;
public:
    static HighScores()
    {
        HighScores::highScores = gcnew List<HighScoreRecord ^>();
        array<unsigned char, 1> ^asciiString = Encoding::ASCII->GetBytes("DEFAULT");

        // initialise the highscore list to defaults; this will be used if there's
        // any problems reading in the highscores from the highscores file.
        for(int i = 0; i < defaultNumberOfEntries; i++)
        {
            HighScoreRecord ^defaultRecord = gcnew HighScoreRecord();
            defaultRecord->Score = 0;
            defaultRecord->SetPlayerName(asciiString);

            highScores->Add(defaultRecord);
        }
    }

    /// <summary>
    /// Attempts to read the highscores into memory. This should be called only once during the lifetime of the 
    /// game, unless Highscores::Shutdown() is called.
    /// </summary>
    /// <param name="filename">the path of the highscores file.</param>
    static void Initialise(String ^filename);

    /// <summary>
    /// Places the specified player name and score at the specified position 
    /// </summary>
    /// <param name="row">the desired zero-based position of the new entry on the high score table.</param>
    /// <param name="playerName">the player name to associate with the high score.</param>
    /// <param name="newHighScore">the high score to display.</param>
    /// <exception cref="System::ArgumentNullException"><i>playerName</i> is <b>null</b>.</exception>
    /// <exception cref="System::ArgumentOutOfRangeException"><i>row</i> is greater than the number of entries on the high score table.</exception>
    /// <exception cref="System::IO::IOException">an unspecified I/O error occured while writing to disk.</exception>
    static void Update(unsigned int row, array<unsigned char, 1> ^playerName, unsigned int newHighScore);


    /// <summary>
    /// Returns the score on the specified position of the high score table.
    /// </summary>
    /// <param name="position">the position of the desired score on the table.</param>
    /// <returns>the score by the looks of things.</returns>
    /// <exception cref="System::ArgumentOutOfRangeException"><i>position</i> is greater than the number of entries on the high score table.</exception>
    static unsigned int GetHighScore(unsigned int position)
    {
        if(position < highScores->Count)
        {
            return highScores[position]->Score;
        }
        else
        {
            throw gcnew ArgumentOutOfRangeException("position");
        }
    }

    /// <summary>
    /// Returns the player name belonging to the specified position on the high score table.
    /// </summary>
    /// <param name="row">the desired names position on the high score table.</param>
    /// <returns>the player name as a string.</returns>
    /// <exception cref="System::ArgumentOutOfRangeException"><i>row</i> is greater than the number 
    /// of positions on the high score table.</exception>
    static String ^GetPlayerName(unsigned int row)
    {
        if(row < highScores->Count)
        {
            return highScores[row]->PlayerNameAsString();
        }
        else
        {
            throw gcnew ArgumentOutOfRangeException("row");
        }
    }

    /// <summary>
    /// Checks if a score is greater than any of the existing scores on the high score table, from 
    /// the top down.
    /// </summary>
    /// <param name="score">the score to verify.</param>
    /// <returns>-1 if the score isn't a high score, else the position on the high score table where
    /// the score belongs.</returns>
    static int IsAHighScore(unsigned int score)
    {
        for(int i = 0; i < highScores->Count; i++)
        {
            if(score > highScores[i]->Score)
            {
                return i;
            }
        }

        return -1;
    }

    /// <summary>
    /// Closes the highscores file and frees the memory allocated by the class. When this function is called,
    /// none of HighScore's methods can be called until the object is reinitialised via HighScores::Initialise().
    /// </summary>
    static void Shutdown()
    {
        binaryWriter->Close();
        binaryReader->Close();
        fileStream->Close();

        delete binaryWriter;
        delete binaryReader;
        delete fileStream;
    }
};