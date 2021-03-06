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
#include "label.h"
#include "resourcemanager.h"

Label::Label(int x, int y, String ^font, String ^text) : Control(x, y)
{
    if(nullptr == font)
    {
        throw gcnew ArgumentNullException("font");
    }
    else if(String::Empty == font)
    {
        throw gcnew ArgumentException("font evaluates to String::Empty.");
    }

    // the text argument isn't checked because sometimes controls that make use of Label
    // don't have anything to pass to the control in terms of text during initialisation.
	// There was a log message here that would log when text evaluated to nullptr; it was designed
	// to be a friendly heads up to aid debugging, but it turned out to be a total pain in the
	// arse. It felt very satisfying getting rid of it, much like scratching a terrible itch
	// or taking a massive dump. 

    try
    {
        this->font = ResourceManager::GetFont(font);
    }
    catch(...)
    {
        throw;
    }

    this->text = text;
}