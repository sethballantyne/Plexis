#include <vcclr.h>
#include "messageboxlogger.h"

MessageBoxLogger::MessageBoxLogger(HWND hWnd)
{
    {
        if(!hWnd)
        {
            throw gcnew ArgumentNullException("hWnd is NULL.");
        }
        else
        {
            this->hWnd = hWnd;
        }
    }
}

void MessageBoxLogger::Write(String ^message)
{
    pin_ptr<const wchar_t> unmanagedString = PtrToStringChars(message);

    MessageBox(this->hWnd, unmanagedString, L"Unholy Error", MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
}

// no idea why you'd use this function but it's here for completeness.
void MessageBoxLogger::WriteLine(String ^message)
{
    Write(message += Environment::NewLine);
}