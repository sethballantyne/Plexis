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
/// <summary>
/// Represents the gameplay state the game is in.
/// </summary>
public enum GameState
{
	/// <summary>
	/// level has been loaded and ball is attached to paddle. 
	/// The game is waiting for the player to hit the fire button.
	/// </summary>
	NewLevel,

	/// <summary>
	/// Player has left the NewLevel game state and now the Ball is in motion, not attached to paddle.
	/// </summary>
	Playing,

	/// <summary>
	/// Player has pressed the pause key and the game is now paused.
	/// </summary>
	Paused,

	/// <summary>
	/// Level has been completed and the game is now in a transition phase.
	/// </summary>
	LevelComplete,

	/// <summary>
	/// The player has died, no more lives are remaining and the game has ended.
	/// </summary>
	GameOver,

	/// <summary>
	/// The player and ball are about to be reset to their default positions.
	/// </summary>
	PlayerReset
};