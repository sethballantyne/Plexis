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
#include "containercontrol.h"
#include "gameoptions.h"

/// <summary>
/// a basic version of the Track Bar found in Windows. 
/// </summary>
public ref class Slider : ContainerControl
{
private:
    // the amount the sliders value can't go below
    double xmlMinimumValue;

    // the amount the sliders value can't exceed.
    double xmlMaximumValue;

    // the amout that's added or subtracted from the currentValu, based on whether
    // the scroll box is being moved to the left (decrease) or right (increase).
    double stepValue;

    // the sliders value; the scroll box's position on the track is relative to this value.
    double currentValue;

    double sliderMinimumValue;
    double sliderMaximumValue;

    // height of the scroll box in pixels
    int scrollBoxHeight = 12;

    int trackLength;

    // the GameOptions key associated with the option that the slider is responsible for changing.
    String ^optionsKey;

    // the lines that make up the track, when rendered.
    array<Line ^, 1> ^trackLines = gcnew array<Line ^, 1>(2);

    // the lines that make up the track box, when rendered.
    array<Line ^, 1> ^trackBoxLines = gcnew array<Line ^, 1>(6);

    /// <summary>
    /// Decreases the currentValue by the step amount specified at construction and updates
    /// the value of the controls option key with the new value.
    /// This also causes the controls track box to slide to the left if currentValue
    /// was greater than the controls minimum value before Decrement() was called; 
    /// the change will be displayed the next time Slider::Render() is executed. 
    /// </summary>
    void Decrement()
    {
        LogManager::WriteLine(LogType::Debug, "verifying if we can decrement {0}", currentValue);
        if(currentValue > sliderMinimumValue)
        {
            LogManager::WriteLine(LogType::Debug, "Decrementing.");
            currentValue -= stepValue;
            if(currentValue < sliderMinimumValue)
            {
                currentValue = sliderMinimumValue;
            }
        }
        
        GameOptions::SetValue(optionsKey, currentValue + xmlMinimumValue);
        GameOptions::Save();
    }

    /// <summary>
    /// Increases the currentValue by the step amount specified at construction and updates 
    /// the value of the controls option key with the new value.
    /// This also causes the controls track box to slide to the right if currentValue
    /// was less than the controls maximum value before Increment() was called; 
    /// the change will be displayed the next time Slider::Render() is executed. 
    /// </summary>
    void Increment()
    {
        if(currentValue < sliderMaximumValue)
        {
            currentValue += stepValue;
            if(currentValue > sliderMaximumValue)
            {
                currentValue = sliderMaximumValue;
            }
        }

        GameOptions::SetValue(optionsKey, currentValue + xmlMinimumValue);
        GameOptions::Save();
    }

public:
    Slider(int x, int y, unsigned int length, int selectedIndex, double minimum, double maximum,
        double step, String ^optionsKey, MenuItemContainer ^parentContainer);
 
    /// <summary>
    /// Called when the controls parent Scene has been activated by the scene manager.
    /// The slider control doesn't have any arguments to receive but since it's called only
    /// when the scene has been activated, it's a good place to update the control during 
    /// runtime so it's displaying the correct information when it's displayed. 
    /// </summary>
    /// <param name="sceneArgs"></param>
    void ReceiveSceneArgs(array<Object ^, 1> ^sceneArgs) override
    {
        static bool firstRun = true;
        try
        {
            if(firstRun)
            {
                currentValue = GameOptions::GetValue(optionsKey, sliderMinimumValue);
                firstRun = false;
            }
        }
        catch(...)
        {

        }
    }

    /// <summary>
    /// 
    /// </summary>
    void Render() override;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="keyboardState"></param>
    /// <param name="mouseState"></param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override
    {
        if(nullptr == keyboardState)
        {
            throw gcnew ArgumentNullException("keyboardState");
        }

        if(keyboardState->KeyPressed(DIK_LEFT))
        {
            Decrement();
        }
        else if(keyboardState->KeyPressed(DIK_RIGHT))
        {
            Increment();
        }
        else if(keyboardState->KeyPressed(DIK_UP))
        {
            ParentContainer->SelectPreviousControl();
        }
        else if(keyboardState->KeyPressed(DIK_DOWN))
        {
            ParentContainer->SelectNextControl();
        }
    }
};