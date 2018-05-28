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
/// Quick and dirty means of grouping credit information for easy rendering.
/// </summary>
private ref class Data
{
public:
    int X;
    int Y;
    String ^Text;

    Data(int x, int y, String ^text)
    {
        X = x; Y = y; Text = text;
    }
};

/// <summary>
/// Displays the game credits.
/// </summary>
public ref class Credits : public Control
{
private:
    Label ^greenText;
    Label ^whiteText;
    List<Data ^> ^nameData;
    List<Data ^> ^descriptionData;

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

            descriptionData = gcnew List<Data ^>();
            nameData = gcnew List<Data ^>();

            descriptionData->Add(gcnew Data(53, 242, "DESIGN / PROGRAMMING / SHIT SFX / CRAPPIER ART / QA"));
            descriptionData->Add(gcnew Data(17, 450, "PLEXIS IS FREE SOFTWARE RELEASED UNDER THE MIT LICENSE"));
            descriptionData->Add(gcnew Data(260, 484, "SEE LICENSE.TXT FOR DETAILS"));

            nameData->Add(gcnew Data(377, 276, "SETH BALLANTYNE"));
        }
        catch(...)
        {
            throw;
        }
    }

    void Render() override
    {
        for(int i = 0; i < descriptionData->Count; i++)
        {
            greenText->Position->X = descriptionData[i]->X;
            greenText->Position->Y = descriptionData[i]->Y;
            greenText->Text = descriptionData[i]->Text;
            greenText->Render();
        }

        for(int i = 0; i < nameData->Count; i++)
        {
            whiteText->Position->X = nameData[i]->X;
            whiteText->Position->Y = nameData[i]->Y;
            whiteText->Text = nameData[i]->Text;
            whiteText->Render();
        }
    }
};