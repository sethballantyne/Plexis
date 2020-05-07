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
#include <msclr\auto_handle.h>
#include "gameoptions.h"
#include "xmlhelper.h"

using namespace System::Xml::Linq;

void GameOptions::Initialise(String ^path)
{
    if(nullptr == path)
    {
        throw gcnew ArgumentNullException("path");
    }
    else if(String::Empty == path)
    {
        throw gcnew ArgumentException("path evaluates to String::Empty.");
    }

    try
    {
        XElement ^xmlFile = XElement::Load(path);

        IEnumerable<XElement ^> ^xmlOptions = xmlFile->Descendants("option");

        for each(XElement ^element in xmlOptions)
        {
            String ^id = XmlHelper::GetAttributeValue(element, "id");
            String ^value = XmlHelper::GetAttributeValue(element, "value");

            GameOptions::gameOptions[id] = value;
        }

        GameOptions::path = path;
    }
    catch(...)
    {
        throw;
    }
}

void GameOptions::Save()
{
    XmlWriterSettings ^xmlWriterSettings = gcnew XmlWriterSettings();

    xmlWriterSettings->Indent = true;
    xmlWriterSettings->CloseOutput = true;
    xmlWriterSettings->WriteEndDocumentOnClose = true;

    try
    {
        msclr::auto_handle<XmlWriter> xmlWriter(XmlWriter::Create(GameOptions::path, xmlWriterSettings));
        xmlWriter->WriteStartDocument();

        // <options>
        xmlWriter->WriteStartElement("options");

        for each(KeyValuePair<String ^, String ^> ^option in GameOptions::gameOptions)
        {
            // <option id="blah" value="blah" />
            xmlWriter->WriteStartElement("option");
            xmlWriter->WriteAttributeString("id", option->Key);
            xmlWriter->WriteAttributeString("value", option->Value);
            xmlWriter->WriteEndElement();
        }

        // </options>
        xmlWriter->WriteEndElement();
        xmlWriter->WriteEndDocument();

        xmlWriter->Close();
    }
    catch(...)
    {
        throw;
    }
}
