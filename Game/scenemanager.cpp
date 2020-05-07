#include "scenemanager.h"
#include "engine_exceptions.h"
#include "scenefactory.h"

void SceneManager::Initialise(XElement ^scenesXML)
{
    if(nullptr == scenesXML)
    {
        throw gcnew ArgumentNullException("scenesXML");
    }

    try
    {
        array<Scene ^, 1> ^parsedScenes = (gcnew SceneFactory())->Read(scenesXML);
        if(0 == parsedScenes->Length)
        {
            throw gcnew NoScenesAvailableException("No scenes have been read from the XML file, so.. no game for you.");
        }

        for(int i = 0; i < parsedScenes->Length; i++)
        {
            scenes->Add(parsedScenes[i]->Name, parsedScenes[i]);
            if(parsedScenes[i]->IsInitialScene)
            {
                currentScene = parsedScenes[i];
            }
        }

        // no initial scene specified in the XML file, so just grab the first one.
        if(nullptr == currentScene)
        {
            currentScene = parsedScenes[0];
        }
    }
    catch(...)
    {
        throw;
    }
}

void SceneManager::SetActiveScene(String ^scene, array<Object ^, 1> ^sceneArgs)
{
    if(nullptr == scene)
    {
        throw gcnew ArgumentNullException("scene");
    }
    else if(String::Empty == scene)
    {
        throw gcnew ArgumentException("scene name evaluates to String::Empty.");
    }

    try
    {
        currentScene = scenes[scene];
        currentScene->BroadcastArguments(sceneArgs);
    }
    catch(...)
    {
        throw;
    }
}