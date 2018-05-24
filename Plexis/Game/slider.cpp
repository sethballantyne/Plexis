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
#include "slider.h"

Slider::Slider(int x, int y, unsigned int length, int selectedIndex, int minimum, int maximum,
    int step, String ^optionsKey, MenuItemContainer ^parentContainer) : ContainerControl(x, y, selectedIndex, parentContainer)
{
    if(nullptr == optionsKey)
    {
        throw gcnew ArgumentNullException("optionsKey");
    }
    else if(String::Empty == optionsKey)
    {
        throw gcnew ArgumentException("optionsKey");
    }

    if(minimum >= maximum)
    {
        throw gcnew ArgumentException("sliders minimum value is greater than or equal to its maximum value.");
    }
    else if(step > maximum)
    {
        throw gcnew ArgumentException("the sliders step value is greater than its maximum value.");
    }
    else if(minimum > Int32::MaxValue)
    {
        throw gcnew ArgumentException("the sliders minimum value is greater than Int32::MaxValue.");
    }
    else if(minimum < Int32::MinValue)
    {
        throw gcnew ArgumentException("the sliders minimum value is less than Int32::MinValue.");
    }
    else if(maximum > Int32::MaxValue)
    {
        throw gcnew ArgumentException("the sliders maximum value is greater than Int32::MaxValue.");
    }
    else if(maximum < Int32::MinValue)
    {
        throw gcnew ArgumentException("the sliders maximum value is less than Int32::MinValue.");
    }
    else if(step > Int32::MaxValue)
    {
        throw gcnew ArgumentException("the sliders step value is greater than Int32::MaxValue.");
    }
    else if(step < Int32::MinValue)
    {
        throw gcnew ArgumentException("the sliders step value is less than Int32::MinValue.");
    }
  
    xmlMinimumValue = minimum;
    xmlMaximumValue = maximum;

    // it's possible for minimum to be something other than 0; it might be dealing with, say, minimum = 245 
    // and maximum = 250. sliderMaximumValue *really* represents the amount of times the trackbox can be 
    // slid to the left. UpdateTrackBox() will factor this in when calculating the percentage based on 
    // the trackbox's new position and place it in the approrpriate place.
    sliderMaximumValue = maximum - minimum;

    stepValue = step;
    trackLength = length;
    this->optionsKey = optionsKey;

    cursorPosition->Y = this->position.Y - (cursorSize->Height / 2);

    // If the option specified in optionsKey doesn't exist, we want currentValue to default to 0 when 
    // xmlMinimumValue is subtracted from optionsValue further down.
    // xmlMinimumValue is being specified instead of just putting 0 because obviously its possible
    // 0 - xmlMinimumValue != 0; if a default value is being assigned to currentValue, it must be 0.
    int optionValue = GameOptions::GetValue(optionsKey, xmlMinimumValue);
    if(optionValue < xmlMinimumValue || optionValue > xmlMaximumValue)
    {
        // optionsKey refers to a garbage value
        LogManager::WriteLine(LogType::Debug, "slider.cpp: options key {0} contains a value not within the bounds "
            "specified by the minimum and maximum XML attributes. Setting the optionKey value to the value specified by "
            "the minimum XML attribute.", optionsKey);

        // fix the problem so it doesn't happen again (hopefully).
        optionValue = xmlMinimumValue;
        GameOptions::SetValue(optionsKey, xmlMinimumValue);
        GameOptions::Save();
    }

    currentValue = optionValue - xmlMinimumValue;
    

    trackLines[0] = gcnew Line(System::Drawing::Color::White, 
        gcnew System::Drawing::Point(x, y), gcnew System::Drawing::Point(x + length, y));
    trackLines[1] = gcnew Line(System::Drawing::Color::FromArgb(0, 127, 127, 127), 
        gcnew System::Drawing::Point(x, y + 1), gcnew System::Drawing::Point(x + length, y + 1));

    // creating the lines that make up the trackbox
    for(int i = 0; i < trackBoxLines->Length; i++)
    {
        // the Y values are unmodified outside of the constructor, the X value is modified in
        // UpdateTrackBox() (i.e, whenever the control is manipulated. Makes sense.)
        System::Drawing::Point ^fromPoint = gcnew System::Drawing::Point(x, y - (scrollBoxHeight / 2));
        System::Drawing::Point ^toPoint = gcnew System::Drawing::Point(x, y + (scrollBoxHeight / 2));

        trackBoxLines[i] = gcnew Line(System::Drawing::Color::White, fromPoint, toPoint);
    }

    // trackbox shadow
    trackBoxLines[trackBoxLines->Length - 1]->Colour = System::Drawing::Color::FromArgb(0, 83, 83, 83);

    // position the trackbox.
    UpdateTrackBox();
}

void Slider::Render()
{
    try
    {
        

        Video::DrawLines(trackLines);
        Video::DrawLines(trackBoxLines);
        
    }
    catch(...)
    {
        throw;
    }
}

void Slider::UpdateTrackBox()
{
    // TODO: Get this crap out of the render method!
    double progress = currentValue / sliderMaximumValue;

    int pixelOffset = (int)Math::Truncate(progress * trackLength) - (trackBoxLines->Length / 2);
    
    for(int i = 0; i < trackBoxLines->Length; i++)
    {
        // updating the position of the track box before rendering.
        trackBoxLines[i]->From->X = Position.X + pixelOffset;
        trackBoxLines[i]->To->X = trackBoxLines[i]->From->X;

        pixelOffset++;
    }

    // the track box shadow.
    trackLines[trackLines->Length - 1]->From->X = Position.X + (trackBoxLines->Length / 2);
    trackLines[trackLines->Length - 1]->To->X = trackLines[trackLines->Length - 1]->From->X;
}