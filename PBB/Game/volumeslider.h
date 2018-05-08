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
#include "slider.h"

/// <summary>
/// 
/// </summary>
public ref class VolumeSlider :  Slider
{
    int lastValue;
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="length"></param>
    /// <param name="selectedIndex"></param>
    /// <param name="minimum"></param>
    /// <param name="maximum"></param>
    /// <param name="step"></param>
    /// <param name="optionsKey"></param>
    /// <param name="parentContainer"></param>
    VolumeSlider(int x, int y, unsigned int length, int selectedIndex, double minimum, double maximum,
        double step, String ^optionsKey, MenuItemContainer ^parentContainer)
        : Slider(x, y, length, selectedIndex, minimum, maximum, step, optionsKey, parentContainer)
    {

    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="keyboardState"></param>
    /// <param name="mouseState"></param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override
    { 
        try
        {
            lastValue  = Slider::Value;
            Slider::Update(keyboardState, mouseState);
            
            if(lastValue != Slider::Value)
            {
                //LogManager::WriteLine(LogType::Debug, "lastValue: {0} Slider::Value: {1}", lastValue, Slider::Value);
                ResourceManager::UpdateVolume(Slider::Value);
                ResourceManager::GetSoundBuffer("volume_conf")->Play();
            }
        }
        catch(...)
        {
            throw;
        }
    }
};