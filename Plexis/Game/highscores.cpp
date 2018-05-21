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
#include "highscores.h"
#include "logmanager.h"

void HighScores::Initialise(String ^filename)
{
    if(nullptr == filename)
    {
        throw gcnew ArgumentNullException("filename");
    }
    else if(String::Empty == filename)
    {
        throw gcnew ArgumentException("filename evaluates to String::Empty.");
    }

    try
    {
        HighScores::fileStream = gcnew FileStream(filename, FileMode::OpenOrCreate, FileAccess::ReadWrite);
        HighScores::binaryReader = gcnew BinaryReader(fileStream, Encoding::ASCII);
        HighScores::binaryWriter = gcnew BinaryWriter(fileStream, Encoding::ASCII);

        if(fileStream->Length != 0)
        {
            
            unsigned int numberOfEntries = binaryReader->ReadUInt32();
            if(numberOfEntries != defaultNumberOfEntries)
            {
                LogManager::WriteLine(LogType::Debug, "Highscores.cpp: the number of entries being read from "
                    "the highscores file differs from defaultNumberOfEntries.");
            }
            for(int i = 0; i < numberOfEntries; i++)
            {
                HighScoreRecord ^highScoreRecord = gcnew HighScoreRecord();

                highScoreRecord->SetPlayerName(
                    binaryReader->ReadBytes(highScoreRecord->GetPlayerName()->Length)
                    );

                highScoreRecord->Score = binaryReader->ReadUInt32();

                LogManager::WriteLine(LogType::Debug, "Name: {0} Score: {1}", highScoreRecord->PlayerNameAsString(), highScoreRecord->Score);
                highScores[i] = highScoreRecord;
            }
        }
    }
    catch(Exception ^e)
    {
        LogManager::WriteLine(LogType::Debug, "Exception {0} caught while attempting to read from highscores.dat. Defaults will be used.", e->GetType()->ToString());
    }
    catch(...)
    {
        LogManager::WriteLine(LogType::Debug, "Unmanaged exception caught while attempting to read from highscores.dat. Defaults will be used.");
    }
}

void HighScores::Update(unsigned int row, array<unsigned char, 1> ^playerName, unsigned int newHighScore)
{
    if(nullptr == playerName)
    {
        throw gcnew ArgumentNullException("playerName");
    }
    else if(row >= highScores->Count)
    {
        throw gcnew ArgumentOutOfRangeException("row");
    }

    highScores[row]->SetPlayerName(playerName);
    highScores[row]->Score = newHighScore;

    try
    {
        fileStream->Flush();
        fileStream->SetLength(0);
        binaryWriter->Write(highScores->Count);

        for(int i = 0; i < highScores->Count; i++)
        {
            binaryWriter->Write(highScores[i]->GetPlayerName());
            binaryWriter->Write(highScores[i]->Score);
        }
    }
    catch(...)
    {
        throw;
    }
}