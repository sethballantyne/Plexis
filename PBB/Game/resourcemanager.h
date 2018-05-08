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

using namespace System::Xml;
using namespace System::Xml::Linq;
using namespace System::Collections;
using namespace System::Collections::Generic;

/// <summary>
/// The resource manager creates, manages and destroys all the games assets. 
/// All objects created by the resource manager must be destroyed by the resource manager. 
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
    /// <param name="path">the path to search for in <i>pathList</i>.</param>
    /// <returns><b>true</b> if <i>pathList</i> contains a ResourcePath instance that points to the specified path,
    /// else returns <b>false.</b></returns>
    static bool PathExists(List<ResourcePath ^> ^pathList, String ^path);

    /// <summary>
    /// Creates a Font object from each &lt;font&gt; element read in fonts.xml
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
    /// <param name="file">the name of the XML file that contains font information, minus the .xml extension.</param>
    /// <remarks>this method can be called only after both the xml files and surfaces have been loaded
    /// as it depends on both having been completed successfully in order to execute correctly.</remarks>
    /// <exception cref="System::ArgumentException"><i>file</i> evaluates to String::Empty.</exception>
    /// <exception cref="System::ArgumentNullException"><i>file</i> is <b>null</b>.</exception>
    /// <exception cref="ResourceNotFoundException">the resource manager doesn't have the requested XML file.</exception>
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
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectSound returned an unspecified COM+ exception.</exception>
    /// <exception cref="System::IO::DirectoryNotFoundException">paths.xml contains a path for audio resources that doesn't exist.</exception>
    /// <exception cref="DirectSoundAllocatedException">the request failed because audio resources were already in use by another caller.</exception>
    /// <exception cref="DirectSoundBadFormatException">the specified wave format is not supported.</exception>
    /// <exception cref="DirectSoundBufferLostException">the buffer memory has been lost and must be restored.</exception>
    /// <exception cref="DirectSoundBufferTooSmallException">the buffer size is not big enough.</exception>
    /// <exception cref="DirectSoundControlUnavailableException">the buffer control requested by the caller is not available.</exception>
    /// <exception cref="DirectSoundInvalidCallException">this function is not valid for the current state of this object.</exception>
    /// <exception cref="DirectSoundInvalidParameterException">an invalid parameter was passed to the returning function.</exception>
    /// <exception cref="DirectSoundNoAggregationException">the object does not support aggregation.</exception>
    /// <exception cref="DirectSoundPriorityLevelNeededException">A cooperative level of DSSCL_PRIORITY or higher is required.</exception>
    /// <exception cref="DirectSoundUninitializedException">DirectSound hasn't been initialised.</exception>
    /// <exception cref="DirectSoundUnsupportedException">the DirectSound function called is not supported.</exception>
    /// <exception cref="DirectSoundVersion8RequiredException">DirectSound8 is required to complete the operation.</exception>
    /// <exception cref="System::IO::EndOfStreamException">the end of the file was reached prematurely.</exception>
    /// <exception cref="System::IO::FileFormatException">the specified file isn't a PCM wav file.</exception>
    /// <exception cref="System::IO::PathTooLongException">paths.xml contains a path for audio resources 
    /// that's longer than the OS-defined maximum length for a path.</exception>
    static void LoadSoundBuffers();

    /// <summary>
    /// Loads each Bitmap located at the path(s) specified in the paths file as Surface objects.
    /// </summary>
    /// <exception cref="DirectDrawDCAlreadyCreatedException">a device context has already been returned for this surface.</exception>
    /// <exception cref="DirectDrawGenericException">generic error occured, fucked if I know what's going on.</exception>
    /// <exception cref="DirectDrawIncompatiblePrimarySurfaceException">the image surface is incompatible with the format of the existing primary surface.</exception>
    /// <exception cref="DirectDrawInvalidCapsException">one or more of the capability bits is incorrect.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawInvalidPixelFormatException">an invalid pixel format was specified.</exception>
    /// <exception cref="DirectDrawInvalidSurfaceTypeException">the requested operation could not be performed because the surface was of the wrong type.</exception>
    /// <exception cref="DirectDrawNoAlphaHardwareException">the video device doesn't support alpha hardware acceleration or it's unavailable.</exception>
    /// <exception cref="DirectDrawNoCooperativeLevelSetException">attempting to create a surface without first setting the cooperative level.</exception>
    /// <exception cref="DirectDrawNoHardwareException">the current device doesn't support hardware-only DirectDraw acceleration.</exception>
    /// <exception cref="DirectDrawNoEmulationException">software emulation isn't available.</exception>
    /// <exception cref="DirectDrawNoExclusiveModeException">exclusive mode required to complete the operation.</exception>
    /// <exception cref="DirectDrawNoFlipHardwareException">flipping visible surfaces is not supported by the video hardware.</exception>
    /// <exception cref="DirectDrawNoMipMapHardwareException">unable to complete the operation because mipmapping isn't supported by the hardware or is not available.</exception>
    /// <exception cref="DirectDrawNoOverlayHardwareException">unable to complete the operation because no hardware support for overlay is available.</exception>
    /// <exception cref="DirectDrawNoZBufferHardwareException">unable to complete the operation because no hardware support for Z-ordering overlays is available.</exception>
    /// <exception cref="DirectDrawOutOfVideoMemoryException">DirectDraw does not have enough display memory available to perform the operation.</exception>
    /// <exception cref="DirectDrawPrimarySurfaceAlreadyExistsException">a primary surface already exists.</exception>
    /// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone. Call IDirectDrawSurface7::Restore to restore the memory associated with it.</exception>
    /// <exception cref="DirectDrawUnsupportedException">the operation is not supported.</exception>
    /// <exception cref="DirectDrawUnsupportedModeException">unable to create a surface for the current display mode, it's unsupported.</exception>
    /// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation is incomplete.</exception>
    /// <exception cref="System::IO::DirectoryNotFoundException">paths.xml contains a path for bitmap resources that doesn't exist.</exception>
    /// <exception cref="OutOfMemoryException">DirectDraw does not have enough memory to perform the operation.</exception>
    /// <exception cref="System::IO::PathTooLongException">paths.xml contains a path for bitmap resources 
    /// that's longer than the OS-defined maximum length for a path.</exception>
    /// <exception cref="Win32Exception">The function was unable to load the image at the specified path.</exception>
    static void LoadSurfaces();

    /// <summary>
    /// Loads each XML file located at the path(s) specified in the paths file as XElement objects.
    /// </summary>
    /// <exception cref="System::IO::DirectoryNotFoundException">paths.xml contains a path for bitmap resources that doesn't exist.</exception>
    /// <exception cref="System::IO::PathTooLongException">paths.xml contains a path for xml resources 
    /// that's longer than the OS-defined maximum length for a path.</exception>
    /// <exception cref="System::Xml::XmlException">An error occured while parsing an XML file.</exception>
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
    /// <exception cref="DirectDrawDCAlreadyCreatedException">a device context has already been returned for this surface.</exception>
    /// <exception cref="DirectDrawGenericException">generic error occured, fucked if I know what's going on.</exception>
    /// <exception cref="DirectDrawIncompatiblePrimarySurfaceException">the image surface is incompatible with the format of the existing primary surface.</exception>
    /// <exception cref="DirectDrawInvalidCapsException">one or more of the capability bits is incorrect.</exception>
    /// <exception cref="DirectDrawInvalidObjectException">DirectDraw received a pointer that was an invalid DirectDraw object.</exception>
    /// <exception cref="DirectDrawInvalidParametersException">one or more of the parameters passed to the method are incorrect.</exception>
    /// <exception cref="DirectDrawInvalidPixelFormatException">an invalid pixel format was specified.</exception>
    /// <exception cref="DirectDrawInvalidSurfaceTypeException">the requested operation could not be performed because the surface was of the wrong type.</exception>
    /// <exception cref="DirectDrawNoAlphaHardwareException">the video device doesn't support alpha hardware acceleration or it's unavailable.</exception>
    /// <exception cref="DirectDrawNoCooperativeLevelSetException">attempting to create a surface without first setting the cooperative level.</exception>
    /// <exception cref="DirectDrawNoHardwareException">the current device doesn't support hardware-only DirectDraw acceleration.</exception>
    /// <exception cref="DirectDrawNoEmulationException">software emulation isn't available.</exception>
    /// <exception cref="DirectDrawNoExclusiveModeException">exclusive mode required to complete the operation.</exception>
    /// <exception cref="DirectDrawNoFlipHardwareException">flipping visible surfaces is not supported by the video hardware.</exception>
    /// <exception cref="DirectDrawNoMipMapHardwareException">unable to complete the operation because mipmapping isn't supported by the hardware or is not available.</exception>
    /// <exception cref="DirectDrawNoOverlayHardwareException">unable to complete the operation because no hardware support for overlay is available.</exception>
    /// <exception cref="DirectDrawNoZBufferHardwareException">unable to complete the operation because no hardware support for Z-ordering overlays is available.</exception>
    /// <exception cref="DirectDrawOutOfVideoMemoryException">DirectDraw does not have enough display memory available to perform the operation.</exception>
    /// <exception cref="DirectDrawPrimarySurfaceAlreadyExistsException">a primary surface already exists.</exception>
    /// <exception cref="DirectDrawSurfaceLostException">access to the surface is refused because the surface memory is gone. Call IDirectDrawSurface7::Restore to restore the memory associated with it.</exception>
    /// <exception cref="DirectDrawUnsupportedException">the operation is not supported.</exception>
    /// <exception cref="DirectDrawUnsupportedModeException">unable to create a surface for the current display mode, it's unsupported.</exception>
    /// <exception cref="DirectDrawWasStillDrawingException">the previous blit operation is incomplete.</exception>
    /// <exception cref="System::OutOfMemoryException">DirectDraw does not have enough memory to perform the operation.</exception>
    /// <exception cref="Win32Exception">The function was unable to load the image at the specified path.</exception>
    static void ReloadSurfaces();

    /// <summary>
    /// Reloads all the audio resources on the event DirectSound loses them due to alt-tabbing etc.
    /// </summary>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectSound returned an unspecified COM+ exception.</exception>
    /// <exception cref="DirectSoundAllocatedException">the request failed because audio resources were already in use by another caller.</exception>
    /// <exception cref="DirectSoundBadFormatException">the specified wave format is not supported.</exception>
    /// <exception cref="DirectSoundBufferLostException">the buffer memory has been lost and must be restored.</exception>
    /// <exception cref="DirectSoundBufferTooSmallException">the buffer size is not big enough.</exception>
    /// <exception cref="DirectSoundControlUnavailableException">the buffer control requested by the caller is not available.</exception>
    /// <exception cref="DirectSoundInvalidCallException">this function is not valid for the current state of this object.</exception>
    /// <exception cref="DirectSoundInvalidParameterException">an invalid parameter was passed to the returning function.</exception>
    /// <exception cref="DirectSoundNoAggregationException">the object does not support aggregation.</exception>
    /// <exception cref="DirectSoundPriorityLevelNeededException">A cooperative level of DSSCL_PRIORITY or higher is required.</exception>
    /// <exception cref="DirectSoundUninitializedException">DirectSound hasn't been initialised.</exception>
    /// <exception cref="DirectSoundUnsupportedException">the function called is not supported.</exception>
    /// <exception cref="DirectSoundVersion8RequiredException">DirectSound8 is required to complete the operation.</exception>
    /// <exception cref="System::IO::EndOfStreamException">the end of the file was reached prematurely.</exception>
    /// <exception cref="System::IO::FileFormatException">the specified file isn't a PCM wav file.</exception>
    /// <exception cref="System::IO::IOException">the method was unable to load the file specified in <i>filename</i> because it either couldn't be found or lacked the permissions to open it, or
    /// the file was corrupt.</exception>
    static void ReloadSoundBuffers();

    /// <summary>
    /// Frees the memory allocated by the resources and destroys all Surface and SoundBuffer objects.
    /// Once this is called, ResourceManager can't be used until ResourceManager::Initialise() is called
    /// again.
    /// </summary>
    static void Shutdown()
    {
        fonts->Clear();
        xmlFiles->Clear();

        Release();
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="volume"></param>
    static void UpdateVolume(unsigned int volume)
    {
        if(volume > 100)
            volume = 100;

        for each(KeyValuePair<String ^, SoundBuffer ^> ^keyValuePair in sounds)
        {
            if(keyValuePair->Value->Volume != volume)
            {
                sounds[keyValuePair->Key]->Volume = volume;
                //keyValuePair->Value->Volume = volume;
            }
        }
    }
};
