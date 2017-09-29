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
