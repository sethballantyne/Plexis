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
#include "label.h"
#include "version.h"

/// <summary>
/// Label that displays a the current version of the game.
/// </summary>
public ref class VersionLabel : public Label
{
public:
    /// <summary>
    /// Initialises a new instance of VersionLabel with the specified position and font.
    /// </summary>
    /// <param name="x">the labels screen position on the x axis.</param>
    /// <param name="y">the labels screen position on the y axis.</param>
    /// <param name="font">the name of the font to use when rendering the labels text.</param>
    VersionLabel(int x, int y, String ^font) : Label(x, y, font, nullptr)
    {
		// VERSION_MAJOR and VERSION_REVISION are converted to string because
		// the compiler was bitching about both values being implicitly boxed 
		// (their values were zero at the time). 
		// They have to be converted to strings anyway but I still think it's dumb.
		this->Text = String::Format("{0}.{1}.{2}", Convert::ToString(VERSION_MAJOR), VERSION_MINOR, Convert::ToString(VERSION_REVISION));
    }
};