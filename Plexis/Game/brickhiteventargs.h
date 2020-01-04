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
using namespace System::Diagnostics;

/// <summary>
/// Contains the data required to be passed to a brick's Death event.
/// </summary>
public ref class BrickHitEventArgs : EventArgs
{
	//String ^name;
	System::Drawing::Point tileCoordinates;
	System::Drawing::Point screenCoordinates; // needed for spawning a powerup
	unsigned int flags = 0;
public:
	/// <summary>
	/// Instantiates a new instance of BrickHitEventArgs with the name and coordinates
	/// of the brick that's been hit.
	/// </summary>
	/// <param name="coordinates">the bricks tile coordinates.</param>
	/// <param name="explode">true if the brick is exploding, otherwise false.</param>
	BrickHitEventArgs(System::Drawing::Point tileCoordinates, System::Drawing::Point screenCoordinates, unsigned int flags) : EventArgs()
	{
		//Debug::Assert(brickName != nullptr || brickName != String::Empty);
		//name = brickName;

		this->tileCoordinates = tileCoordinates;
		this->screenCoordinates = screenCoordinates;
		this->flags = flags;
	}
	
	/// <summary>
	/// Gets the tile coordinates of the brick 
	/// </summary>
	property System::Drawing::Point TileCoordinates
	{
		System::Drawing::Point get() { return tileCoordinates; }
	}

	/// <summary>
	/// Gets the screen coordinates of the brick 
	/// </summary>
	property System::Drawing::Point ScreenCoordinates
	{
		System::Drawing::Point get() { return screenCoordinates; }
	}

	/// <summary>
	/// Gets a value indicating whether the brick is exploding or not.
	/// </summary>
	property unsigned int Flags
	{
		unsigned int get() { return flags; }
	}
	/// <summary>
	/// Gets the name of the brick
	/// </summary>
	/*property String ^Name
	{
		String ^get() { return name; }
	}*/

};