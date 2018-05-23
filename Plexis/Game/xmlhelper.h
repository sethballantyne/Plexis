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

using namespace System;
using namespace System::Xml;
using namespace System::Xml::Linq;

/// <summary>
/// Helper class that retrieves the values of the specified XML attribute.
/// </summary>
public ref class XmlHelper abstract sealed
{
public:
    /// <summary>
    /// Counts the number of items XElement items within an IEnumberable collection.
    /// </summary>
    /// <param name="collection">the collection to count</param>
    /// <returns></returns>
    static int XmlHelper::Count(System::Collections::IEnumerable ^collection)
    {
        int count = 0;
        for each(XElement ^element in collection)
        {
            count++;
        }

        return count;
    }

    /// <summary>
    /// Gets the value of the specified attribute. 
    /// </summary>
    /// <param name="element">the element the attribute belongs to.</param>
    /// <param name="attribute">the desired attribute to query.</param>
    /// <returns>the value of the attribute as a string.</returns>
    /// <exception cref="System::ArgumentException"><i>attribute</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>element</i> or <i>attribute</i> evaluates to <b>null</b>.</exception>
    /// <exception cref="System::Xml::XmlException">the specified attribute is missing from the element or its value evaluates to String::Empty.</exception>
    static String ^GetAttributeValue(XElement ^element, String ^attribute);

    /// <summary>
    /// Gets the value of the specified attribute. 
    /// </summary>
    /// <param name="element">the element the attribute belongs to.</param>
    /// <param name="attribute">the desired attribute to query.</param>
    /// <returns>the attributes value as an integer.</returns>
    /// <exception cref="System::ArgumentException"><i>attribute</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>element</i> or <i>attribute</i> evaluates to <b>null</b>.</exception>
    /// <exception cref="System::FormatException">the attributes value is non-numeric.</exception>
    /// <exception cref="System::OverflowException">the attributes value is greater than Int32::MaxValue.</exception>
    /// <exception cref="System::Xml::XmlException">the specified attribute is missing from the element or its value evaluates to String::Empty.</exception>
    static int GetAttributeValueAsInt32(XElement ^element, String ^attribute);

    /// <summary>
    /// Gets the value of the specified attribute as an unsigned integer.
    /// </summary>
    /// <param name="element">the element the attribute belongs to.</param>
    /// <param name="attribute">the desired attribute to query.</param>
    /// <returns><i>attribute<i>'s value as an unsigned integer.</returns>
    /// <exception cref="System::ArgumentException"><i>attribute</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>element</i> or <i>attribute</i> evaluates to <b>null</b>.</exception>
    /// <exception cref="System::FormatException">the attributes value is non-numeric.</exception>
    /// /// <exception cref="System::OverflowException">the attributes value is less than UInt32::MinValue or greater than UInt32::MaxValue.</exception>
    static unsigned int GetAttributeValueAsUInt32(XElement ^element, String ^attribute);

    /// <summary>
    /// Gets the value of the specified attribute. 
    /// </summary>
    /// <param name="element">the element the attribute belongs to.</param>
    /// <param name="attribute">the desired attribute to query.</param>
    /// <returns>the boolean value of the attribute.</returns>
    /// <exception cref="System::ArgumentException"><i>attribute</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>element</i> or <i>attribute</i> evaluates to <b>null</b>.</exception>
    /// <exception cref="System::Xml::XmlException">the specified attribute is missing from the element or its value evaluates to String::Empty.</exception>
    static bool GetAttributeValueAsBoolean(XElement ^element, String ^attribute)
    {
        try
        {
            String ^temp = XmlHelper::GetAttributeValue(element, attribute);
            return Convert::ToBoolean(temp);
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Gets the value of the specified attribute. 
    /// </summary>
    /// <param name="element">the element the attribute belongs to.</param>
    /// <param name="attribute">the desired attribute to query.</param>
    /// <returns>the double value of the attribute.</returns>
    /// <exception cref="System::ArgumentException"><i>attribute</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>element</i> or <i>attribute</i> evaluates to <b>null</b>.</exception>
    /// <exception cref="System::FormatException">the attributes value is non-numeric.</exception>
    /// <exception cref="System::OverflowException">the attributes value is greater than Double::MaxValue or less than Double::MinValue.</exception>
    /// <exception cref="System::Xml::XmlException">the specified attribute is missing from the element or its value evaluates to String::Empty.</exception>
    static double GetAttributeValueAsDouble(XElement ^element, String ^attribute);
};