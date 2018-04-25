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
#include "scene.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Xml::Linq;

/// <summary>
/// Controls all the scenes within the game and acts as an intermediary 
/// between each scene and all the other subsystems within the game.
/// </summary>
public ref class SceneManager abstract sealed
{
private:
    // All the scenes read from the XML file, the key being the scenes name 
    // as read from scenes id attribute.
    static Dictionary<String ^, Scene ^> ^scenes;

    // the scene that's currently being displayed to the user.
    static Scene ^currentScene;
public:
    static SceneManager()
    {
        scenes = gcnew Dictionary<String ^, Scene ^>();
        currentScene = nullptr;
    }

    /// <summary>
    /// Builds an internal collection of Scenes based on the contents of the XML file and sets the 
    /// specified scene as the initial scene that's displayed when the game first starts. If an initial
    /// scene isn't specified in the XML file, it selects the first scene parsed from the XML file. If no
    /// scenes are read from the XML file, a NoScenesAvailableException is thrown.
    /// </summary>
    /// <param name="scenesXML"></param>
    static void Initialise(XElement ^scenesXML);

    /// <summary>
    /// Specifies which scene to display to the user, along with optional arguments which can be
    /// consumed by its child controls.
    /// </summary>
    /// <param name="scene">the scene to be presented to the user.</param>
    /// <param name="sceneArgs">the optional arguments to pass to the scene.</param>
    /// <exception cref="System::ArgumentException">scene evaluates to String::Empty</exception>
    /// <exception cref="System::ArgumentNullException"><i>scene</i> is <b>null</b>.</exception>
    /// <remarks>arguments specified in <i>sceneArgs</i> are read from a controls navigateTo attribute
    /// in the xml file.</remarks>
    static void SetActiveScene(String ^scene, array<Object ^, 1> ^sceneArgs);

    /// <summary>
    /// Gets the scene that's currently being displayed.
    /// </summary>
    static property Scene ^CurrentScene
    {
        Scene ^get()
        {
            return currentScene;
        }
    }
};