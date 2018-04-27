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

Slider::Slider(int x, int y, unsigned int length, int selectedIndex, double minimum, double maximum,
    double step, String ^optionsKey, MenuItemContainer ^parentContainer) : ContainerControl(x, y, selectedIndex, parentContainer)
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
    sliderMaximumValue = maximum - minimum;
    stepValue = step;
    trackLength = length;
    this->optionsKey = optionsKey;

    cursorPosition->Y = this->position.Y - (cursorSize->Height / 2);

    // sliderMinimumValue isn't used for the default because xmlMinimumValue still
    // needs to be subtracted from the value returned by GetValue. If, for example,
    // xmlMinimumValue is 1, and sliderMinimumValue is returned by GetValue, 
    // currentValue will be < 0, which isn't what we want. Setting the default to xmlMinimumValue
    // is a round about way of getting 0, which we do want if defaults need to be used. 
    currentValue = GameOptions::GetValue(optionsKey, xmlMinimumValue) - xmlMinimumValue;

    trackLines[0] = gcnew Line(System::Drawing::Color::White, 
        gcnew System::Drawing::Point(x, y), gcnew System::Drawing::Point(x + length, y));
    trackLines[1] = gcnew Line(System::Drawing::Color::FromArgb(0, 127, 127, 127), 
        gcnew System::Drawing::Point(x, y + 1), gcnew System::Drawing::Point(x + length, y + 1));

    for(int i = 0; i < trackBoxLines->Length; i++)
    {
        // x is a place holder, it'll be updated to the correct value when the slider is rendered.
        System::Drawing::Point ^fromPoint = gcnew System::Drawing::Point(x, y - (scrollBoxHeight / 2));
        System::Drawing::Point ^toPoint = gcnew System::Drawing::Point(x, y + (scrollBoxHeight / 2));

        trackBoxLines[i] = gcnew Line(System::Drawing::Color::White, fromPoint, toPoint);
    }

    trackBoxLines[trackBoxLines->Length - 1]->Colour = System::Drawing::Color::FromArgb(0, 83, 83, 83);
}

void Slider::Render()
{
    try
    {
        double progress = currentValue / sliderMaximumValue;

        int pixelOffset = (int)Math::Truncate(progress * trackLength) - (trackBoxLines->Length / 2);
      
        for(int i = 0; i < trackBoxLines->Length; i++)
        {
            // updating the position of the track box before rendering.
            trackBoxLines[i]->From->X = Position.X + pixelOffset;
            trackBoxLines[i]->To->X = trackBoxLines[i]->From->X;

            //LogManager::WriteLine(LogType::Debug, "{0}", pixelOffset);
            //LogManager::WriteLine(LogType::Debug, "trackBoxLines[{0}]->To.X = {1}", i, trackBoxLines[i]->To->X);*/
            pixelOffset++;
        }

        // the track box shadow.
        trackLines[trackLines->Length - 1]->From->X = Position.X + (trackBoxLines->Length / 2);
        trackLines[trackLines->Length - 1]->To->X = trackLines[trackLines->Length - 1]->From->X;

        Video::DrawLines(trackLines);
        Video::DrawLines(trackBoxLines);
        
    }
    catch(...)
    {
        throw;
    }
}