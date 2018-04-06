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

#include "logmanager.h"

void LogManager::Write(List<ILog ^> ^subscriberPool, String ^message)
{
    // catch blocks are empty blocks are empty because it has to fail silently.
    for(int i = 0; i < subscriberPool->Count; i++)
    {
        try
        {
            debugSubscribers[i]->Write(message);
        }
        catch(...)
        {

        }
    }
}

void LogManager::Add(LogType flags, ILog ^subscriber)
{
    if(subscriber == nullptr)
    {
        return;
    }

    if((flags & LogType::Debug) == LogType::Debug)
    {
        debugSubscribers->Add(subscriber);
    }

    if((flags & LogType::Log) == LogType::Log)
    {
        logSubscribers->Add(subscriber);
    }

    if((flags & LogType::Error) == LogType::Error)
    {
        errorSubscribers->Add(subscriber);
    }
}

void LogManager::Remove(LogType flags, ILog ^subscriber)
{
    if(subscriber == nullptr)
    {
        return;
    }

    if((flags & LogType::Debug) == LogType::Debug)
    {
        debugSubscribers->Remove(subscriber);
    }

    if((flags & LogType::Log) == LogType::Log)
    {
        logSubscribers->Remove(subscriber);
    }

    if((flags & LogType::Error) == LogType::Error)
    {
        errorSubscribers->Remove(subscriber);
    }
}

void LogManager::Write(LogType flags, String ^message, ...array<Object^> ^args)
{
    if(message == nullptr)
    {
        return;
    }

    String ^formattedMessage = String::Format(message, args);

    if((flags & LogType::Debug) == LogType::Debug)
    {
        Write(debugSubscribers, formattedMessage);
    }

    if((flags & LogType::Log) == LogType::Log)
    {
        Write(logSubscribers, formattedMessage);
    }

    if((flags & LogType::Error) == LogType::Error)
    {
        Write(errorSubscribers, formattedMessage);
    }
}

void LogManager::WriteLine(LogType flags, String ^message, ...array<Object^> ^args)
{
    Write(flags, message += Environment::NewLine, args);
}
