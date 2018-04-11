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
#include <strsafe.h>
#include <windows.h>
#include <vcclr.h>
#include "engine_exceptions.h"

String ^Win32Exception::Format(String ^function)
{
    if(function == nullptr)
    {
        throw gcnew ArgumentNullException("function");
    }

    LPVOID lpMessageBuffer;
    LPVOID lpDisplayBuffer;
    DWORD lastError = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        lastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMessageBuffer,
        0, NULL);

    pin_ptr<const wchar_t> pinnedFunctionString = PtrToStringChars(function);

    // allocate memory for the message string
    lpDisplayBuffer = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMessageBuffer) + function->Length + 40) * sizeof(TCHAR));

    // print the string into the allocated buffer.
    StringCchPrintf((LPTSTR)lpDisplayBuffer,
        LocalSize(lpDisplayBuffer) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        pinnedFunctionString, pinnedFunctionString, lpMessageBuffer);

    // convert the buffer to a managed string.
    String ^formattedMessage = gcnew String((wchar_t *) lpDisplayBuffer);

    LocalFree(lpMessageBuffer);
    LocalFree(lpDisplayBuffer);

    return formattedMessage;
}
