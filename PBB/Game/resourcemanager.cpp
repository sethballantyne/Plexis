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
#include "audio.h"
#include "resourcemanager.h"
#include "xmlhelper.h"
#include "logmanager.h"

using namespace System::IO;

bool ResourceManager::PathExists(List<ResourcePath ^> ^pathList, String ^path)
{
    for(int i = 0; i < pathList->Count; i++)
    {
        if(pathList[i]->Path == path)
            return true;
    }

    return false;
}

void ResourceManager::ParsePaths(XElement ^resourceElement, List<ResourcePath ^> ^%pathList)
{
    if(nullptr == resourceElement)
    {
        throw gcnew ArgumentNullException("resourceElement");
    }
    else if(nullptr == pathList)
    {
        throw gcnew ArgumentNullException("pathList");
    }

    System::Collections::Generic::IEnumerable<XElement ^> ^pathsQuery = resourceElement->Descendants((String ^)"path");
    

    for each(XElement ^pathElement in pathsQuery)
    {
        if(pathElement != nullptr)
        {
            bool includeSubDirs = XmlHelper::GetAttributeValueAsBoolean(pathElement, "includeSubDirectories");
            String ^path = pathElement->Value;

            // empty value means the current directory.
            if(String::IsNullOrWhiteSpace(path))
            {
                path = "//";
            }

            if(!Directory::Exists(workingDirectory + path))
            {
                throw gcnew DirectoryNotFoundException(String::Format("the path {0} doesn't exist.", path));
            }

            if(!PathExists(pathList, path))
            {
                pathList->Add(gcnew ResourcePath(path, includeSubDirs));
            }
        }
    }
}

void ResourceManager::Initialise(String ^pathsFile)
{
    if(nullptr == pathsFile)
    {
        throw gcnew ArgumentNullException("pathsFile");
    }
    else if(String::Empty == pathsFile)
    {
        throw gcnew ArgumentException("pathsFile evaluates to String::Empty.");
    }

    workingDirectory = Directory::GetCurrentDirectory();
    XElement ^xmlPathsFile = XElement::Load(pathsFile);

    System::Collections::Generic::IEnumerable<XElement ^> ^resourcesQuery = xmlPathsFile->Descendants("resource");
    if(XmlHelper::Count(resourcesQuery) == 0)
    {
        LogManager::WriteLine(LogType::Error, "{0} doesn't contain any <resource> elements!", pathsFile);
    }

    for each(XElement ^resourceElement in resourcesQuery)
    {
        String ^type = XmlHelper::GetAttributeValue(resourceElement, "type");
        if("bitmaps" == type)
        {
            ParsePaths(resourceElement, bitmapPaths);
        }
        else if("audio" == type)
        {
            ParsePaths(resourceElement, audioPaths);
        }
        else if("xml" == type)
        {
            ParsePaths(resourceElement, xmlPaths);
        }
        else
        {
            throw gcnew XmlException(String::Format("invalid attribute value {0} within a <resource> element.", type));
        }
    }
}

void ResourceManager::LoadFonts(String ^file)
{
    if(nullptr == file)
    {
        throw gcnew ArgumentNullException("file");
    }
    else if(String::Empty == file)
    {
        throw gcnew ArgumentException("file evaluates to String::Empty.");
    }

    try
    {
        XElement ^fontFile = ResourceManager::GetXML("fonts");
        array<Font ^, 1> ^loadedFonts = FontFactory::CreateFonts(fontFile);

        for(int i = 0; i < loadedFonts->Length; i++)
        {
            fonts->Add(loadedFonts[i]->Name, loadedFonts[i]);
        }
    }
    catch(...)
    {
        throw;
    }
}

void ResourceManager::LoadSoundBuffers()
{
    for each(ResourcePath ^path in audioPaths)
    {
        array<String ^, 1> ^files;

        if(path->IncludeSubDirectories)
        {
            files = Directory::GetFiles(path->Path, "*.wav", SearchOption::AllDirectories);
        }
        else
        {
            files = Directory::GetFiles(path->Path, "*.wav", SearchOption::TopDirectoryOnly);
        }

        for each(String ^file in files)
        {
            SoundBuffer ^soundBuffer = Audio::CreateSoundBuffer(file);
            soundBuffer->Path = file;
            String ^resourceName = Path::GetFileNameWithoutExtension(file);

            sounds->Add(resourceName, soundBuffer);
        }
    }
}