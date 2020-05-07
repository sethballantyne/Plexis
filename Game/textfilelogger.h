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

using namespace System;
using namespace System::IO;

/// <summary>
/// Writes time-stamped messages to a text file. TextFileLogger is designed to be used as 
/// a subscriber to LogManager.
/// </summary>
public ref class TextFileLogger : public ILog
{
private:
    // the underlying stream responsible for writing to the text file.
    StreamWriter ^streamWriter;
public:
    /// <summary>
    /// Puts the object into a state where it can write to the text file. 
    /// </summary>
    /// <param name="path">The path (including file name) of the file to use as the log.</param>
    /// <exception cref="System::ArgumentNullException"><i>path</i> is <b>null</b>.</exception>
    /// <exception cref="System::ArgumentException"><i>path</i> evaluates to String::Empty, or contains the name of a system device.</exception>
    /// <exception cref="System::UnauthorizedAccessException">Access to the file has been denied by the host operating system.</exception>
    /// <exception cref="System::IO::DirectoryNotFoundException">The specified path is invalid.</exception>
    /// <exception cref="System::IO::IOException"><i>path</i> contains illegal characters or is incorrectly formatted.</exception>
    /// <exception cref="System::IO::PathTooLongException">the path exceeds the maximum length defined by the operating system.</exception>
    /// <exception cref="System::Security::SecurityException">The application doesn't have the required permissions.</exception>
    /// <remarks>if the log already exists when the constructor is called, its contents is cleared.</remarks>
    TextFileLogger(String ^path);

    /// <summary>
    /// Closes the underlying stream. Once the stream is closed, the text file can't be written to.
    /// </summary>
    void Close()
    {
        if(streamWriter != nullptr)
        {
            try
            {
                streamWriter->Flush();
            }
            catch(...)
            {
                //  deliberately silent; I don't want to kill the application just because
                // the program has failed to log some text.
            }

            streamWriter->Close();
            streamWriter = nullptr;
        }
    }

    /// <summary>
    /// Writes a time-stamped mmessage to the text file. Any exceptions thrown by the
    /// StreamWriter instance are ignored.
    /// </summary>
    /// <param name="message">the text to the text file.</param>
    void virtual Write(String ^message);

    /// <summary>
    /// Writes a time-stamped mmessage to the text file along with an appended newline character.
    /// </summary>
    /// <param name="message">the text to the text file.</param>
    void virtual WriteLine(String ^message);

    !TextFileLogger() 
    { 
        Close(); 
    }

    ~TextFileLogger()
    {
        Close();
    }
};