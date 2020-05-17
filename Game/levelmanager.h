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
#include "level.h"

/// <summary>
/// Used to retrieve levels during gameplay.
/// </summary>
public ref class LevelManager abstract sealed
{
private:
    // the name of each XML file that contains level data.
    static array<System::String ^, 1> ^levels = nullptr;

    // used for indexing the levels array.
    static unsigned int currentLevel = 0;

public:
    /// <summary>
    /// Parses the XML containing the names of each level to played in the game.
    /// </summary>
    /// <param name="xmlLevelList">The XML containing the level names.</param>
    static void Initialise(XElement ^xmlLevelList);

    /// <summary>
    /// Retrieves the next the level to played after the current one.
    /// </summary>
    /// <returns>A <see cref="Level"/> instance containing the next level to be played.</returns>
    /// <exception cref="System::FormatException">An attribute in the levels XML is supposed to contain a numeric value but it's non-numeric.</exception>
    /// <exception cref="System::OverflowException">An attribute in the levels XML contains a numeric value that exceeds the bounds of the type it's being converted to.</exception>
    /// <exception cref="System::Xml::XmlException">A required attribute is missing, or the value of an 
    /// attribute evaluates to String::Empty, or the specified coordinate for a brick is outside the bounds 
    /// specified by the levels <c>width</c> and <c>height</c> attributes.</exception>
    static Level ^GetNextLevel()
    {
		try
		{
			// the number of times we've attempted to load a level
			// before the function could return. If it evaluates to 
			// levels->Length, ALL the levels are empty and we can bail.
			// we're stuck in an inifinite loop otherwise, obviously I don't
			// want that. 
			int numTries = 0;

			Level ^newLevel = nullptr;

			while(nullptr == newLevel)
			{
				if(numTries == levels->Length)
				{
					throw gcnew Exception("All the levels are empty!");
				}

				if(currentLevel >= (unsigned int)levels->Length)
				{
					currentLevel = 0;
				}

				newLevel = ReadLevel(levels[currentLevel], false);
				currentLevel++;
				numTries++;
			}

			return newLevel;
		}
		catch(...)
		{
			throw;
		}
    }

	/// <summary>
	/// Generates the specified level.
	/// </summary>
	/// <param name="levelFile">the name of the level to generate.</param>
	/// <param name="testLevel">true if the level has been passed via the /map command line argument,
	/// otherwise false.</param>
	/// <returns>A <see cref="Level"/> instance containing the specified level.</returns>
	/// <exception cref="System::FormatException">An attribute was supposed to have a numeric value but instead contains a non-numeric value.</exception>
	/// <exception cref="System::OverflowException">An attribute was contains a numeric value that's outside its types range.</exception>
	/// <exception cref="System::Xml::XmlException">A required attribute is missing, or the value of an 
	/// attribute evaluates to String::Empty, or the specified coordinate for a brick is outside the bounds 
	/// specified by the levels <c>width</c> and <c>height</c> attributes.</exception>
	static Level ^ReadLevel(String ^levelFile, bool testLevel);

	/// <summary>
	/// Resets the level counter back to 0. When this is called, the next time LevelManager::GetNextLevel()
	/// is called, the first level will be returned.
	/// </summary>
	static void ResetLevelCounter()
	{
		currentLevel = 0;
	}
};