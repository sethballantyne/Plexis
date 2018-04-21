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
#include "video.h"

using namespace System::IO;



array<Font ^, 1> ^ResourceManager::CreateFonts(XElement ^fontsXmlFile)
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

            Surface ^fontSurface = ResourceManager::GetSurface(Path::GetFileNameWithoutExtension(bitmap));
            parsedFonts->Add(gcnew Font(name, fontSurface, glyphWidth, glyphHeight));
        }
    }
    catch(...)
    {
        throw;
    }

    return parsedFonts->ToArray();
}

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
                throw gcnew DirectoryNotFoundException(String::Format("the path {0} doesn't exist or is invalid.", path));
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

    try
    {
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
    catch(...)
    {
        throw;
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
        XElement ^fontFile = ResourceManager::GetXML(file);
        array<Font ^, 1> ^loadedFonts = CreateFonts(fontFile);

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
    try
    {
        for each(ResourcePath ^path in audioPaths)
        {
            array<String ^, 1> ^files;
            String ^pathToSearch = workingDirectory + path->Path;

            if(path->IncludeSubDirectories)
            {
                files = Directory::GetFiles(pathToSearch, "*.wav", SearchOption::AllDirectories);
            }
            else
            {
                files = Directory::GetFiles(pathToSearch, "*.wav", SearchOption::TopDirectoryOnly);
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
    catch(...)
    {
        throw;
    }
}

void ResourceManager::LoadSurfaces()
{
    try
    {
        for each(ResourcePath ^path in bitmapPaths)
        {
            array<String ^, 1> ^files;
            String ^pathToSearch = workingDirectory + path->Path;

            if(path->IncludeSubDirectories)
            {
                files = Directory::GetFiles(pathToSearch, "*.bmp", SearchOption::AllDirectories);
            }
            else
            {
                files = Directory::GetFiles(pathToSearch, "*.bmp", SearchOption::TopDirectoryOnly);
            }

            for each(String ^file in files)
            {
                Surface ^surface = Video::CreateSurface(file);
                surface->Path = file;
                surface->Name = Path::GetFileNameWithoutExtension(file);

                surfaces->Add(surface->Name, surface);
            }
        }
    }
    catch(...)
    {
        throw;
    }
}

void ResourceManager::LoadXML()
{
    try
    {
        for each(ResourcePath ^path in xmlPaths)
        {
            array<String ^, 1> ^files;

            String ^pathToSearch = workingDirectory + path->Path;
            if(path->IncludeSubDirectories)
            {
                files = Directory::GetFiles(pathToSearch, "*.xml", SearchOption::AllDirectories);
            }
            else
            {
                files = Directory::GetFiles(pathToSearch, "*.xml", SearchOption::TopDirectoryOnly);
            }

            for each(String ^file in files)
            {
                XElement ^xmlFile = XElement::Load(file);
                String ^resourceName = Path::GetFileNameWithoutExtension(file);

                xmlFiles->Add(resourceName, xmlFile);
            }
        }
    }
    catch(...)
    {
        throw;
    }
}

void ResourceManager::ReloadSurfaces()
{
    try
    {
        for each(KeyValuePair<String ^, Surface ^> ^keyValuePair in surfaces)
        {
            keyValuePair->Value->Release();

            // when calling Surface::Release(), it frees the underlying DirectDraw surface but the
            // actual Surface object remains. We don't need to recreate the Surface object, just update
            // the DirectDraw surface it encapsulates.
            surfaces[keyValuePair->Key]->Data = Video::CreateSurface(keyValuePair->Value->Path)->Data;
        }
    }
    catch(...)
    {
        throw;
    }

    //// now that each surface has been reloaded, the fonts have to be updated since they
    //// each contain a Surface object.
    //for each(KeyValuePair<String ^, Font ^> ^kvp in fonts)
    //{
    //    fonts[kvp->Key]->FontSurface->Data = ResourceManager::GetSurface(kvp->Value->FontSurface->Name)->Data;
    //}
}

void ResourceManager::ReloadSoundBuffers()
{
    try
    {
        for each(KeyValuePair<String ^, SoundBuffer ^> ^kvp in sounds)
        {
            kvp->Value->Release();

            sounds[kvp->Key]->Data = Audio::CreateSoundBuffer(kvp->Value->Path)->Data;
        }
    }
    catch(...)
    {
        throw;
    }
}