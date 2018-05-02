#include "font.h"
#include "video.h"
#include "logmanager.h"

using namespace System::Text;
using namespace System::Collections::Generic;

void Font::Init(String ^name, Surface ^fontBitmap, unsigned int glyphWidth, unsigned int glyphHeight)
{
    if(nullptr == name)
    {
        throw gcnew ArgumentNullException("name");
    }
    else if(nullptr == fontBitmap)
    {
        throw gcnew ArgumentNullException("fontBitmap");
    }

    if(String::Empty == name)
    {
        throw gcnew ArgumentException("name evaluates to String::Empty.");
    }
    else if(glyphWidth > fontBitmap->Width)
    {
        throw gcnew ArgumentException("the specified width of the glyph is greater than the width "
            "of the font surface.");
    }
    else if(glyphHeight > fontBitmap->Height)
    {
        throw gcnew ArgumentException("the specified height of the glyph is greater than the hieght"
            " of the font surface.");
    }

    this->glyphWidth = glyphWidth;
    this->glyphHeight = glyphHeight;
    this->fontSurface = fontBitmap;
    this->name = name;

    // A bitmap character only contains ASCII values 32 to 126.
    int glyphsPerSheet = 94;
    this->glyphPositions = gcnew array<System::Drawing::Point, 1>(glyphsPerSheet);

    for(int i = 0; i < glyphsPerSheet; i++)
    {
        int x = i * this->glyphWidth;
        glyphPositions[i] = System::Drawing::Point(x, 0);
    }
}

void Font::Render(int x, int y, String ^text, ...array<Object ^> ^args)
{
    if(nullptr == text)
    {
        throw gcnew ArgumentNullException("text");
    }

    String ^formattedText = String::Format(text, args);

    array<unsigned char, 1> ^ascii = Encoding::ASCII->GetBytes(formattedText);

    System::Drawing::Rectangle destRect;
    System::Drawing::Rectangle srcRect;

    try
    {
        for(int i = 0; i < ascii->Length; i++)
        {
            destRect.X = x + (i * (glyphWidth - 1));
            destRect.Y = y;
            destRect.Width = glyphWidth;
            destRect.Height = glyphHeight;

            // subtracting 32 because the first character in a bitmap font
            // is the space character, which is decimal 32. A bitmap font contains
            // the characters decimal 32 (space)to decimal 126 (~).
            int index = ascii[i] - 32;
            if(index < 0)
            {
                // probably just processed a null character, lets ignore it.
                continue;
            }

            srcRect.X = glyphPositions[index].X;
            srcRect.Y = 0;
            srcRect.Width = glyphWidth;
            srcRect.Height = glyphHeight;

            Video::Blit(srcRect, destRect, fontSurface);
        }
    }
    catch(...)
    {
        throw;
    }
}