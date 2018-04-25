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


/// <summary>
/// Represents a single <path> element in paths.xml.
/// For every <path> element parsed by the resource manager, one resulting instance of this
/// class will be created. Resources can have multiple instances of this class, each instance
/// pointing to a different directory location.
/// </summary>
public ref class ResourcePath : public IEquatable<ResourcePath ^>
{
private:
    // the directory containing the resource(s). Paths are relative to the game binary.
    String ^path;

    // determines whether the subdirectories in the location pointed to by path are searched by the 
    // searched by the resource manager when loading resources.
    bool includeSubDirs;
public:
    /// <summary>
    /// Initialises a new instance of ResourcePath with a string specifying the location of resources 
    /// to be consumed by the resource manager and a boolean value indicating whether the directory
    /// contains subdirectories that should also be scanned for resources.
    /// </summary>
    /// <param name="path">the path containing the resources, relative to the game binary.</param>
    /// <param name="includeSubDirs"><b>true</b> if the resource manager should scan the subdirectories of the directory
    /// specified in <i>path</i> for resources, otherwise <b>false</b>.</param>
    /// <exception cref="System::ArgumentException"><i>path</i> evaluates to String::Empty or contains only whitespace.</exception>
    /// <exception cref="System::ArgumentNullException"><i>path</i> is <b>null</b></exception>
    ResourcePath(String ^path, bool includeSubDirs)
    {
        if(nullptr == path)
        {
            throw gcnew ArgumentNullException("path");
        }

        if(String::IsNullOrWhiteSpace(path) || String::Empty == path)
        {
            throw gcnew ArgumentException("Empty strings or strings containing whitespace are not valid paths.");
        }
        else
        {
            this->path = path;
            this->includeSubDirs = includeSubDirs;
        }
    }

    /// <summary>
    /// Determines whether this instance and another specified ResourcePath object have the same path.
    /// </summary>
    /// <param name="obj">the resource path to compare against.</param>
    /// <returns><b>true</b> if <i>obj</i> contains the same path, otherwise <b>false</b>; if <i>obj</i> is <b>null</b>, <b>false</b> is returned.</returns>
    virtual bool Equals(ResourcePath ^obj)
    {
        if(nullptr == obj)
        {
            return false;
        }

        return path->Equals(obj->Path);
    }

    /// <summary>
    /// Returns true if the resource manager should scan the subdirectories of ResourcePath::Path for
    /// resources, otherwise returns false.
    /// </summary>
    property bool IncludeSubDirectories
    {
        bool get()
        {
            return includeSubDirs;
        }
        void set(bool value)
        {
            this->includeSubDirs = value;
        }
    }

    /// <summary>
    /// Gets or sets the directory assigned to the object.
    /// </summary>
    /// <exception cref="System::ArgumentException">Attempting to assign an empty string or a string that contains only whitespace.</exception>
    /// <exception cref="Systemm::ArgumentNullException">Attempting to assign a <b>null</b> value.</exception>
    property String ^Path
    {
        String ^get()
        {
            return this->path;
        }

        void set(String ^value)
        {
            if(nullptr == value)
            {
                throw gcnew ArgumentNullException("value", "A path can't be null.");
            }
            else if(String::IsNullOrWhiteSpace(value) || String::Empty == value)
            {
                throw gcnew ArgumentException("Empty strings or strings containing whitespace are not valid paths.");
            }
            else
            {
                this->path = value;
            }
        }
    }
};