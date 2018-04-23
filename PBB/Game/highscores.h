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
public:
    HighScores()
    {
        HighScores::highScores = gcnew List<HighScoreRecord ^>();
    }

    /// <summary>
    /// Attempts to read the highscores into memory. This should be called only once during the lifetime of the 
    /// game, unless Highscores::Shutdown() is called.
    /// </summary>
    /// <param name="filename">the path of the highscores file.</param>
    /// <exception cref="System::ArgumentException"><i>filename</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>filename</i> is <b>null</b>.</exception>
    /// <exception cref="System::IO::DirectoryNotFoundException">the specified path is invalid.</exception>
    /// <exception cref="System::IO::EndOfStreamException">prematurely reached the end of the file while trying to read the highscores into memory.</exception>
    /// <exception cref="System::IO::IOException">an unspecified I/O error occured.</exception>
    /// <exception cref="System::NotSupportedException"><i>filename</i> refers to a non-file device.</exception>
    /// <exception cref="System::IO::PathTooLongException">the length of <i>filename</i> exceeds the maximum length allowed for a path.</exception>
    /// <exception cref="System::UnauthorizedAccessException">access to the specified path was denied by the operating system.</exception>
    static void Initialise(String ^filename);
};