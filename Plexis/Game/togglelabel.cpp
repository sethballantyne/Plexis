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
#include "togglelabel.h"

ToggleLabel::ToggleLabel(int x, int y, String ^font, String ^trueCaption, String ^falseCaption,
    int selectedIndex, String ^optionsKey, MenuItemContainer ^parentContainer) : ContainerControl(x, y, selectedIndex, parentContainer)
{
    if(nullptr == font)
    {
        throw gcnew ArgumentNullException("font");
    }
    else if(nullptr == trueCaption)
    {
        throw gcnew ArgumentNullException("trueCaption");
    }
    else if(nullptr == falseCaption)
    {
        throw gcnew ArgumentNullException("falseCaption");
    }

    if(String::Empty == font)
    {
        throw gcnew ArgumentException("font evaluates to String::Empty.");
    }
    else if(String::Empty == trueCaption)
    {
        throw gcnew ArgumentException("trueCaption evaluates to String::Empty.");
    }
    else if(String::Empty == falseCaption)
    {
        throw gcnew ArgumentException("falseCaption evaluates to String::Empty.");
    }
    else if(String::Empty == optionsKey)
    {
        throw gcnew ArgumentException("optionsKey evaluates to String::Empty.");
    }

    captions = gcnew array<String ^, 1>(2);
    captions[0] = falseCaption;
    captions[1] = trueCaption;

    this->optionsKey = optionsKey;
    try
    {
        //// set the initial state of the control
        //toggleState = (bool)GameOptions::GetValue(optionsKey, false);
        //label = gcnew Label(x, y, font, captions[toggleState]);
        label = gcnew Label(x, y, font, captions[toggleState]);
    }
    catch(...)
    {
        throw;
    }
}