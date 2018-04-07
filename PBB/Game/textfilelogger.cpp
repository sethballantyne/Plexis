#include "textfilelogger.h"

TextFileLogger::TextFileLogger(String ^path)
{
    if(path == nullptr)
    {
        throw gcnew ArgumentNullException("path");
    }
    else if(path == String::Empty)
    {
        throw gcnew ArgumentException("path evaluates to String::Empty.");
    }

    try
    {
        streamWriter = gcnew StreamWriter(path, true);
    }
    catch(...)
    {
        throw;
    }
}

void TextFileLogger::Write(String ^message)
{
    try
    {
        String ^timeStamp = DateTime::Now.ToString("dd/MM/yy HH:mm:ss");

        this->streamWriter->Write("[{0}] {1}", timeStamp, message);
        this->streamWriter->Flush();
    }
    catch(...)
    {

    }
}

void TextFileLogger::WriteLine(String ^message)
{
    this->Write(message += Environment::NewLine);
}