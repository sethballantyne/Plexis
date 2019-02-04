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
#include "input.h"
#include "font.h"
#include "game.h"
#include "gameoptions.h"
#include "logmanager.h"
#include "video.h"
#include "resourcemanager.h"
#include "highscores.h"
#include "scenemanager.h"
#include "entitymanager.h"
#include "levelmanager.h"

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

            ResourceManager::ReloadSurfaces();
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
		ShowCursor(false);

        LogManager::WriteLine(LogType::Log, "Initialising video");
        Video::Initialise();

        LogManager::WriteLine(LogType::Log, "Initialising audio");
        Audio::Initialise(hWnd);

        LogManager::WriteLine(LogType::Log, "Ininitialising input");
        Input::Initialise(hInstance, hWnd);

        LogManager::WriteLine(LogType::Log, "Loading the games settings.");
        GameOptions::Initialise("options.xml");

        LogManager::WriteLine(LogType::Log, "Ininitialising the resource manager.");
        ResourceManager::Initialise("paths.xml");

        LogManager::WriteLine(LogType::Log,
            "Setting display mode to {0}x{1}@{2}bpp", windowWidth, windowHeight, bitsPerPixel);
        Video::SetDisplayMode(hWnd, windowWidth, windowHeight, bitsPerPixel);

        LogManager::WriteLine(LogType::Log, "Loading resources");
        ResourceManager::LoadResources();
        ResourceManager::UpdateVolume(GameOptions::GetValue("soundVolume", 50));

		LogManager::WriteLine(LogType::Log, "loading entities.");
		EntityManager::Initialise(ResourceManager::GetXML("entities"));

		// highscores have to be loaded before the scenes, otherwise the
		// highscore table will be populated with default values.
		LogManager::WriteLine(LogType::Log, "loading high scores");
		HighScores::Initialise("highscores.dat");

		LogManager::WriteLine(LogType::Log, "loading scenes");
		SceneManager::Initialise(ResourceManager::GetXML("scenes"));

		LogManager::WriteLine(LogType::Log, "loading levels list.");
		LevelManager::Initialise(ResourceManager::GetXML("levels"));
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
        Video::Clear(System::Drawing::Color::Black);
        //Video::Blit(0, 0, ResourceManager::GetSurface("green_font"));
        SceneManager::CurrentScene->Render();
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

    LogManager::WriteLine(LogType::Log, "closing the high scores file");
    HighScores::Shutdown();

    LogManager::WriteLine(LogType::Log, "Shutting down the resource manager");
    ResourceManager::Shutdown();

    LogManager::WriteLine(LogType::Log, "Shutting down video");
    Video::Shutdown();

    LogManager::WriteLine(LogType::Log, "Shutting down audio");
    Audio::Shutdown();

    LogManager::WriteLine(LogType::Log, "Shutting down input");
    Input::Shutdown();
}

void Game::Update()
{
    DWORD currentTime = timeGetTime();
    DWORD timeDifference = currentTime - lastTime;

    if(timeDifference != 0)
    {
        Keys ^keyboardState = Input::ReadKeyboard();
        Mouse ^mouseState = Input::ReadMouse();

        /*if(GameOptions::GetValue("mouseMovesPaddle", false))
        {
            mouseState = Input::ReadMouse();
        }*/

        SceneManager::CurrentScene->Update(keyboardState, mouseState);

        Game::Render();
    }

    lastTime = currentTime;
}
