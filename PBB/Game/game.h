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

#include <windows.h>

/// <summary>
/// Handles the game state, seperating it from the application layer. 
/// No actual logic related to game play is handled by Game; that's the domain of GameView.
/// </summary>
public ref class Game abstract
{

public:
    static Game()
    {
        IsRunning = true;
        IsWindowActive = false;
    }
    /// <summary>
    /// Initialises each subsystem and loads all resources and config information, putting the game 
    /// into a playable state.
    /// </summary>
    /// <param name="hInstance">The application instance handle.</param>
    /// <param name="hWnd">the handle of the window the game will be played in.</param>
    static void Initialise(HINSTANCE hInstance, HWND hWnd);

    /// <summary>
    /// Renders the contents of the backbuffer to the screen.
    /// </summary>
    static void Render();

    /// <summary>
    /// Shuts down the game, instructing all subsystems to perform clean up and release any memory that's 
    /// acquired.
    /// </summary>
    static void Shutdown();

    /// <summary>
    /// Updates the game state and polls user input.
    /// Update doesn't draw anything to the screen but rather sets up the scene in preparation to be drawn.
    /// </summary>
    static void Update();

    static property bool IsRunning;
    static property bool IsWindowActive;
};