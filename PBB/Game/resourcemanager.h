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
    /// <exception cref="System::ArgumentNullException"><i>resourceElement</i> or <i>pathList</i> are <b>null</b>.</exception>
    /// <exception cref="System::IO::DirectoryNotFoundException">a path specified in the xml file doesn't exist or is invalid: the path is too long or the application doesn't have the correct permissions to access the location.</exception>
    /// <exception cref="System::Xml::XmlException">an xml element is missing a required attribute.</exception>
    static void ParsePaths(XElement ^resourceElement, List<ResourcePath ^> ^%pathList);

    /// <summary>
    /// Searches a list of ResourcePath instances for the specified path.
    /// </summary>
    /// <param name="pathList">the list to search.</param>
    /// <param name="path">the search for in <i>pathList</i>.</param>
    /// <returns></b>true</b> if <i>pathList</i> contains a ResourcePath instance that points to the specified path,
    /// else returns <b>false.</b></returns>
    static bool PathExists(List<ResourcePath ^> ^pathList, String ^path);

    /// <summary>
    /// Creates a Font object from each <font> element read in fonts.xml
    /// </summary>
    /// <param name="fontsXmlFile">the xml file containing font information.</param>
    /// <returns>the parsed the fonts as Font instances.</returns>
    /// <exception cref="System::ArgumentException">the <i>glyphWidth</i> or <i>glyphHeight</i> attribute in a <i>path</i> 
    /// element contains a value that's greater than the width (for <i>glyphWidth</i>) or height (for <i>glyphHeight</i>)
    /// of the fonts bitmap.</exception>
    /// <exception cref="System::FormatException">the <i>glyphWidth</i> or <i>glyphHeight</i> attributes contain values that aren't numbers.</exception>
    /// <exception cref="System::OverflowException">the <i>glyphWidth</i> or <i>glyphHeight</i> attributes contain a value greater than Int32::MaxValue.</exception>
    /// <exception cref="System::Xml::XmlException">a font element is missing a required attribute or an attribute evaluates to String::Empty.</exception>
    static array<Font ^, 1> ^CreateFonts(XElement ^fontsXmlFile);

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
    /// <param name="pathsFile">the xml file that contains path information.</param>
    /// <exception cref="System::ArgumentException"><i>pathsFile</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>pathsFile</i> is <b>null</b>.</exception>
    /// <exception cref="System::Xml::XmlException">a required attribute is missing or contains an invalid value.</exception>
    static void Initialise(String ^pathsFile);

    /// <summary>
    /// Retrieves the specified font from the font resource pool.
    /// </summary>
    /// <param name="fontName">the name of the desired font; this will be the value specified in the 
    /// fonts <i>name</i> attribute in fonts.xml.</param>
    /// <returns>The desired bitmap font.</returns>
    /// <exception cref="ResourceNotFoundException">the resource manager received a request for a font that it doesn't have.</exception>
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
    /// <param name="soundName">the name of the desired sound effect; this will be the name of the originating *.wav file, minus the file extension.</param>
    /// <returns>the desired sound effect as a SoundBuffer instance.</returns>
    /// <exception cref="ResourceNotFoundException">the resource manager received a request for a sound effect that it doesn't have.</exception>
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
    /// <param name="surfaceName">the name of the surface to search for; this will be the filename of the originating bitmap,
    /// minus the .bmp extension.</param>
    /// <returns>the desired bitmap as a surface object.</returns>
    /// <exception cref="ResourceNotFoundException">the resource manager received a request for a surface that it doesn't have.</exception>
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
    /// <param name="documentName">the name of the xml document to search for.</param>
    /// <returns>the desired XML document as an XElement object.</returns>
    /// <exception cref="ResourceNotFoundException">the resource manager received a request for an xml document that it doesn't have.</exception>
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
    /// <remarks>this method can be called only after both the xml files and surfaces have been loaded
    /// as it depends on both having been completed successfully in order to execute correctly.</remarks>
    static void LoadFonts(String ^file);

    /// <summary>
    /// Helper function that loads each resource type in the appropriate order. 
    /// Calling this is the same as calling LoadXML(), LoadSurfaces(), 
    /// LoadSoundBuffers() and LoadFonts() in that order.
    /// </summary>
    static void LoadResources()
    {
        try
        {
            ResourceManager::LoadXML();
            ResourceManager::LoadSurfaces();
            ResourceManager::LoadSoundBuffers();
            ResourceManager::LoadFonts("fonts");
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    static void LoadSoundBuffers();

    /// <summary>
    /// Loads each Bitmap located at the path(s) specified in the paths file as Surface objects.
    /// </summary>
    static void LoadSurfaces();

    /// <summary>
    /// Loads each XML file located at the path(s) specified in the paths file as XElement objects.
    /// </summary>
    static void LoadXML();

    /// <summary>
    /// Releases all the Surfaces and SoundObjects contained within the resource manager. 
    /// Once released, these resources have to be reloaded before they can be used again.
    /// </summary>
    static void Release()
    {
        for each(KeyValuePair<String ^, Surface ^> ^kvp in surfaces)
        {
            kvp->Value->Release();
        }

        for each(KeyValuePair<String ^, SoundBuffer ^> ^kvp in sounds)
        {
            kvp->Value->Release();
        }

        surfaces->Clear();
        sounds->Clear();
    }

    /// <summary>
    /// Reloads each Surface object if restoring failed after the user alt-tabbed.
    /// </summary>
    static void ReloadSurfaces();

    /// <summary>
    /// 
    /// </summary>
    static void ReloadSoundBuffers();

    /// <summary>
    /// 
    /// </summary>
    static void Shutdown()
    {
        fonts->Clear();
        xmlFiles->Clear();

        Release();
    }
};
