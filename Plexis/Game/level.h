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
#include "brick.h"
#include "entitymanager.h"

using namespace System;

/// <summary>
/// 
/// </summary>
public ref class Level
{
private:
    array<Brick ^, 2>^ level;

public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="width"></param>
    /// <param name="height"></param>
    Level(unsigned int width, unsigned int height)
    {
        level = gcnew array<Brick ^, 2>(width, height);
    }

    void PutBrickAt(unsigned int x, unsigned int y, String ^brickName)
    {
        level[y, x] = EntityManager::GetEntity<Brick ^>(brickName);

        int spriteWidth = level[y, x]->Sprite->CurrentFrame->Coordinates.Width;
        int spriteHeight = level[y, x]->Sprite->CurrentFrame->Coordinates.Height;

        level[y, x]->Position = System::Drawing::Point(x * spriteWidth, y * spriteHeight);
    }

    Brick ^GetBrickAt(unsigned int x, unsigned int y)
    {
        return level[y, x];
    }

    void Render()
    {
        for(int i = 0; i < level->GetLength(0); i++)
        {
            for(int j = 0; j < level->GetLength(1); j++)
            {
                if(nullptr != level[i, j])
                {
                    level[i, j]->Sprite->Render();
                }
            }
        }
    }
};
