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
#include "xmlhelper.h"

String ^XmlHelper::GetAttributeValue(XElement ^element, String ^attribute)
{
    if(nullptr == element)
    {
        throw gcnew ArgumentNullException("element");
    }
    else if(nullptr == attribute)
    {
        throw gcnew ArgumentNullException("attribute");
    }

    if(String::Empty == attribute)
    {
        throw gcnew ArgumentException("attribute evaluates to String::Empty.");
    }

    XAttribute ^xAttribute = element->Attribute(attribute);
    if(nullptr == xAttribute)
    {
        throw gcnew XmlException(String::Format("the {0} attribute is missing.", attribute));
    }
    
    if(String::Empty == xAttribute->Value)
    {
        throw gcnew XmlException(String::Format("the value of the {0} attribute evaluates to String::Empty."));
    }
    else
    {
        return xAttribute->Value;
    }
}

int XmlHelper::GetAttributeValueAsInt32(XElement ^element, String ^attribute)
{
    int number;
    try
    {
        String ^initialValue = XmlHelper::GetAttributeValue(element, attribute);
        number = Convert::ToInt32(initialValue);

        return number;
    }
    catch(FormatException ^)
    {
        throw gcnew FormatException(String::Format("the {0} attribute contains a non-numeric value.", attribute));
    }
    catch(OverflowException ^)
    {
        throw gcnew OverflowException(String::Format("the {0} attribute contains a number that's greater than {1}", attribute, Int32::MaxValue));
    }
    catch(...)
    {
        throw;
    }
}

double XmlHelper::GetAttributeAsDouble(XElement ^element, String ^attribute)
{
    try
    {
        String ^initialValue = XmlHelper::GetAttributeValue(element, attribute);
        return Convert::ToDouble(initialValue);
    }
    catch(FormatException ^)
    {
        throw gcnew FormatException(String::Format("the {0} attribute contains a non-numeric value.", attribute));
    }
    catch(OverflowException ^)
    {
        throw gcnew OverflowException(String::Format("the attribute {0} contains a "
            "value that is less than {1} or greater than {2}.", attribute, Double::MinValue, Double::MaxValue));
    }
    catch(...)
    {
        throw;
    }
}