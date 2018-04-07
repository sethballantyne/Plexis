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

#include "ilog.h"
#include "logtype.h"

using namespace System;
using namespace System::Collections::Generic;

/// <summary>
/// LogManager maintains pools of subscribers that subscribe to certain types of messages; 
/// currently these are error messages, debug messages and generic log messages.
/// When LogManager::Write or LogManager::WriteLine is called, all subscribers in the specified pool receive the
/// message and log or present it, based on how ILog::Write and ILog::WriteLine are implemented by the subscriber.
/// By design, LogManager handles errors silently.This is so if it fails, it doesn't bring down the rest of the 
/// program with it.
/// </summary>
public ref class LogManager abstract sealed
{
private:
    // subscribers that want to log error messages.
    static List<ILog ^> ^errorSubscribers;

    // subscribers that want to receive any messages that aren't related to errors or debug information.
    static List<ILog ^> ^logSubscribers;

    // subscribers that want to receive messages containing debug information.
    static List<ILog ^> ^debugSubscribers;

    /// <summary>
    /// internal function used to write <i>message</i> to the specified pool.
    /// </summary>
    /// <param name="subscriberPool">the pool receiving the message.</param>
    /// <param name="message">the message to send.</param>
    static void Write(List<ILog ^> ^subscriberPool, String ^message);
public:
    static LogManager()
    {
        errorSubscribers = gcnew List<ILog ^>();
        logSubscribers = gcnew List<ILog ^>();
        debugSubscribers = gcnew List<ILog ^>();
    }

    /// <summary>
    /// Adds the subscriber to the specified list. 
    /// Flags can be OR'd, allowing subscription to more than one list. 
    /// </summary>
    /// <param name="flags">The pool the subscriber is to be added to.</param>
    /// <param name="subscriber">The subscriber to add.</param>
    static void Add(LogType flags, ILog ^subscriber);

    /// <summary>
    /// Removes the subscriber from the specified list. 
    /// Like Add, Flags can be OR'd resulting in removal from multiple lists.
    /// </summary>
    /// <param name="flags">The pool the subscriber is to be removed from.</param>
    /// <param name="subscriber">The subscriber to remove.</param>
    static void Remove(LogType flags, ILog ^subscriber);

    /// <summary>
    /// Passes <i>message</i> to all subscribers of the specified list. 
    /// Flags can be OR'd together to include multiple lists.
    /// </summary>
    /// <param name="flags">The pool(s) to send the message to.</param>
    /// <param name="message">The message to send.</param>
    /// <param name="args"></param>
    static void Write(LogType flags, String ^message, ...array<Object ^> ^args);

    /// <summary>
    /// Passes <i>message</i> to all subscribers of the specified list(s), with a new line character
    /// appended. Flags can be OR'd together to include multiple lists.
    /// </summary>
    /// <param name="flags">The pool(s) to send the message to.</param>
    /// <param name="message">The message to send.</param>
    /// <param name="args"></param>
    static void WriteLine(LogType flags, String ^message, ...array<Object ^> ^args);
};