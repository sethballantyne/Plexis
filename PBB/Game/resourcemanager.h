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
#include "fontfactory.h"
#include "soundbuffer.h"
#include "resourcepath.h"
#include "surface.h"

using namespace System;
using namespace System::Xml;
using namespace System::Xml::Linq;
using namespace System::Collections;
using namespace System::Collections::Generic;

/// <summary>
/// The resource manager creates, manages and destroys all the games assets. 
/// All objects created by the resource manager must be destroyed by the resource manager.
/// When you request a resource via the 3 Get* methods, you're not getting a copy of a resource but 
/// rather the address of the resource stored in the pool, thus multiple requests for the same resource
/// by multiple objects get the same address to the same resource.DO NOT FREE THE RESOURCE UNTIL
/// IT'S TIME TO TERMINATE. Doing so makes the resource unavailable to any other object that needs it. 
/// </summary>
public ref class ResourceManager abstract sealed
{
private:
    // pools for each resource type.
    static Dictionary<String ^, Font ^> ^fonts;
    static Dictionary<String ^, SoundBuffer ^> ^sounds;
    static Dictionary<String ^, Surface ^> ^surfaces;
    static Dictionary<String ^, XElement ^> ^xmlFiles;

    // each list contains absolute paths based on the paths read from paths.xml
    // these are used both when loading the resources when the game is launched
    // and also when the DirectDraw surfaces or DirectSound soundbuffer's need to be restored.
    static List<ResourcePath ^> ^bitmapPaths;
    static List<ResourcePath ^> ^xmlPaths;
    static List<ResourcePath ^> ^audioPaths;

    // the absolute path to the directory the program is executing in.
    static String ^workingDirectory;

    /// <summary>
    /// Reads all the paths associated with the specified element.
    /// </summary>
    /// <param name="resourceElement"></param>
    /// <param name="pathList"></param>
    static void ParsePaths(XElement ^resourceElement, List<ResourcePath ^> ^%pathList);

    static bool PathExists(List<ResourcePath ^> ^pathList, String ^path);
public:
    static ResourceManager()
    {
        bitmapPaths = gcnew List<ResourcePath ^>();
        xmlPaths = gcnew List<ResourcePath ^>();
        audioPaths = gcnew List<ResourcePath ^>();

        fonts = gcnew Dictionary<String ^, Font ^>();
        sounds = gcnew Dictionary<String ^, SoundBuffer ^>();
        surfaces = gcnew Dictionary<String ^, Surface ^>();
        xmlFiles = gcnew Dictionary<String ^, XElement ^>();
    }

    /// <summary>
    /// Initialises the resource manager with the paths in the specified XML file.
    /// </summary>
    /// <param name="paths"></param>
    static void Initialise(String ^paths);

    /// <summary>
    /// Retrieves the specified font from the font resource pool.
    /// </summary>
    /// <param name="fontName"></param>
    /// <returns></returns>
    static Font ^GetFont(String ^fontName)
    {
        try
        {
            return fonts[fontName];
        }
        catch(KeyNotFoundException ^)
        {
            throw gcnew ResourceNotFoundException(
                String::Format("the resource manager doesn't have a font with the name {0}.", fontName));
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Retrieves the specified sound from the sound resource pool.
    /// </summary>
    /// <param name="soundName"></param>
    /// <returns></returns>
    static SoundBuffer ^GetSoundBuffer(String ^soundName)
    {
        try
        {
            return sounds[soundName];
        }
        catch(KeyNotFoundException ^)
        {
            throw gcnew ResourceNotFoundException(
                String::Format("the resource manager doesn't have a sound buffer object with the name {0}.", soundName));
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Retrieves the specified surface from the surface resource pool.
    /// </summary>
    /// <param name="surfaceName"></param>
    /// <returns></returns>
    static Surface ^GetSurface(String ^surfaceName)
    {
        try
        {
            return surfaces[surfaceName];
        }
        catch(KeyNotFoundException ^)
        {
            throw gcnew ResourceNotFoundException(
                String::Format("the resource manager doesn't have a surface object with the name {0}.", surfaceName));
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Retrieves the specified file from the xml resource pool.
    /// </summary>
    /// <param name="documentName"></param>
    /// <returns></returns>
    static XElement ^GetXML(String ^documentName)
    {
        try
        {
            return xmlFiles[documentName];
        }
        catch(KeyNotFoundException ^)
        {
            throw gcnew ResourceNotFoundException(
                String::Format("the resource manager doesn't have an XML document with the name {0}.", documentName));
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Creates the fonts based on the settings within the fonts file.
    /// </summary>
    /// <param name="file"></param>
    static void LoadFonts(String ^file);

    /// <summary>
    /// 
    /// </summary>
    static void LoadSoundBuffers();
};
