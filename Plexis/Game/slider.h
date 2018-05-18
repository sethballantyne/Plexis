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
    // the amout that's added or subtracted from the currentValu, based on whether
    // the scroll box is being moved to the left (decrease) or right (increase).
    double stepValue;

    // the sliders value; the scroll box's position on the track is relative to this value.
    double currentValue;

    // a slider is essentially a progress bar, it operates around a percentage. 
    // I wanted it to be flexible and support ranges that don't start from 0, so
    // it really has two ranges it works with; the external ranges supplied via
    // xml and passed to the constructor and then there's its internal ranges which
    // always start from 0; its these internal ranges that are used to calculate
    // where the track bar should be positioned each time it slides to the left or right
    // or whether it should move at all. The internal ranges (slider*Value) are simply conversions
    // of the xml*Value ranges.
    double xmlMinimumValue;
    double xmlMaximumValue;
    const double sliderMinimumValue = 0;
    double sliderMaximumValue;

    // height of the scroll box in pixels
    int scrollBoxHeight = 12;

    // length of the track in pixels.
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
        if(currentValue > sliderMinimumValue)
        {
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
    /// <summary>
    /// 
    /// </summary>
    /// <param name="x">the controls screen coordinate on the x axis.</param>
    /// <param name="y">the controls screen coordinate on the y axis.</param>
    /// <param name="length">the length of the controls track, in pixels.</param>
    /// <param name="selectedIndex">the controls position in the control list.</param>
    /// <param name="minimum">the lower bound value.</param>
    /// <param name="maximum">the upper bound value.</param>
    /// <param name="step">the amount the sliders value increases by when moving to the right, or decreases when moving to the left.</param>
    /// <param name="optionsKey">the option in GameOptions that should be revised each time the sliders value changes.</param>
    /// <param name="parentContainer">the menu item container the control belongs to.</param>
    /// <exception cref="System::ArgumentException">
    /// the sliders minimum value is greater than or equal to its maximum value, or
    /// the sliders step value is greater than its maximum value, or
    /// the sliders minimum value is greater than Int32::MaxValue, or
    /// the sliders minimum value is less than Int32::MinValue, or
    /// the sliders maximum value is greater than Int32::MaxValue, or
    /// the sliders maximum value is less than Int32::MinValue, or
    /// the sliders step value is greater than Int32::MaxValue, or
    /// the sliders step value is less than Int32::MinValue.
    /// </exception>
    /// <exception cref="System::ArgumentNullException"><i>optionsKey</i> is <b>null</b>.</exception>
    Slider(int x, int y, unsigned int length, int selectedIndex, double minimum, double maximum,
        double step, String ^optionsKey, MenuItemContainer ^parentContainer);
 
    /// <summary>
    /// Not used by Slider.
    /// </summary>
    /// <param name="sceneArgs"></param>
    void ReceiveSceneArgs(array<Object ^, 1> ^sceneArgs) override
    {
        /*static bool firstRun = true;
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

        }*/
    }

    /// <summary>
    /// Renders the control to the backbuffer.
    /// </summary>
    void Render() override;

    /// <summary>
    /// Handles user input related to the state of the control.
    /// </summary>
    /// <param name="keyboardState">current state of the keyboard.</param>
    /// <param name="mouseState">current state of the mouse.</param>
    /// <exception cref="System::ArgumentNullException"><i>keyboardState</i> is <b>null</b>.</exception>
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

    /// <summary>
    /// Gets the current value of the slider.
    /// </summary>
    property virtual double Value
    {
        double get()
        {
            return this->currentValue;
        }
    }
};