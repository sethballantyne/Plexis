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

#include "audio.h"
#include "exceptions.h"
#include "game.h"
#include "logmanager.h"
#include "video.h"

void Game::RestoreSurfaces()
{
    try
    {
        Video::Restore();
    }
    catch(DirectDrawWrongModeException ^)
    {
        try
        {
            Video::SetDisplayMode(Game::gamehWnd, windowWidth, windowHeight, bitsPerPixel);
        }
        catch(...)
        {
            throw;
        }
    }
    catch(...)
    {
        throw;
    }
}

void Game::Initialise(HINSTANCE hInstance, HWND hWnd)
{
    if(!hWnd)
    {
        throw gcnew ArgumentNullException("hWnd");
    }
    else if(!hInstance)
    {
        throw gcnew ArgumentNullException("hInstance");
    }
    else
    {
        Game::gamehWnd = hWnd;
    }

    try
    {
        LogManager::WriteLine(LogType::Log, "Initialising Video\n");
        Video::Initialise();

        LogManager::WriteLine(LogType::Log, "Initialising Audio\n");
        Audio::Initialise(hWnd);

        LogManager::WriteLine(LogType::Log,
            "Setting display mode to {0}x{1}@{2}bpp\n", windowWidth, windowHeight, bitsPerPixel);
        Video::SetDisplayMode(hWnd, windowWidth, windowHeight, bitsPerPixel);
    }
    catch(...)
    {
        throw;
    }
}

void Game::Render()
{
    try
    {
        Video::Clear(Color::Black);
        Video::Flip();
    }
    catch(DirectDrawSurfaceLostException ^)
    {
        try
        {
            if(Game::IsRunning)
            {
                RestoreSurfaces();
            }
        }
        catch(...)
        {
            // failed to restore surfaces, which means its time to either
            // reload everything or pack up and go home.
            throw;
        }
    }
    catch(...)
    {
        throw;
    }
}

void Game::Shutdown()
{
    IsRunning = false;

    LogManager::WriteLine(LogType::Log, "Shutting down Video");
    Video::Shutdown();

    LogManager::WriteLine(LogType::Log, "Shutting down Audio");
    Audio::Shutdown();
}

void Game::Update()
{

}
