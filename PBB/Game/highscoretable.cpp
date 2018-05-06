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
#include "highscoretable.h"
#include "highscores.h"

HighScoreTable::HighScoreTable(int x, int y, int numberOfRows, int verticalSpacing,
    int indexXPosition, String ^rowNumberFont, int PlayerNameXPosition, String ^playerNameFont,
    int scoreXPosition, String ^scoreFont) : SelectableControl(x, y, -1, gcnew System::Drawing::Size(0, 0))
{
    if(nullptr == rowNumberFont)
    {
        throw gcnew ArgumentNullException("rowNumberFont");
    }
    else if(nullptr == playerNameFont)
    {
        throw gcnew ArgumentNullException("playerNameFont");
    }
    else if(nullptr == scoreFont)
    {
        throw gcnew ArgumentNullException("scoreFont");
    }

    if(String::Empty == rowNumberFont)
    {
        throw gcnew ArgumentException("rowNumberFont evaluates to String::Empty.");
    }
    else if(String::Empty == playerNameFont)
    {
        throw gcnew ArgumentException("playerNameFont evaluates to String::Empty.");
    }
    else if(String::Empty == scoreFont)
    {
        throw gcnew ArgumentException("scoreFont evaluates to String::Empty.");
    }

    this->Enabled = false;
    this->IsSelected = false;

    rows = gcnew List<HighScoreRow ^>();
    int yStep = 0;
    for(int i = 0; i < numberOfRows; i++)
    {
        // 18 is the height of the font; replace so it's calculated based on the
        // actual font being used.
        int rowYPosition = y + yStep;
        yStep = ((i + 1) * (18 + verticalSpacing));

        Point ^indexPosition = gcnew Point(indexXPosition, rowYPosition);
        Point ^playerNamePosition = gcnew Point(PlayerNameXPosition, rowYPosition);
        Point ^scorePosition = gcnew Point(scoreXPosition, rowYPosition);

        
        try
        {
            HighScoreRow ^newHighScoreRow = gcnew HighScoreRow(rowNumberFont, indexPosition,
                playerNameFont, playerNamePosition, scoreFont, scorePosition);

            String ^indexText;
            // adding 1 to i because the range used to indicate a players position
            // on the highscore table needs to be 1 to 10, not 0 to 9.
            if(i < 9)
            {
                indexText = String::Format(" {0})", i + 1);
            }
            else
            {
                indexText = String::Format("{0})", i + 1);
            }

            newHighScoreRow->Index->Text = indexText;
            newHighScoreRow->PlayerName->Text = HighScores::GetPlayerName(i);

            // the score text is right aligned, calculating how many spaces need to be
            // added so it's correctly padded.
            // we want this:
            // 99999
            //   999
            // not this:
            // 99999
            // 999
            StringBuilder ^sb = gcnew StringBuilder(Convert::ToString(HighScores::GetHighScore(i)));
            int numberOfSpacesToInsert = 5 - sb->Length;
            sb->Insert(0, " ", numberOfSpacesToInsert);
            newHighScoreRow->Score->Text = sb->ToString();

            rows->Add(newHighScoreRow);
        }
        catch(...)
        {
            throw;
        }
    }
}

void HighScoreTable::ReceiveSceneArgs(array<Object ^, 1> ^args)
{
    if(nullptr != args && args->Length >= 1)
    {
        try
        {
            int arg = Convert::ToInt32(args[0]);
            rows[arg]->PlayerName->Text = HighScores::GetPlayerName(arg);
            rows[arg]->Score->Text = Convert::ToString(HighScores::GetHighScore(arg));
        }
        catch(...)
        {
            LogManager::WriteLine(LogType::Debug, "Error converting the 0th scene argument in HighScoreTable.");
        }
    }
}