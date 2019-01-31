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

using namespace System::Text;
using namespace System::Diagnostics;

/// <summary>
/// Represents a numerica value and associated caption to the left of the value.
/// </summary>
public ref class NumericField
{
private:
	// the label that displays the value's caption
	Label ^captionText;

	// label that displays the value on the screen
	Label ^numberText;

	Surface ^image;

	// the actual score.
	int numberAmount;

	// the max number of zeros that the value is padded with.
	// if numberOfZeros = 5, and numberAmount = 4, scoreAmountText will look like this: 00004.
	// if the same values were used but numberAmount = 456, it would look like this: 00456.
	unsigned int numberOfZeros;

	int imageX;

	int imageY;
	//
	bool renderTextCaption = true;

	// used to pad the numeric value with zeros and convert to a string.
	StringBuilder ^textFormatter = gcnew StringBuilder();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	String ^Pad(int value)
	{
		textFormatter->Clear();
		textFormatter->Append(numberAmount.ToString());

		// calculate the number of zeros needed to pad the numeric value with.
		// the max is 5.
		int numZeros = this->numberOfZeros - textFormatter->Length;

		// Pad the numeric value with the calculated amount of zeros.
		textFormatter->Insert(0, "0", numZeros);

		return textFormatter->ToString();
	}

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="captionText"></param>
	/// <param name="scoreInitialValue"></param>
	/// <param name="paddingAmount"></param>
	NumericField(unsigned int x, unsigned int y, String ^captionText, int ^numberTextInitialValue, unsigned int paddingAmount)
	{
		Debug::Assert(nullptr != captionText);
		Debug::Assert(nullptr != numberTextInitialValue);

		String ^convertedNumText = Convert::ToString(numberTextInitialValue);
		this->captionText = gcnew Label(x, y, "white", captionText);

		this->numberAmount = Convert::ToUInt32(numberTextInitialValue);
		this->numberOfZeros = paddingAmount;

		unsigned int numberPositionX = x + (this->captionText->LabelFont->GlyphWidth + 1) * this->captionText->Text->Length;
		this->numberText = gcnew Label(numberPositionX, y, "white", Pad(paddingAmount));

		
	}

	NumericField(unsigned int x, unsigned int y, Surface ^image, int numberTextInitialValue, unsigned int paddingAmount)
	{
		String ^convertedNumText = Convert::ToString(numberTextInitialValue);
		this->image = image;
		this->imageX = x;
		this->imageY = y;

		this->numberAmount = Convert::ToUInt32(numberTextInitialValue);
		this->numberOfZeros = paddingAmount;
		this->renderTextCaption = false;

		// width of the image + a space the width of the font used for the numbers.
		unsigned int numberPositionX = x + image->Size->Width + (ResourceManager::GetFont("white")->GlyphWidth / 2);
		this->numberText = gcnew Label(numberPositionX, y, "white", Pad(paddingAmount));

		
	}

	void Render()
	{	
		(true == renderTextCaption) ? captionText->Render() : Video::Blit(imageX, imageY, image);
		numberText->Render();
	}

	

	property int Value
	{
		int get()
		{
			return this->numberAmount;
		}

		void set(int value)
		{
			this->numberAmount = value;

			// update the numeric value on the screen.
			numberText->Text = Pad(this->numberAmount);
		}
	}
};