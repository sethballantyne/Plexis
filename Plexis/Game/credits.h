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
#include "control.h"
#include "label.h"

/// <summary>
/// 
/// </summary>
public ref class Credits : public Control
{
    Label ^greenText;
    Label ^whiteText;

public:
    Credits(String ^nameFont, String ^descriptionFont) : Control(0, 0)
    {
        if(nullptr == nameFont)
        {
            throw gcnew ArgumentNullException("nameFont");
        }
        else if(nullptr == descriptionFont)
        {
            throw gcnew ArgumentNullException("descriptionFont");
        }
        else if(String::Empty == nameFont)
        {
            throw gcnew ArgumentException("nameFont evaluates to String::Empty.");
        }
        else if(String::Empty == descriptionFont)
        {
            throw gcnew ArgumentException("descriptionFont evaluates to String::Empty.");
        }

        try
        {
            greenText = gcnew Label(0, 0, descriptionFont, nullptr);
            whiteText = gcnew Label(0, 0, nameFont, nullptr);
        }
        catch(...)
        {
            throw;
        }
    }

    void Render() override
    {
        greenText->Position->X = 53;
        greenText->Position->Y = 242;
        greenText->Text = "DESIGN / PROGRAMMING / SHIT SFX / CRAPPIER ART / QA";
        greenText->Render();

        whiteText->Position->X = 377;
        whiteText->Position->Y = 276;
        whiteText->Text = "SETH BALLANTYNE";
        whiteText->Render();

        greenText->Position->X = 17;
        greenText->Position->Y = 450;
        greenText->Text = "PLEXIS IS FREE SOFTWARE RELEASED UNDER THE MIT LICENSE";
        greenText->Render();

        greenText->Position->X = 260;
        greenText->Position->Y = 484;
        greenText->Text = "SEE LICENSE.TXT FOR DETAILS";
        greenText->Render();
    }
};