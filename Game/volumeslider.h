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
/// Slider control for manipulating audio volume.
/// </summary>
public ref class VolumeSlider :  Slider
{
    int lastValue;
public:
    /// <summary>
    /// Initialises an instance of VolumeSlider.
    /// </summary>
    /// <param name="x">the controls screen coordinate on the x axis.</param>
    /// <param name="y">the controls screen coordinate on the y axis.</param>
    /// <param name="length">the length of the controls track, in pixels.</param>
    /// <param name="selectedIndex">the controls position in the control list.></param>
    /// <param name="minimum">the lower bound value.</param>
    /// <param name="maximum">the upper bound value.</param>
    /// <param name="step">the amount the sliders value increases by when moving to the right, or decreases when moving to the left.</param>
    /// <param name="optionsKey">the option in GameOptions that should be revised each time the sliders value changes.</param>
    /// <param name="parentContainer">the menu item container the control belongs to.</param>
    /// <exception cref="System::ArgumentNullException"><i>optionsKey</i> is <b>null</b>.</exception>
    VolumeSlider(int x, int y, unsigned int length, int selectedIndex, int minimum, int maximum,
        int step, String ^optionsKey, MenuItemContainer ^parentContainer)
        : Slider(x, y, length, selectedIndex, minimum, maximum, step, optionsKey, parentContainer)
    {

    }

    /// <summary>
    /// Updates the state of the control. If the track box is moved by the player,
    /// this plays a sound demonstrating the new volume setting.
    /// </summary>
    /// <param name="keyboardState">current state of the keyboard.</param>
    /// <param name="mouseState">current state of the mouse.</param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override
    { 
        try
        {
            lastValue = Slider::Value;
            Slider::Update(keyboardState, mouseState);
            
            if(lastValue != Slider::Value)
            {
                // track box has been moved by the player.
                // play a sound so they can hear the new volume setting.
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