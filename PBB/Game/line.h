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

using namespace System;
using namespace System::Drawing;

/// <summary>
/// Stores two points and a colour that represent the position, length and colour of a line.
/// </summary>
public ref class Line
{
private:
    // the lines starting position.
    Point from;

    // the lines finishing position.
    Point to;

    // the lines colour. Stick to facebook if I really need to explain this.
    Color colour;

public:
    /// <summary>
    /// Initialises a new instance of the Line class with the specified location and colour.
    /// </summary>
    /// <param name="colour">the lines colour... derp derp derp.</param>
    /// <param name="from">the lines starting point.</param>
    /// <param name="to">the lines finishing point.</param>
    Line(Color colour, Point from, Point to)
    {
        this->from = from;
        this->to = to;
        this->colour = colour;
    }

    /// <summary>
    /// Initialises a new instance of the Line class with the specified location and colour.
    /// </summary>
    /// <param name="colour">the colour of the line. PRO TIP: don't set the colour to the same as the background!</param>
    /// <param name="fromX">the lines starting x coordinate.</param>
    /// <param name="fromY">the lines starting y coordinate.</param>
    /// <param name="toX">the lines finishing x coordinate.</param>
    /// <param name="toY">the lines finishing y coordinate.</param>
    Line(Color colour, int fromX, int fromY, int toX, int toY)
    {
        this->from = Point(fromX, fromY);
        this->to = Point(toX, toY);
        this->colour = colour;
    }

    /// <summary>
    /// Gets or sets the lines colour.
    /// </summary>
    property Color Colour
    {
        Color get()
        {
            return this->colour;
        }
        void set(Color value)
        {
            this->colour = value;
        }
    }

    /// <summary>
    /// Gets or sets the lines starting coordinates.
    /// </summary>
    property Point From
    {
        Point get()
        {
            return this->from;
        }
        void set(Point value)
        {
            this->from = value;
        }
    }

    /// <summary>
    /// Gets or sets the lines finishing coordinates.
    /// </summary>
    property Point To
    {
        Point get()
        {
            return this->to;
        }
        void set(Point value)
        {
            this->to = value;
        }
    }
};