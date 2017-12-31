/*Copyright (c) Seth Ballantyne <seth.ballantyne@gmail.com>
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in
*all copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*THE SOFTWARE.
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace Level_Editor
{
    class PaintedBricks
    {
        List<Point> brickLocations = new List<Point>();

        // index of the brick in the palette listview. 
        short brickIndex = -1;

        public PaintedBricks(Point[] brickLocations, short index)
        {
            if (brickLocations == null)
            {
                throw new ArgumentNullException("brickLocations");
            }

            this.brickLocations.AddRange(brickLocations);
            brickIndex = index;
        }

        public PaintedBricks(ushort x, ushort y, short index)
        {
            Point p = new Point(x, y);

            brickLocations.Add(p);
            brickIndex = index;
        }

        public Point[] GetPoints()
        {
            return brickLocations.ToArray();
        }

        public void AddCoordinate(ushort x, ushort y)
        {
            Point p = new Point(x, y);

            brickLocations.Add(p);
        }

        public short Index
        {
            get { return brickIndex; }
            set { brickIndex = value; }
        }

        public void Clear()
        {
            brickLocations.Clear();
            brickIndex = -1;
        }
    }
}
