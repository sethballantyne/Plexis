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
#include "font.h"
#include "xmlhelper.h"

using namespace System::Xml;
using namespace System::Xml::Linq;
using namespace System::Collections::Generic;

/// <summary>
/// Produces Font objects based on the settings in fonts.xml; these are consumed by ResourceManager.
/// </summary>
public ref class FontFactory abstract sealed
{
public:
    /// <summary>
    /// Creates a Font object from each <font> element read in fonts.xml
    /// </summary>
    /// <param name="fontsXmlFile">the xml file containing font information.</param>
    /// <returns>the parsed the fonts as Font instances.</returns>
    /// <exception cref="System::ArgumentException">the value of the <i>glyphWidth</i> attribute is greater 
    /// than the width of the bitmap, or the value of the <i>glyphHeight</i> attribute is greater than the
    /// height of the bitmap.</exception>
    /// <exception cref="System::FormatException">the <i>glyphWidth</i> or <i>glyphHeight</i> attributes contain values that aren't numbers.</exception>
    /// <exception cref="System::OverflowException">the <i>glyphWidth</i> or <i>glyphHeight</i> attributes contain a value greater than Int32::MaxValue.</exception>
    /// <exception cref="System::Xml::XmlException">a font element is missing a required attribute or an attribute evaluates to String::Empty.</exception>
    static array<Font ^, 1> ^CreateFonts(XElement ^fontsXmlFile)
    {
        System::Collections::Generic::IEnumerable<XElement ^> ^fontElements = fontsXmlFile->Elements("font");

        List<Font ^> ^parsedFonts = gcnew List<Font ^>();
        try
        {
            for each(XElement ^fontElement in fontElements)
            {
                String ^name = XmlHelper::GetAttributeValue(fontElement, "name");
                String ^bitmap = XmlHelper::GetAttributeValue(fontElement, "bitmap");
                int glyphWidth = XmlHelper::GetAttributeValueAsInt32(fontElement, "glyphWidth");
                int glyphHeight = XmlHelper::GetAttributeValueAsInt32(fontElement, "glyphHeight");

                Surface ^fontSurface = ResourceManager::GetSurface("bitmap");
                parsedFonts->Add(gcnew Font(name, fontSurface, glyphWidth, glyphHeight));
            }
        }
        catch(...)
        {
            throw;
        }

        return parsedFonts->ToArray();
    }
};